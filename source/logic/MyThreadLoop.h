#pragma once
#include "../MyTypes.h"

//Don't touch
#define NumThreads 32

class MyThreadLoop {

    private:
        //threads
        abool threadGoMarker[NumThreads];
        std::thread* threads[NumThreads];
        abool threadTerminated[NumThreads];
        abool terminateThreads = false;
        abool pauseThreads = false;

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

        virtual void processTick(int threadIndex) = 0;

        virtual void onTickEnded() = 0;

        void tick_multiple_threads(int threadIndex) {

            threadGoMarker[threadIndex] = true;
            waitAllThreads();

            while (!terminateThreads) {

                ///Синхронизация начала тика
                threadGoMarker[threadIndex] = false;
                this->onTickStated();
                threadGoMarker[threadIndex] = true;
                waitAllThreads();


                ///Синхронизация завершения тика
                threadGoMarker[threadIndex] = false;

                this->processTick(threadIndex);
                threadGoMarker[threadIndex] = true;
                waitAllThreads();


                threadGoMarker[threadIndex] = false;
                this->onTickEnded();
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

                if ((threadsReady == NumThreads) && (!pauseThreads))
                    break;
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
