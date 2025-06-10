#pragma once

template <typename T>
struct Node {
    T data;
    Node<T>* next = nullptr;
    explicit Node(const T& d) : data(d), next(nullptr) {}
};
