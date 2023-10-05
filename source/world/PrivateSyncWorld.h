#pragma once

#include "Cell.h"
#include "../Settings.h"
#include "../entity/Object.h"
#include "../entity/Bot.h"
#include <vector>

class PrivateSyncWorld {

    private:



    public:
        template<typename T> class thread_safe_vector {

                std::vector<T> vec;
                mutable std::mutex mut;

            public:
                T operator[](uint index) const {
                    auto lck = std::lock_guard(mut);
                    return vec[index];
                }

                void push_back(T value) {
                    auto lck = std::lock_guard(mut);
                    vec.push_back(value);
                }

                bool empty() {
                    auto lck = std::lock_guard(mut);
                    return vec.empty();
                }

                uint size() {
                    auto lck = std::lock_guard(mut);
                    return vec.size();
                }

                T at(uint index) {
                    auto lck = std::lock_guard(mut);
                    return vec.at(index);
                }

                std::vector<T>::iterator begin() {
                    auto lck = std::lock_guard(mut);
                    return vec.begin();
                }

                std::vector<T>::iterator end() {
                    auto lck = std::lock_guard(mut);
                    return vec.end();
                }

                void clear() {
                    auto lck = std::lock_guard(mut);
                    vec.clear();
                }

                void erase(std::vector<T>::const_iterator iterator) {
                    auto lck = std::lock_guard(mut);
                    vec.erase(iterator);
                }

                void moveTo(thread_safe_vector<T> & anotherVector) {
                    auto lck = std::lock_guard(mut);
                    for (T obj : vec) {

                        
                        if (!(Bot *)obj->isAlive) {
                            delete obj;
                            continue;
                        }

                        anotherVector.push_back(obj);

                    }
                    vec.clear();
                }

                T getRandomItem() {
                    auto lck = std::lock_guard(mut);
                    int randomIndex = rand() % vec.size();
                    T returnValue = vec.at(randomIndex);
                    vec.erase(vec.begin() + randomIndex);
                    return returnValue;
                }



        };

        std::mutex worldMutex;

        //Игровое поле
        Cell worldMap[FieldCellsWidth][FieldCellsHeight];

        //Список объектов ожидающих обновления
        thread_safe_vector<Object*> entityes;

        //Список обновленных объектов
        thread_safe_vector<Object*> tempEntityes;


        PrivateSyncWorld();

        //thread-unsafetly
        bool addObjectUnsafetly(Object* obj);

        bool removeObjectUnsafetly(int X, int Y);




};

