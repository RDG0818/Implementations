#include <iostream>

using namespace std;

template <typename T> class Node {
    public:
    Node<T>* parent;
    Node<T>* right;
    Node<T>* left;
    T value;
    int height = 0;
    Node<T>(T value=T{}, Node<T>* parent=nullptr, Node<T>* right=nullptr, Node<T>* left=nullptr);   
};

template <typename T> Node<T>::Node(T value, Node<T>* parent, Node<T>* right, Node<T>* left) {
    this->value = value;
    this->parent = parent;
    this->right = right;
    this->left = left;
}

template <typename T> class AVL {
    public:
    Node<T>* root;
    int _size = 0;
    AVL<T>(Node<T>* root = nullptr);
    AVL(const AVL<T>& other);
    AVL& operator=(const AVL<T>& other);
    int size();
    void insert(T value);
    bool count(T value);
    T remove(T value);
    void print(Node<T>* root = nullptr);
    ~AVL();
};

// AVL Constructor
template <typename T> AVL<T>::AVL(Node<T>* root) {
    this->root = root;
    if (root != nullptr) _size++;
}

// Copy Constructor
template <typename T>
AVL<T>::AVL(const AVL<T>& other) {
    _size = other._size;
    auto copyTree = [&](auto& self, Node<T>* node, Node<T>* parent) -> Node<T>* {
        if (!node)
            return nullptr;
        Node<T>* newNode = new Node<T>(node->value, parent, nullptr, nullptr);
        newNode->left = self(self, node->left, newNode);
        newNode->right = self(self, node->right, newNode);
        return newNode;
    };
    root = copyTree(copyTree, other.root, nullptr);
}

// Assignment Operator Overload
template <typename T>
AVL<T>& AVL<T>::operator=(const AVL<T>& other) {
    if (this != &other) {
        auto deleteTree = [&](auto& self, Node<T>* node) -> void {
            if (!node)
                return;
            self(self, node->left);
            self(self, node->right);
            delete node;
        };
        deleteTree(deleteTree, root);
        _size = other._size;
        auto copyTree = [&](auto& self, Node<T>* node, Node<T>* parent) -> Node<T>* {
            if (!node)
                return nullptr;
            Node<T>* newNode = new Node<T>(node->value, parent, nullptr, nullptr);
            newNode->left = self(self, node->left, newNode);
            newNode->right = self(self, node->right, newNode);
            return newNode;
        };
        root = copyTree(copyTree, other.root, nullptr);
    }
    return *this;
}


// Size Function
template <typename T> int AVL<T>::size() {
    return _size;
}

// Insert Function
template <typename T> void AVL<T>::insert(T value) {
    Node<T>* parent = nullptr;
    Node<T>* newNode = new Node<T>(value, parent);
    if (!_size) root = newNode;
    else {
        Node<T>* currNode = root;
        while (currNode != nullptr) {
            parent = currNode;
            currNode->height++;
            if (currNode->value > value) currNode = currNode->left;
            else currNode = currNode->right;
        }
        if (parent->value > value) parent->left = newNode;
        else parent->right = newNode;
    }
   _size++;
    
    //TODO: Add the AVL rotations
}

// Count Function
template <typename T> bool AVL<T>::count(T value) {
    Node<T>* currNode = root;
    while (currNode != nullptr) {
        if (currNode->value == value) {
            return true;
        }
        if (currNode->value > value) currNode = currNode->left;
        else currNode = currNode->right;
    }
    return false;
}

// Print Function
template <typename T> void AVL<T>::print(Node<T>* root) {
    bool flag = false;
    if (_size == 0) return;
    if (root == nullptr) root = this->root; 
    if (root->left != nullptr) print(root->left);
    else {
        flag = true;
        cout << root->value << " ";
    } 
    if (root->right != nullptr) print(root->right);
    else if (!flag) cout << root->value << " "; 
    
}

// Destructor
template <typename T>
AVL<T>::~AVL() {
    auto deleteTree = [&](auto& self, Node<T>* node) -> void {
        if (!node)
            return;
        self(self, node->left);
        self(self, node->right);
        delete node;
    };
    deleteTree(deleteTree, root);
}


int main() {
    AVL<int> avl;
    avl.insert(3);
    avl.insert(5);
    avl.insert(4);
    avl.print();
    return 0;
}
