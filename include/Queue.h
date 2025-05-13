#pragma once

template <typename T>
class Queue
{
private:

    struct Node{
        T Data;
        Node* Next;
        
        Node(T data){
            Data = data;
            Next = nullptr;
        }
    };

    Node* _head;
    Node* _tail;
    size_t _size;

public:
    Queue() {
        _head = nullptr;
        _tail = nullptr;
        _size = 0;
    }

    
    bool IsEmpty(){
        return (_size == 0);
    }

    void Push(T data) {
        if (_head == nullptr) {
            _head = new Node(data);
            _tail = _head;
            
        } else {
            _tail->Next = new Node(data);
            _tail = _tail->Next;
        }
        _size++;
    }


    
    void Pop() {
        if (IsEmpty()) throw "Queue is empty";
    
        Node* tmp = _head;     
        _head = _head->Next;     
        delete tmp;            
        _size--;
    
        if (_head == nullptr) {  
            _tail = nullptr;
        }
    }
    
    ~Queue() {
        while (!IsEmpty()) {
            Pop(); 
        }
    }
    
    T Front(){
        return _head->Data;
    }

 
    
    
};
