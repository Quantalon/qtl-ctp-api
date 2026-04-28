# DispatchQueue 任务队列

`DispatchQueue` 是项目中的核心线程安全组件，用于将 CTP 内部线程的同步回调解耦到独立的工作线程中异步执行。

## 设计背景

CTP 库在其内部线程中**同步调用** SPI 回调函数。如果回调处理逻辑（尤其是调用 Python 代码）耗时较长，会导致 CTP 内部消息队列积压，最终引发队列溢出异常。

`DispatchQueue` 的作用：
- **立即返回**: CTP 回调函数只负责将任务入队，不执行任何实际逻辑
- **异步处理**: 独立的工作线程从队列中取出任务并执行
- **线程安全**: 使用 `std::mutex` 和 `std::condition_variable` 实现同步
- **GIL 管理**: 工作线程在执行 Python 回调前获取 GIL

## 类定义

```cpp
class DispatchQueue {
    using Task = std::function<void()>;

public:
    explicit DispatchQueue();
    ~DispatchQueue();

    void dispatch(Task&& op);

    // 禁用拷贝和移动
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
```

## 构造函数

```cpp
DispatchQueue::DispatchQueue() {
    thread_ = std::thread(&DispatchQueue::dispatch_thread_handler, this);
}
```

构造时立即启动一个后台工作线程，执行 `dispatch_thread_handler()`。

## 析构函数

```cpp
DispatchQueue::~DispatchQueue() {
    std::unique_lock<std::mutex> lock(lock_);
    quit_ = true;
    lock.unlock();
    cv_.notify_all();
    if (thread_.joinable()) {
        thread_.join();
    }
}
```

析构时：
1. 设置 `quit_ = true` 通知工作线程退出
2. 唤醒条件变量
3. 等待工作线程结束（`join`）

## dispatch 方法

```cpp
void DispatchQueue::dispatch(Task&& op) {
    std::unique_lock<std::mutex> lock(lock_);
    task_queue_.push(std::move(op));
    lock.unlock();
    cv_.notify_one();
}
```

将任务加入队列并通知工作线程。使用右值引用避免 lambda 拷贝开销。

## 工作线程循环

```cpp
void DispatchQueue::dispatch_thread_handler() {
    std::unique_lock<std::mutex> lock(lock_);
    do {
        cv_.wait(lock, [this]{
            return (!task_queue_.empty() || quit_);
        });

        while (!quit_ && !task_queue_.empty()) {
            auto op = std::move(task_queue_.front());
            task_queue_.pop();
            lock.unlock();   // 解锁，允许其他线程入队
            op();            // 执行任务（可能获取 GIL 调用 Python）
            lock.lock();     // 重新加锁
        }
    } while (!quit_);
}
```

工作线程的执行逻辑：

1. **等待条件**: 当队列为空且未收到退出信号时，线程阻塞在 `cv_.wait()`
2. **批量处理**: 每次被唤醒后，尽可能多地处理队列中的任务
3. **执行时解锁**: 取出任务后先 `unlock()`，再执行。这样 CTP 回调线程可以在此期间继续入队
4. **循环检查**: 处理完当前所有任务后，再次检查是否退出

## 使用示例（MdApi 回调中）

```cpp
void MdApi::OnFrontConnected() {
    queue_->dispatch([=]() {
        nb::gil_scoped_acquire acquire;  // 获取 Python GIL
        PyOnFrontConnected();             // 调用 Python 回调
    });
}
```

```cpp
void MdApi::OnRspUserLogin(CThostFtdcRspUserLoginField *data,
                           CThostFtdcRspInfoField *error,
                           int request_id, bool is_last) {
    // 1. 在 CTP 线程中捕获数据（避免指针失效）
    std::optional<CThostFtdcRspUserLoginField> rsp_data;
    if (data) rsp_data = *data;
    std::optional<CThostFtdcRspInfoField> rsp_error;
    if (error) rsp_error = *error;

    // 2. 将任务入队
    queue_->dispatch([=]() {
        nb::gil_scoped_acquire acquire;
        nb::dict py_data;
        if (rsp_data) {
            py_data["BrokerID"] = gbk_to_utf8(rsp_data->BrokerID);
            // ... 更多字段转换
        }
        nb::dict py_error;
        if (rsp_error) {
            py_error["ErrorID"] = rsp_error->ErrorID;
            py_error["ErrorMsg"] = gbk_to_utf8(rsp_error->ErrorMsg);
        }
        PyOnRspUserLogin(py_data, py_error, request_id, is_last);
    });
}
```

## 关键要点

1. **数据必须在入队前复制**: CTP 传入的指针在回调返回后可能失效，因此使用 `std::optional` 在入队前复制数据
2. **Lambda 按值捕获**: `[=]` 确保所有数据被复制到 lambda 中，而不是引用
3. **GIL 在执行时获取**: 只在实际需要调用 Python 时才获取 GIL，减少锁竞争
4. **单线程顺序执行**: 所有回调按入队顺序依次执行，天然保证了事件顺序性
