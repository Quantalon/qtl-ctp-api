#ifndef DISPATCH_QUEUE_H
#define DISPATCH_QUEUE_H

#include <functional>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>


class DispatchQueue {
    using Task = std::function<void()>;

public:
    explicit DispatchQueue();
    ~DispatchQueue();

    void dispatch(Task&& op);

    DispatchQueue(const DispatchQueue& rhs) = delete;
    DispatchQueue(DispatchQueue&& rhs) = delete;
    DispatchQueue& operator=(const DispatchQueue& rhs) = delete;
    DispatchQueue& operator=(DispatchQueue&& rhs) = delete;

private:
    std::mutex lock_;
    std::thread thread_;
    std::queue<Task> task_queue_;
    std::condition_variable cv_;
    bool quit_ = false;

    void dispatch_thread_handler();
};

#endif //DISPATCH_QUEUE_H
