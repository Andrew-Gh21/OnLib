#pragma once

#include <mutex>
#include <deque>

namespace net
{
#pragma region Class definition

	template <typename T>
	/// <summary>
	/// Thread safe queue
	/// </summary>
	/// <typeparam name="T">Data type</typeparam>
	class BlockingQueue
	{
	public:
		BlockingQueue() = default;
		BlockingQueue(const BlockingQueue<T>& other) = delete;
		virtual ~BlockingQueue() { Clear(); }

		const T& Front() const;
		const T& Back() const;
		T PopFront();
		T PopBack();
		void PushFront(const T& item);
		void PushBack(const T& item);
		bool Empty() const;
		std::size_t Size() const;
		void Clear();
		void Wait();

	protected:
		std::mutex muxQueue;
		std::deque<T> deque;
		std::condition_variable cvBlocking;
		std::mutex muxBlocking;
	};

#pragma endregion
}

#pragma region Methods implementation

template<typename T>
inline const T& net::BlockingQueue<T>::Front() const
{
	std::scoped_lock lock(muxQueue);
	return deque.front();
}

template<typename T>
inline const T& net::BlockingQueue<T>::Back() const
{
	std::scoped_lock lock(muxQueue);
	return deque.back();
}

template<typename T>
inline T net::BlockingQueue<T>::PopFront()
{
	std::scoped_lock lock(muxQueue);
	auto val = std::move(deque.front());
	deque.pop_front();

	return val;
}

template<typename T>
inline T net::BlockingQueue<T>::PopBack()
{
	std::scoped_lock lock(muxQueue);
	auto val = std::move(deque.back());
	deque.pop_back();

	return val;
}

template<typename T>
inline void net::BlockingQueue<T>::PushFront(const T& item)
{
	std::scoped_lock lock(muxQueue);
	deque.emplace_front(std::move(item));

	std::unique_lock<std::mutex> ul(muxBlocking);
	cvBlocking.notify_one();
}

template<typename T>
inline void net::BlockingQueue<T>::PushBack(const T& item)
{
	std::scoped_lock lock(muxQueue);
	deque.emplace_back(std::move(item));

	std::unique_lock<std::mutex> ul(muxBlocking);
	cvBlocking.notify_one();
}

template<typename T>
inline bool net::BlockingQueue<T>::Empty() const
{
	std::scoped_lock lock(muxQueue);
	return deque.empty();
}

template<typename T>
inline std::size_t net::BlockingQueue<T>::Size() const
{
	std::scoped_lock lock(muxQueue);
	return deque.size();
}

template<typename T>
inline void net::BlockingQueue<T>::Clear()
{
	std::scoped_lock lock(muxQueue);
	deque.clear();
}

template<typename T>
inline void net::BlockingQueue<T>::Wait()
{
	while (Empty())
	{
		std::unique_lock<std::mutex> ul(muxBlocking);
		cvBlocking.wait(ul);
	}
}

#pragma endregion