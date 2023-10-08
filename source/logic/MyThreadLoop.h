#pragma once
#include "../MyTypes.h"

//Don't touch
constexpr int NumThreads = 32;

class MyThreadLoop {

    private:
        //threads
        abool threadGoMarker[NumThreads];
        std::thread* threads[NumThreads];
        abool threadTerminated[NumThreads];
        abool terminateThreads = false;
        abool pauseThreads = false;
        abool syncGoFlag = false;

        long long poolTick = 0;

        abool isProcessing = false;

    public:

        MyThreadLoop(int threadCount) {

            this->pauseThreads = true;

            //Start threads
            for (int i = 0; i < NumThreads; i++) {
                threadGoMarker[i] = false;
                threads[i] = new std::thread(&MyThreadLoop::tick_multiple_threads, this, i);
            }

        }
        virtual void onTickStated() = 0;

        virtual void processTick(int threadIndex, long long poolTick) = 0;

        virtual void onTickEnded() = 0;

        void privateTickStated() {
            if (isProcessing) {
                return;
            }
            isProcessing = true;

            poolTick++;
            this->onTickStated();

        }

        void privateTickEnded() {

            if (!isProcessing) {
                return;
            }
            isProcessing = false;

            this->onTickEnded();
        }

        void tick_multiple_threads(int threadIndex) {

            threadGoMarker[threadIndex] = true;
            waitAllThreads();

            while (!terminateThreads) {

                ///Синхронизация начала тика
                threadGoMarker[threadIndex] = false;
                this->syncGoFlag = false;
                privateTickStated();
                threadGoMarker[threadIndex] = true;
                waitAllThreads();


                ///Синхронизация завершения тика
                threadGoMarker[threadIndex] = false;
                this->processTick(threadIndex, poolTick);
                this->syncGoFlag = false;
                threadGoMarker[threadIndex] = true;
                waitAllThreads();


                threadGoMarker[threadIndex] = false;
                this->privateTickEnded();
                this->syncGoFlag = false;

                threadGoMarker[threadIndex] = true;
                //Wait for threads to synchronize first time
                waitAllThreads();

            }


        }

        //Start all threads
        void StartThreads() {
            //Start threads
            for (int i = 0; i < NumThreads; i++) {
                threadGoMarker[i] = true;
            }
        }

        void PauseThreads() {
            pauseThreads = true;
        }

        void UnpauseThreads() {
            pauseThreads = false;
        }

        //Wait for all threads to finish their calculations
        void waitAllThreads() {

            uint threadsReady;

            for (;;) {

                threadsReady = 0;

                for (int i = 0; i < NumThreads; i++) {
                    if (threadGoMarker[i] == true) {
                        threadsReady++;
                    }
                }

                if ((threadsReady == NumThreads) && (!pauseThreads) || this->syncGoFlag ) {
                    this->syncGoFlag = true;
                    break;
                }
                Sleep(1);
                std::this_thread::yield();

            }

        }

        ~MyThreadLoop() {

            repeat(NumThreads)
                threadTerminated[i] = false;

            terminateThreads = true;

            for (;;)
            {
                uint tcount = 0;

                repeat(NumThreads)
                {
                    if (threadTerminated[i] == true)
                        ++tcount;
                }

                if (tcount == NumThreads)
                    break;

                repeat(NumThreads)
                    threadGoMarker[i] = true;

                pauseThreads = false;

                SDL_Delay(1);
            }

            repeat(NumThreads)
            {
                threads[i]->join();
            }
        }

};
