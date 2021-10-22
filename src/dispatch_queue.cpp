#include "dispatch_queue.h"


DispatchQueue::DispatchQueue(size_t thread_cnt) : threads_(thread_cnt)
{
    for(auto & thread : threads_)
    {
        thread = std::thread(&DispatchQueue::dispatch_thread_handler, this);
    }
}

DispatchQueue::~DispatchQueue()
{
    // Signal to dispatch threads that it's time to wrap up
    std::unique_lock<std::mutex> lock(lock_);
    quit_ = true;
    lock.unlock();
    cv_.notify_all();

    // Wait for threads to finish before we exit
    for(auto & thread : threads_)
    {
        if(thread.joinable())
        {
            thread.join();
        }
    }
}

void DispatchQueue::dispatch(const fp_t& op)
{
    std::unique_lock<std::mutex> lock(lock_);
    q_.push(op);

    // Manual unlocking is done before notifying, to avoid waking up
    // the waiting thread only to block again (see notify_one for details)
    lock.unlock();
    cv_.notify_one();
}

void DispatchQueue::dispatch(fp_t&& op)
{
    std::unique_lock<std::mutex> lock(lock_);
    q_.push(std::move(op));

    // Manual unlocking is done before notifying, to avoid waking up
    // the waiting thread only to block again (see notify_one for details)
    lock.unlock();
    cv_.notify_one();
}

void DispatchQueue::dispatch_thread_handler()
{
    std::unique_lock<std::mutex> lock(lock_);

    do {
        //Wait until we have data or a quit signal
        cv_.wait(lock, [this]{
            return (!q_.empty() || quit_);
        });

        //after wait, we own the lock
        if(!quit_ && !q_.empty())
        {
            auto op = std::move(q_.front());
            q_.pop();

            //unlock now that we're done messing with the queue
            lock.unlock();
            op();
            lock.lock();
        }
    } while (!quit_);
}
