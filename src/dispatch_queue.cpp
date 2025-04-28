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

void DispatchQueue::dispatch(Task&& op)
{
    std::unique_lock<std::mutex> lock(lock_);
    task_queue_.push(std::move(op));
    lock.unlock();
    cv_.notify_one();
}

void DispatchQueue::dispatch_thread_handler()
{
    std::unique_lock<std::mutex> lock(lock_);
    do {
        cv_.wait(lock, [this]{
            return (!task_queue_.empty() || quit_);
        });

        while (!quit_ && !task_queue_.empty())
        {
            auto op = std::move(task_queue_.front());
            task_queue_.pop();
            lock.unlock();
            op();
            lock.lock();
        }
    } while (!quit_);
}
