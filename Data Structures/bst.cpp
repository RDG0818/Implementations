#include <iostream>

using namespace std;

template <typename T> class Node {
    public:
    Node<T>* right;
    Node<T>* left;
    T value;
    Node<T>(T value=T{}, Node<T>* right=nullptr, Node<T>* left=nullptr);   
};

template <typename T> Node<T>::Node(T value, Node<T>* right, Node<T>* left) {
    this->value = value;
    this->right = right;
    this->left = left;
}

template <typename T> class BST {
    public:
    Node<T>* root;
    BST<T>(Node<T>* root = nullptr);
};

template <typename T> BST<T>::BST(Node<T>* root) {
    this->root = root;
}

int main() {

    return 0;
}