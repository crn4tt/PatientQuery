#pragma once
#include <cstddef>
#include <stdexcept>
#include "Node.hpp"

template <typename T>
class Queue {
private:
    Node<T>* head = nullptr;
    Node<T>* tail = nullptr;
    std::size_t size = 0;
public:
    Queue() = default;
    ~Queue();

    void push(const T& elem);
    void pop();
    T& front() const;
    bool isEmpty() const { return size == 0; }
    bool isFull() const { return false; }
};

template <typename T>
Queue<T>::~Queue() {
    while (!isEmpty()) {
        pop();
    }
}

template <typename T>
void Queue<T>::push(const T& elem) {
    Node<T>* node = new Node<T>(elem);
    if (tail) {
        tail->next = node;
    } else {
        head = node;
    }
    tail = node;
    ++size;
}

template <typename T>
void Queue<T>::pop() {
    if (isEmpty()) {
        throw std::runtime_error("Queue is empty");
    }
    Node<T>* temp = head;
    head = head->next;
    if (!head) {
        tail = nullptr;
    }
    delete temp;
    --size;
}

template <typename T>
T& Queue<T>::front() const {
    if (isEmpty()) {
        throw std::runtime_error("Queue is empty");
    }
    return head->data;
}

