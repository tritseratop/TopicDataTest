#ifndef THREAD_SAFE_QUEUE_H_
#define THREAD_SAFE_QUEUE_H_

#include <deque>
#include <optional>
#include <shared_mutex>

#include "../DdsCommonClasses.h"

template <typename T>
class ThreadSafeDeque
{
private:
	std::deque<T> queue_;
	mutable std::shared_mutex mutex_;

public:
	ThreadSafeDeque() = default;
	ThreadSafeDeque(const ThreadSafeDeque<T>&) = delete;
	ThreadSafeDeque& operator=(const ThreadSafeDeque<T>&) = delete;

	ThreadSafeDeque(ThreadSafeDeque<T>&& other)
	{
		std::unique_lock<std::shared_mutex> w_lock(mutex_);
		queue_ = std::move(other.queue_);
	}

	virtual ~ThreadSafeDeque() { }

	size_t size() const
	{
		std::shared_lock<std::shared_mutex> r_lock(mutex_);
		return queue_.size();
	}

	size_t empty() const
	{
		std::shared_lock<std::shared_mutex> r_lock(mutex_);
		return queue_.empty();
	}

	std::optional<T> front() const
	{
		std::shared_lock<std::shared_mutex> r_lock(mutex_);
		if (queue_.empty())
		{
			return {};
		}
		T tmp = queue_.front();
		return tmp;
	}

	std::optional<T> back() const
	{
		std::shared_lock<std::shared_mutex> r_lock(mutex_);
		if (queue_.empty())
		{
			return {};
		}
		T tmp = queue_.back();
		return tmp;
	}

	std::optional<T> pop_front()
	{
		std::unique_lock<std::shared_mutex> w_lock(mutex_);
		if (queue_.empty())
		{
			return {};
		}
		T tmp = queue_.front();
		queue_.pop_front();
		return tmp;
	}

	void push_back(const T& item)
	{
		std::unique_lock<std::shared_mutex> w_lock(mutex_);
		queue_.push_back(item);
	}

	void clear()
	{
		std::unique_lock<std::shared_mutex> w_lock(mutex_);
		queue_.clear();
	}

	std::deque<T> getDequeCopy() const
	{
		return queue_;
	}
};

#endif //!THREAD_SAFE_QUEUE_H_