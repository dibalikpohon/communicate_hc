#include <queue>

#include <mutex>
#include <condition_variable>

#ifndef SAFE_QUEUE_SAFE_QUEUE_HPP
#define SAFE_QUEUE_SAFE_QUEUE_HPP

template <class T>
class safe_queue
{
public:
    // default constructor
    safe_queue() = default;

    // no copy-constructor
    safe_queue(safe_queue<T>&) = delete;

    // no copy-assignment operator
    safe_queue<T>& operator=(safe_queue<T>&) = delete;

    void push(T& value)
    {
        std::lock_guard<std::mutex> lock_guard(queue_mutex);
        queue.push(value);
        queue_cv.notify_one();
    }

    T& front()
    {
        std::unique_lock<std::mutex> unique_lock(queue_mutex);
        queue_cv.wait(unique_lock, [this]{return !empty();});
        return queue.front();
    }

    void pop()
    {
        std::lock_guard<std::mutex> lock_guard(queue_mutex);
        if (size()) {
            queue.pop();
        }
    }

    std::size_t size() const
    {
        return queue.size();
    }

    bool empty() const
    {
        return queue.empty();
    }

private:
    std::mutex queue_mutex;
    std::condition_variable queue_cv;

    std::queue<T> queue;
};

#endif //SAFE_QUEUE_SAFE_QUEUE_HPP
