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
        _size = arr_size;
    }
}

// Copy Constructor
template <typename T> LinkedList<T>::LinkedList(const LinkedList<T>& other) {
    head = tail = new Node<T>(); // Create a new dummy node
    Node<T>* currNode = head;
    Node<T>* otherNode = other.head->next; // Skip dummy node

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

    head = tail = new Node<T>(); // New dummy node
    currNode = head;
    Node<T>* otherNode = other.head->next; 

    while (otherNode != nullptr) {
        Node<T>* nextNode = new Node<T>(otherNode->value, currNode);
        currNode->next = nextNode;
        tail = currNode = nextNode;
        otherNode = otherNode->next;
    }

    return *this; // Return reference to self
}

template <typename T> int LinkedList<T>::size() {
    return _size;
}

// Print Function
template <typename T> void LinkedList<T>::print() {
    Node<T>* currNode = head->next; // Ignoring dummy node
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
    int arr[5] = {5, 4, 3, 2, 1};
    LinkedList<int> ll(arr, 5);
    ll.print();
    cout << ll.size() << endl;
    return 0;
}
