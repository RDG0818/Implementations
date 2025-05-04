/*
Basic implementation of an Adelson-Velsky Landis (AVL) Tree. Commonly used in sets.

Asymptotic Complexity:
Insertion: O(log n)
Deletion: O(log n)
Search: O(log n)
Space: O(n)

*/

#include <iostream>
#include <algorithm> // For std::max and std::min

using namespace std;

template <typename T> class Node {
public:
    Node<T>* parent;
    Node<T>* right;
    Node<T>* left;
    T value;
    int balance_factor;
    Node<T>(T value = T{}, Node<T>* parent = nullptr, Node<T>* right = nullptr, Node<T>* left = nullptr);
};

template <typename T> Node<T>::Node(T value, Node<T>* parent, Node<T>* right, Node<T>* left) {
    this->value = value;
    this->parent = parent;
    this->right = right;
    this->left = left;
    this->balance_factor = 0; 
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
    void print(Node<T>* root = nullptr);
    ~AVL();

private:
    void rotateLeft(Node<T>* x);
    void rotateRight(Node<T>* y);
};

// AVL Constructor
template <typename T> AVL<T>::AVL(Node<T>* root) {
    this->root = root;
    if (root != nullptr) _size++;
}

// Copy Constructor 
template <typename T> AVL<T>::AVL(const AVL<T>& other) {
    _size = other._size;
    auto copyTree = [&](auto& self, Node<T>* node, Node<T>* parent) -> Node<T>* {
        if (!node)
            return nullptr;
        Node<T>* newNode = new Node<T>(node->value, parent, nullptr, nullptr);
        newNode->left = self(self, node->left, newNode);
        newNode->right = self(self, node->right, newNode);
        newNode->balance_factor = node->balance_factor; 
        return newNode;
    };
    root = copyTree(copyTree, other.root, nullptr);
}

// Assignment Operator
template <typename T> AVL<T>& AVL<T>::operator=(const AVL<T>& other) {
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
            newNode->balance_factor = node->balance_factor; 
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
    Node<T>* newNode = new Node<T>(value);
    if (!_size) {
        root = newNode;
    } else {
        Node<T>* currNode = root;
        while (currNode != nullptr) {
            parent = currNode;
            if (currNode->value > value) {
                currNode = currNode->left;
            } else {
                currNode = currNode->right;
            }
        }
        newNode->parent = parent;
        if (parent->value > value) {
            parent->left = newNode;
        } else {
            parent->right = newNode;
        }
    }
    _size++;

    // Update balance factors and perform rotations
    Node<T>* current = newNode;
    while (current->parent != nullptr) {
        Node<T>* parent = current->parent;
        if (current == parent->left) {
            parent->balance_factor += 1;
        } else {
            parent->balance_factor -= 1;
        }

        if (parent->balance_factor == 2 || parent->balance_factor == -2) {
            if (parent->balance_factor == 2) {
                if (parent->left->balance_factor == 1) { // LL case
                    rotateRight(parent);
                    parent->balance_factor = 0;
                    parent->parent->balance_factor = 0;
                } 
                else { // LR case
                    Node<T>* x = parent->left;
                    Node<T>* z = x->right;
                    int z_bf = z->balance_factor;

                    rotateLeft(x);
                    rotateRight(parent);

                    if (z_bf == 1) {
                        parent->balance_factor = -1;
                        x->balance_factor = 0;
                    } else if (z_bf == -1) {
                        x->balance_factor = 1;
                        parent->balance_factor = 0;
                    } else {
                        x->balance_factor = 0;
                        parent->balance_factor = 0;
                    }
                    z->balance_factor = 0;
                }
            } 
            else { // balance_factor == -2
                if (parent->right->balance_factor == -1) { // RR case
                    rotateLeft(parent);
                    parent->balance_factor = 0;
                    parent->parent->balance_factor = 0;
                } else { // RL case
                    Node<T>* x = parent->right;
                    Node<T>* z = x->left;
                    int z_bf = z->balance_factor;

                    rotateRight(x);
                    rotateLeft(parent);

                    if (z_bf == -1) {
                        parent->balance_factor = 1;
                        x->balance_factor = 0;
                    } else if (z_bf == 1) {
                        x->balance_factor = -1;
                        parent->balance_factor = 0;
                    } else {
                        x->balance_factor = 0;
                        parent->balance_factor = 0;
                    }
                    z->balance_factor = 0;
                }
            }
            break; 
        } else if (parent->balance_factor == 0) {
            break;
        } else {
            current = parent; 
        }
    }
}

// Count Function
template <typename T> bool AVL<T>::count(T value) {
    Node<T>* currNode = root;
    while (currNode != nullptr) {
        if (currNode->value == value) {
            return true;
        }
        if (currNode->value > value) {
            currNode = currNode->left;
        } else {
            currNode = currNode->right;
        }
    }
    return false;
}

// Print Function
template <typename T> void AVL<T>::print(Node<T>* node) {
    if (_size == 0) return;
    if (node == nullptr) {
        node = root;
    }
    if (node->left != nullptr) {
        print(node->left);
    }
    cout << node->value << " ";
    if (node->right != nullptr) {
        print(node->right);
    }
}

/*=========================================================================================

Before Right Rotation (on y) -----> Right Rotate(y) ----> After Right Rotation (on y)
After Left Rotation (on x)   <----- Left Rotate(x) <----- Before Left Rotation (on x)

                         (y)                            (x)
                        /   \                          /   \
                      (x)   T3                        T1   (y)
                     /   \                                /   \
                    T1   T2                              T2   T3
                    
==========================================================================================*/


template <typename T> void AVL<T>::rotateLeft(Node<T>* x) {
    Node<T>* y = x->right;
    Node<T>* T2 = y->left;

    y->left = x;
    x->right = T2;

    y->parent = x->parent;
    x->parent = y;
    if (T2 != nullptr) {
        T2->parent = x;
    }

    if (y->parent != nullptr) {
        if (y->parent->left == x) {
            y->parent->left = y;
        } else {
            y->parent->right = y;
        }
    } else {
        root = y;
    }
}

template <typename T> void AVL<T>::rotateRight(Node<T>* y) {
    Node<T>* x = y->left;
    Node<T>* T2 = x->right;

    x->right = y;
    y->left = T2;

    x->parent = y->parent;
    y->parent = x;
    if (T2 != nullptr) {
        T2->parent = y;
    }

    if (x->parent != nullptr) {
        if (x->parent->left == y) {
            x->parent->left = x;
        } else {
            x->parent->right = x;
        }
    } else {
        root = x;
    }
}

// Destructor
template <typename T> AVL<T>::~AVL() {
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
    cout << "Test Case 1:" << endl;
    cout << "Expected Output: [0, 1, 2, 3, 4]" << endl;
    AVL<int> avl1;
    avl1.insert(1);
    avl1.insert(0);
    avl1.insert(4);
    avl1.insert(2);
    avl1.insert(3);
    avl1.print();
    cout << endl << "Data Structure Size: " << avl1.size() << endl;

    cout << "Test Case 2:" << endl;
    cout << "Expected Output: ['a', 'b', 'c', 'd', 'e']" << endl;
    AVL<char> avl2;
    avl2.insert('a');
    avl2.insert('d');
    avl2.insert('e');
    avl2.insert('b');
    avl2.insert('c');
    avl2.print();
    cout << endl << "Data Structure Size: " << avl2.size() << endl;

    cout << "Test Case 3:" << endl;
    cout << "Expected Output: [.2, 2.9, 3.8, 5.1, 6.4]" << endl;
    AVL<float> avl3;
    avl3.insert(.2);
    avl3.insert(3.8);
    avl3.insert(2.9);
    avl3.insert(6.4);
    avl3.insert(5.1);
    avl3.print();
    cout << endl << "Data Structure Size: " << avl3.size() << endl;
    return 0;
}
