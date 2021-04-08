//
// Created by T-bag on 30-Nov-20.
//

#ifndef TETRAVEXGAME_THREADPOOL_H
#define TETRAVEXGAME_THREADPOOL_H


#include <condition_variable>
#include <functional>
#include <iostream>
#include <vector>
#include <thread>
#include <queue>

using namespace std;

//Class de création des threads et d'exécutions des tâches
class ThreadPool {
public:
    //Format d'une tâche
    using Task = function<void()>;

    ThreadPool(size_t nbThreads) {
        //Initialisation du nombre de threads
        start(nbThreads);
    }

    ~ThreadPool() {
        //Arrêt de tout les threads lors de la destruction de la class ThreadPool
        stop();
    }

    void addTask(Task task) {
        //Ajout d'une nouvelle tâche à la liste des tâches et notification des threads

        {
            unique_lock<mutex> lock{mu};
            listTasks.emplace(move(task));
        }

        var.notify_one();
    }

    
private:
    //Liste des threads
    vector<thread> listThreads;

    // Variable permettant de notifier le threads après l'ajout d'une nouvelle tâche à la liste des tâches
    condition_variable var;

    mutex mu;
    bool toStop = false;

    queue<Task> listTasks;

    void start(size_t nbThreads) {
        for (int i = 0; i < nbThreads; ++i) {
            listThreads.emplace_back([=] {
                while (true) {
                    Task task;

                    {
                        unique_lock<mutex> lock{mu};

                        var.wait(lock, [=] { return toStop || !listTasks.empty(); });

                        if (toStop && listTasks.empty())
                            break;

                        task = move(listTasks.front());
                        listTasks.pop();
                    }

                    task();
                }
            });
        }
    }
    
    void stop() noexcept {
        {
            unique_lock<mutex> lock{mu};
            toStop = true;
        }

        var.notify_all();

        for (auto &thread : listThreads)
            thread.join();
    }



};


#endif //TETRAVEXGAME_THREADPOOL_H

