#pragma once

#include <memory>
#include <stdexcept>

/**
 * Manually implemented queue using a singly linked list.  Memory
 * management is handled via std::unique_ptr to avoid leaks and follow
 * RAII principles.
 */
template <typename T>
class Queue
{
private:
    struct Node
    {
        T data;
        std::unique_ptr<Node> next;

        explicit Node(const T& value) : data(value), next(nullptr) {}
        explicit Node(T&& value) : data(std::move(value)), next(nullptr) {}
    };

    std::unique_ptr<Node> _head;
    Node* _tail{nullptr};
    size_t _size{0};

public:
    Queue() = default;
    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;
    Queue(Queue&&) noexcept = default;
    Queue& operator=(Queue&&) noexcept = default;
    ~Queue() = default;

    bool IsEmpty() const { return _size == 0; }

    void Push(const T& value)
    {
        auto newNode = std::make_unique<Node>(value);
        Node* newTail = newNode.get();
        if (_tail)
        {
            _tail->next = std::move(newNode);
        }
        else
        {
            _head = std::move(newNode);
        }
        _tail = newTail;
        ++_size;
    }

    void Push(T&& value)
    {
        auto newNode = std::make_unique<Node>(std::move(value));
        Node* newTail = newNode.get();
        if (_tail)
        {
            _tail->next = std::move(newNode);
        }
        else
        {
            _head = std::move(newNode);
        }
        _tail = newTail;
        ++_size;
    }

    void Pop()
    {
        if (IsEmpty())
            throw std::runtime_error("Queue is empty");
        _head = std::move(_head->next);
        if (!_head)
            _tail = nullptr;
        --_size;
    }

    T& Front()
    {
        if (IsEmpty())
            throw std::runtime_error("Queue is empty");
        return _head->data;
    }

    const T& Front() const
    {
        if (IsEmpty())
            throw std::runtime_error("Queue is empty");
        return _head->data;
    }

    size_t Size() const { return _size; }
};
