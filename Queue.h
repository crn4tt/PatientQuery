template <typename T>
class Queue
{
private:

    template <typename T>
    struct Node{
        T Data;
        Node* Next;
        
        Node(T data){
            Data = data;
            Next = nullptr;
        }
    };

    Node<T>* _head;
    Node<T>* _tail;
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
            _head = new Node<T>(data);
            _tail = _head;
            
        } else {
            _tail->Next = new Node<T>(data);
            _tail = _tail->Next;
        }
        _size++;
    }


    void Pop() {
        if (IsEmpty()) throw "Queue is empty";

        Node<T>* tmp = _head;
        delete _head;
        _head = tmp->Next;
        delete tmp;        
        _size -= 1;

        if (!_head) _tail = nullptr;

        return;
    };

    T Front(){
        return _head->Data;
    }

    ~Queue(){
        Node<T>* tmp = _head;
        Node<T>* cur = _head->Next;
        for (size_t i = 0; i < _size; i++) {
            delete tmp;
            if (cur->Next == nullptr) break;
            tmp = cur;
            cur = cur->Next;
        }
        delete cur;
    }


};
