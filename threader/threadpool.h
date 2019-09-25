#pragma once

#include <iostream>
#include <vector>
#include <queue>

#include <mutex>
#include <atomic>
#include <thread>
#include <condition_variable>

#include <boost/asio.hpp>

// based on works (spi*eno)
// of https://github.com/a-rodionov/Otus.Cpp.Homework13/blob/master/ThreadPool.h
// and https://www.gamedev.net/forums/topic/681670-threadpool-with-abortable-jobs-and-then-function/?tab=comments#comment-5308026

class ThreadPool{
public:
    // (de)constructs
    ThreadPool(boost::shared_ptr<boost::asio::io_service> io_service):io_service(io_service){

    }
//    ThreadPool(){};
   ~ThreadPool()
   {
       join_workers();
   }

   ThreadPool(const ThreadPool&) = delete ;
   ThreadPool& operator=(const ThreadPool&) = delete;
   ThreadPool(ThreadPool&&) = delete;
   ThreadPool& operator=(ThreadPool&&) = delete;


   auto add_worker(){
       std::lock_guard<std::mutex>  mlock(threads_mutex);
       if(threads.empty()){
           work = std::make_shared<boost::asio::io_service::work>(*io_service);
       }

       is_new_thread_started    = false;
       is_new_thread_on_pause   = true;
       is_new_thread_force_terminate = false;
       //
       // something to protect against fault execution, no clear for me, need to think
       //
       // this_thread::yield
       // the current thread and put it on the back of the queue of the same-priority threads that
       // are ready to run (and if there are no other threads at the same priority, yield has no effect).
       auto worker_thread = std::thread ([this](){
           while(is_new_thread_on_pause)
               std::this_thread::yield();   //wait until someone sets is_new_thread_on_pause = false;
           if(is_new_thread_force_terminate)
               return ;
           is_new_thread_started = true;
           for (;;) {
               try {
                   std::cout << "running " << &io_service << std::endl;
                   io_service->run();
                   break;
               } catch (std::exception &exc) {                   
                    std::cout << "thread pool error "  << &io_service << " "<<exc.what() << std::endl;
               }
           }

       });

       try {
           threads.push_back(std::move(worker_thread));
           is_new_thread_on_pause = false;
           while(!is_new_thread_started)
               std::this_thread::yield();
       }
       catch(std::exception& exc) {
           if(threads.empty()) {
               work.reset();
           }
           is_new_thread_force_terminate = true;
           is_new_thread_on_pause = false;
           if(worker_thread.joinable()) {
               worker_thread.join();
           }
           throw exc;
       }
       return threads.back().get_id();
   }



   void join_workers()
   {
        if(threads.empty())return;
        work.reset();
        for(auto &thr: threads){
            if(thr.joinable()) thr.join();
        }
   }


   void stop_workers()
   {
       std::lock_guard<std::mutex> mlock(threads_mutex);
       join_workers();
       threads.clear();
       io_service->reset();
   }

   template<class Task>
   void add_task(Task&& task){
       io_service->post(task);
   }

private:

   boost::shared_ptr<boost::asio::io_service> io_service;
   std::shared_ptr<boost::asio::io_service::work> work;

   std::vector<std::thread> threads;
   mutable std::mutex threads_mutex;

   std::queue<std::exception_ptr> exceptions;
   std::mutex exceptions_mutex;

   std::atomic_bool is_new_thread_on_pause;
   std::atomic_bool is_new_thread_force_terminate;
   std::atomic_bool is_new_thread_started;
   std::atomic_bool is_error_log_working{true};
};
