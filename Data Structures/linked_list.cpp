/*
Basic implementation of a linked list in C++. It can additionally be used as a LIFO stack or FIFO queue.

Asymptotic Complexity:
Random Access: O(n)
Insertion (end or front): O(1)
Insertion (middle): O(n)
Deletion (end or front): O(1)
Deletion (middle): O(n)
Search: O(n)
Space: O(n)

*/

#include <iostream>

using namespace std;

template <typename T> class Node {    
    public:
    Node<T>* prev;
    Node<T>* next;
    T value;
    Node<T>(T value = T{}, Node<T>* prev = nullptr, Node<T>* next = nullptr);
};

// Node Constructor
template <typename T> Node<T>::Node(T value, Node<T>* prev, Node<T>* next) {
    this->value = value;
    this->prev = prev;
    this->next = next;
}

template <typename T> class LinkedList {
    private:
    int _size;
    Node<T>* head;
    Node<T>* tail;

    public:
    LinkedList(T* value_arr = nullptr, int arr_size = 0);
    LinkedList(const LinkedList<T>& other);
    LinkedList& operator=(const LinkedList<T>& other);
    int size();
    T& at(int index);
    void push_front(T value);
    void push_back(T value);
    void insert(int index, T value);
    T pop_front();
    T pop_back();
    T remove(int index);
    void print();
    ~LinkedList();
};

// Linked List Constructor
template <typename T> LinkedList<T>::LinkedList(T* value_arr, int arr_size) {
    head = tail = new Node<T>(); // Dummy Node
    Node<T>* currNode = head;
    if (value_arr != nullptr) {
        for (int i = 0; i < arr_size; i++) {
            Node<T>* nextNode = new Node<T>(value_arr[i], currNode);
            currNode->next = nextNode;
            tail = currNode = nextNode;
        }     
    }
    _size = arr_size;
}

// Copy Constructor
template <typename T> LinkedList<T>::LinkedList(const LinkedList<T>& other) {
    head = tail = new Node<T>();
    Node<T>* currNode = head;
    Node<T>* otherNode = other.head->next; 

    while (otherNode != nullptr) {
        Node<T>* nextNode = new Node<T>(otherNode->value, currNode);
        currNode->next = nextNode;
        tail = currNode = nextNode;
        otherNode = otherNode->next;
    }
}

// Assignment Operator
template <typename T> LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& other) {
    if (this == &other) {
        return *this; 
    }

    Node<T>* currNode = head;
    while (currNode != nullptr) {
        Node<T>* nextNode = currNode->next;
        delete currNode;
        currNode = nextNode;
    }

    head = tail = new Node<T>(); 
    currNode = head;
    Node<T>* otherNode = other.head->next; 

    while (otherNode != nullptr) {
        Node<T>* nextNode = new Node<T>(otherNode->value, currNode);
        currNode->next = nextNode;
        tail = currNode = nextNode;
        otherNode = otherNode->next;
    }

    return *this; 
}

// Size Function
template <typename T> int LinkedList<T>::size() {
    return _size;
}

// At Function
template <typename T> T& LinkedList<T>::at(int index) {
    if (index < 0 || index >= _size) {
        throw out_of_range("Index out of bounds");
    }
    int currNodeIndex = 0;
    Node<T>* currNode = head->next;
    for (int i = 0; i < index; ++i) {
        currNode = currNode->next;
    }
    return currNode->value;
}

// Push Front Function
template <typename T> void LinkedList<T>::push_front(T value) {
    Node<T>* currNode = new Node<T>(value, head);
    if (head->next == nullptr) {
        head->next = currNode;
        tail = currNode;
    }
    else {
        currNode->next = head->next;
        head->next->prev = currNode;
        head->next = currNode;
    }
    _size++;
}

// Push Back Function
template <typename T> void LinkedList<T>::push_back(T value) {
    Node<T>* currNode = new Node<T>(value, tail);
    tail = currNode;
    currNode->prev->next = currNode;
    if (head->next == nullptr) {
        head->next = currNode;        
    }
    _size++;
}

// Insert Function
template <typename T> void LinkedList<T>::insert(int index, T value) {
    if (index < 0 || index > _size) {
        throw out_of_range("Index out of bounds");
    }
    if (index == 0) push_front(value);
    else if (index == _size) push_back(value);
    else {

        Node<T>* currNode = head->next;
        int nodeIndex = 0;
        while (nodeIndex < index) {
            currNode = currNode->next;
            nodeIndex++;
        }
        Node<T>* newNode = new Node<T>(value, currNode->prev, currNode);
        currNode->prev->next = newNode;
        currNode->prev = newNode;
    }
    _size++;
}

// Pop Front Function
template <typename T> T LinkedList<T>::pop_front() {
    if (head->next == nullptr) {
        throw std::out_of_range("List is empty. Cannot pop from an empty list.");
    }
    Node<T>* currNode = head->next;
    T value = currNode->value;
    head->next = currNode->next;
    if (head->next != nullptr) {
        head->next->prev = head;
    }
    else {
        tail = head; 
    }
    delete currNode;
    _size--;
    return value;
}

// Pop Back Function
template <typename T> T LinkedList<T>::pop_back() {
    if (head->next == nullptr) {
        throw std::out_of_range("List is empty. Cannot pop from an empty list.");
    }
    Node<T>* currNode = tail;
    tail = tail->prev;
    T value = currNode->value;
    tail->next = nullptr;
    delete currNode;
    _size--;
    return value;
}

// Remove Function
template <typename T> T LinkedList<T>::remove(int index) {
    if (head->next == nullptr) {
        throw std::out_of_range("List is empty. Cannot remove from an empty list.");
    }
    if (index < 0 || index >= _size) {
        throw out_of_range("Index out of bounds");
    }
    T value;
    if (index == 0) value = pop_front();
    else if (index == _size - 1) value = pop_back();
    else {
        Node<T>* currNode = head->next;
        int nodeIndex = 0;
        while (nodeIndex < index) {
            currNode = currNode->next;
            nodeIndex++;
        }
        value = currNode->value;
        currNode->next->prev = currNode->prev;
        currNode->prev->next = currNode->next;
        delete currNode;
    }
    _size--;
    return value;
}

// Print Function
template <typename T> void LinkedList<T>::print() {
    Node<T>* currNode = head->next; 
    while (currNode != nullptr) {
        cout << currNode->value << " ";
        currNode = currNode->next;
    }
    cout << endl;
}

// Destructor
template <typename T> 
LinkedList<T>::~LinkedList() {
    Node<T>* currNode = head;
    while (currNode != nullptr) {
        Node<T>* nextNode = currNode->next; 
        delete currNode; 
        currNode = nextNode; 
    }
}


int main() {
    cout << "Test Case 1:" << endl;
    cout << "Expected Output: [0, 1, 2, 3, 4]" << endl;
    LinkedList<int> ll;
    ll.push_back(1);
    ll.push_front(0);
    ll.push_back(4);
    ll.insert(2, 2);
    ll.insert(3, 3);
    ll.insert(1, 10);
    ll.remove(1);
    ll.print();
    cout << "Data Structure Size: " << ll.size() << endl;

    cout << "Test Case 2:" << endl;
    cout << "Expected Output: ['a', 'b', 'c', 'd', 'e']" << endl;
    char arr[3]; for (char c : arr) c = 'f';
    LinkedList<char> stack(arr, 3);
    stack.remove(1);
    stack.pop_back();
    stack.pop_back();
    stack.push_back('a');
    stack.push_back('b');
    stack.push_back('c');
    stack.push_back('d');
    stack.push_back('e');
    stack.print();
    cout << "Data Structure Size: " << stack.size() << endl;

    cout << "Test Case 3:" << endl;
    cout << "Expected Output: [.2, 3.8, 2.9, 5.1, 6.4]" << endl;
    LinkedList<float> queue;
    queue.push_back(2);
    queue.pop_front();
    queue.push_front(.2);
    queue.push_back(3.8);
    queue.push_back(2.9);
    queue.push_back(5.1);
    queue.push_back(6.4);
    queue.print();
    cout << "Data Structure Size: " << queue.size() << endl;

    return 0;
}

