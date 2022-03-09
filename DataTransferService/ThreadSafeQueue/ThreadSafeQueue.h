#ifndef THREAD_SAFE_QUEUE_H_
#define THREAD_SAFE_QUEUE_H_

#include <queue>
#include <optional>
#include <shared_mutex>

template<typename T>
class ThreadSafeQueue 
{
private:
    std::queue<T> queue_;
    mutable std::shared_mutex mutex_;

    // Moved out of public interface to prevent races between this
    // and pop().
    bool empty() const {
        return queue_.empty();
    }

public:
    ThreadSafeQueue() = default;
    ThreadSafeQueue(const ThreadSafeQueue<T>&) = delete;
    ThreadSafeQueue& operator=(const ThreadSafeQueue<T>&) = delete;

    ThreadSafeQueue(ThreadSafeQueue<T>&& other) {
        std::unique_lock<std::shared_mutex> w_lock(mutex_);
        queue_ = std::move(other.queue_);
    }

    virtual ~ThreadSafeQueue() { }

    size_t size() const {
        std::shared_lock<std::shared_mutex> r_lock(mutex_);
        return queue_.size();
    }

    std::optional<T> front() {
        std::shared_lock<std::shared_mutex> r_lock(mutex_);
        if (queue_.empty()) {
            return {};
        }
        T tmp = queue_.front();
        return tmp;
    }

    std::optional<T> pop() {
        std::unique_lock<std::shared_mutex> w_lock(mutex_);
        if (queue_.empty()) {
            return {};
        }
        T tmp = queue_.front();
        queue_.pop();
        return tmp;
    }

    void push(const T& item) {
        std::unique_lock<std::shared_mutex> w_lock(mutex_);
        queue_.push(item);
    }
};

#endif //!THREAD_SAFE_QUEUE_H_