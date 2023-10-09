#pragma once
#include "../MyTypes.h"

//Don't touch
constexpr int NumThreads = 32;

class MyThreadLoop {

    private:
        //threads
        long long threadGoMarker[NumThreads];
        std::thread* threads[NumThreads];
        abool threadTerminated[NumThreads];
        abool terminateThreads = false;
        abool pauseThreads = false;

        std::atomic_int64_t poolTick = 0;

        long long isProcessing = false;
        std::mutex syncMutex;

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
            auto lck = std::lock_guard{ syncMutex };

            if (isProcessing) {
                return;
            }
            isProcessing = true;

            this->onTickStated();
            poolTick++;

        }

        void privateTickEnded() {

            if (!isProcessing) {
                return;
            }

            this->onTickEnded();
        }

        void tick_multiple_threads(int threadIndex) {

            threadGoMarker[threadIndex] = poolTick;
            waitAllThreads(threadIndex);

            while (!terminateThreads) {
                ///Синхронизация начала тика

                privateTickStated();
                threadGoMarker[threadIndex]++;
                //waitAllThreads(threadIndex);


                this->processTick(threadIndex, poolTick);

                poolTick = threadGoMarker[threadIndex];
                waitAllThreads(threadIndex);

                isProcessing = false;

            }


        }

        //Start all threads
        void StartThreads() {
            //Start threads
            for (int i = 0; i < NumThreads; i++) {
                threadGoMarker[i] = poolTick;
            }
        }

        void PauseThreads() {
            pauseThreads = true;
        }

        void UnpauseThreads() {
            pauseThreads = false;
        }

        //Wait for all threads to finish their calculations
        void waitAllThreads(int threadIndex) {

            uint threadsReady;


            for (;;) {

                if (threadGoMarker[threadIndex] < poolTick) {
                    return;
                }

                threadsReady = 0;

                for (int i = 0; i < NumThreads; i++) {
                    if (threadGoMarker[i] >= poolTick) {
                        threadsReady++;
                    }
                }

                if ((threadsReady == NumThreads) && (!pauseThreads)) {
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
