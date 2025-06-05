#pragma once

#include <queue>
#include <stdexcept>

/**
 * Simple wrapper around std::queue providing the minimal interface
 * required by the application. Using the STL container avoids manual
 * memory management and follows RAII principles.
 */
namespace clinic {

template <typename T>
class Queue
{
private:
    std::queue<T> _data;

public:
    Queue() = default;

    bool IsEmpty() const { return _data.empty(); }

    void Push(const T& value) { _data.push(value); }

    void Pop()
    {
        if (IsEmpty())
            throw std::runtime_error("Queue is empty");
        _data.pop();
    }

    T& Front() { return _data.front(); }
    const T& Front() const { return _data.front(); }

    size_t Size() const { return _data.size(); }

};

} // namespace clinic
