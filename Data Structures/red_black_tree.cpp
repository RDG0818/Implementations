/*
Root and leaves are always black.
If a node is red, then its children are black.
All paths from a node to its NIL leaves contain the same number of black nodes.

Basic implementation of a Red Black Tree in C++.

Asymptotic Complexity:
Insertion: O(log n)
Deletion: O(log n)
Search: O(log n)
Space: O(n)
*/
using namespace std;

template <typename T> class Node {
    public:
        Node<T>* parent;
        Node<T>* right;
        Node<T>* left;
        T value;
        bool color; // 1 is black, 0 is red
        Node<T>(T value = T{}, Node<T>* parent = nullptr, Node<T>* right = nullptr, Node<T>* left = nullptr, bool color = 1);
    };
    
template <typename T> Node<T>::Node(T value, Node<T>* parent, Node<T>* right, Node<T>* left, bool color) {
        this->value = value;
        this->parent = parent;
        this->right = right;
        this->left = left; 
        this->color = color;
    }

template <typename T> class RedBlack {
public:
    Node<T>* root;
    int _size = 0;
    RedBlack<T>(Node<T>* root = nullptr);
    RedBlack(const RedBlack<T>& other);
    RedBlack& operator=(const RedBlack<T>& other);
    int size();
    void insert(T value);
    bool count(T value);
    void print(Node<T>* root = nullptr);
    ~RedBlack();
private:
    void rotateLeft(Node<T>* x);
    void rotateRight(Node<T>* y);

};

// Red Black Constructor
template <typename T> RedBlack<T>::RedBlack(Node<T>* root) {
    this->root = root;
    if (root != nullptr) _size++;
}

// Copy Constructor
template <typename T> RedBlack<T>::RedBlack(const RedBlack<T>& other) {

}

// Assignment Operator
template <typename T> RedBlack<T>& RedBlack<T>::operator=(const RedBlack<T>& other) {

}

// Size Function
template <typename T> int RedBlack<T>::size() {
    return _size;
}

// Insert Function
template <typename T> void RedBlack<T>::insert(T value) {

}

// Count Function
template <typename T> bool RedBlack<T>::count(T value) {
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
        return false;
    }
}

// Print Function
template <typename T> void RedBlack<T>::print(Node<T>* node) {
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

template <typename T> void RedBlack<T>::rotateLeft(Node<T>* x) {

}

template <typename T> void RedBlack<T>:: rotateRight(Node<T>* y) {

}

// Destructor
template <typename T> RedBlack<T>::~RedBlack() {
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
    return 0;
}