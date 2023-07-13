#include "dispatch_queue.h"


DispatchQueue::DispatchQueue()
{
    thread_ = std::thread(&DispatchQueue::dispatch_thread_handler, this);
}

DispatchQueue::~DispatchQueue()
{
    std::unique_lock<std::mutex> lock(lock_);
    quit_ = true;
    lock.unlock();
    cv_.notify_all();
    if (thread_.joinable()) {
        thread_.join();
    }
}

void DispatchQueue::dispatch(fp_t&& op)
{
    std::unique_lock<std::mutex> lock(lock_);
    q_.push(std::move(op));
    lock.unlock();
    cv_.notify_one();
}

void DispatchQueue::dispatch_thread_handler()
{
    std::unique_lock<std::mutex> lock(lock_);
    do {
        cv_.wait(lock, [this]{
            return (!q_.empty() || quit_);
        });

        if(!quit_ && !q_.empty())
        {
            auto op = std::move(q_.front());
            q_.pop();
            lock.unlock();
            op();
            lock.lock();
        }
    } while (!quit_);
}
