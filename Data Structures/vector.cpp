/*
Basic implementation of a vector in C++. 

Asymptotic Complexity:
Random Access: O(1)
Insertion (end): amortized O(1), worst case O(n)
Insertion (front or middle): O(n)
Deletion (end): O(1)
Deletion (front or middle): O(n)
Search (unsorted): O(n)
Search (sorted): O(log(n))
Space: O(n)

*/

#include <iostream>

using namespace std;

template <typename T> class myVector {
    private:
    T* ptr;
    int _size;
    int capacity;

    public:
    myVector(int s = 0, T value = T{});
    myVector(const myVector& other);
    myVector& operator=(const myVector& other);
    T& operator[](int index);
    void print();
    int size();
    void push_back(T value);
    void insert(int index, T value);
    T pop_back();
    void erase(int index);
    ~myVector();
};

// Constructor
template <typename T> myVector<T>::myVector(int s, T value) {
    capacity = 2 * s;
    ptr = new T[capacity];
    _size = s;
    for (int i = 0; i < _size; i++) ptr[i] = value;
}

// Copy Constructor
template <typename T>
myVector<T>::myVector(const myVector& other) : _size(other._size) {
    ptr = new T[_size]; 
    for (int i = 0; i < _size; i++) {
        ptr[i] = other.ptr[i]; 
    }
}

// Assignment Operator
template <typename T>
myVector<T>& myVector<T>::operator=(const myVector& other) {
    if (this == &other) 
        return *this;

    delete[] ptr; 

    _size = other._size; 
    ptr = new T[_size]; 
    for (int i = 0; i < _size; i++) {
        ptr[i] = other.ptr[i];
    }

    return *this; 
}

// [] Operator
template <typename T> T& myVector<T>::operator[](int index) {
    if (index < 0 || index >= _size) throw out_of_range("Index out of bounds");
    return ptr[index];
}

// Print Function
template <typename T> void myVector<T>::print() {
    for (int i = 0; i < _size; i++) cout << ptr[i] << " ";
    cout << endl;
}

// Size Function
template <typename T> int myVector<T>::size() {
    return _size;
}

// Push Back Function 
template<typename T> void myVector<T>::push_back(T value) {
    if (_size == capacity) {
        capacity = (capacity == 0) ? 1 : capacity * 2;
        T* temp_ptr = new T[capacity];
        for(int i = 0; i < _size; i++) {
            temp_ptr[i] = ptr[i];
        }
        delete[] ptr;
        ptr = temp_ptr;

    }
    ptr[_size] = value;
    _size++;
}

// Insert Function
template<typename T> void myVector<T>::insert(int index, T value) {
    if (index < 0 || index > _size) {
        throw std::out_of_range("Index out of bounds");
    }
    if (_size == capacity) {
        capacity = (capacity == 0) ? 1 : capacity * 2;
        T* temp_ptr = new T[capacity];
        for(int i = 0; i < index; i++) temp_ptr[i] = ptr[i];
        temp_ptr[index] = value;
        for (int j = index + 1; j < _size + 1; j++) temp_ptr[j] = ptr[j-1];
        delete[] ptr;
        ptr = temp_ptr;
    }
    else {
        for (int i = _size; i > index; i--) ptr[i] = ptr[i-1];
        ptr[index] = value;
    }

    _size++;
}

// Pop Back Function
template<typename T> T myVector<T>::pop_back() {
    if (_size == 0) {
        throw out_of_range("Tried to pop an empty vector");
    }
    _size--;
    T temp = ptr[_size]; ptr[_size] = T{};
    return temp;
}

// Erase Function
template<typename T> void myVector<T>::erase(int index) {
    if (index < 0 || index >= _size) {
        throw std::out_of_range("Index out of bounds in erase()");
    }
    if (_size == 0) {
        throw out_of_range("Tried to erase an empty vector");
    }
    for (int i = index; i < _size; i++) ptr[i] = ptr[i + 1];
    _size--;
}

// Destructor
template <typename T> myVector<T>::~myVector() {
    delete[] ptr;
}

int main() {
    cout << "Test Case 1:" << endl;
    cout << "Expected Output: [0, 1, 2, 3, 4]" << endl;
    myVector<int> v(5, 0);
    for (int i = 0; i < v.size(); i++) v[i] = i;
    v.print();
    cout << "Data Structure Size: " << v.size() << endl;

    cout << "\nTest Case 2:" << endl;
    cout << "Expected Output: [1, 5, 25, 125, 625, 3125, 15625]" << endl;

    myVector<int> v2; v2.push_back(1);
    for(int i = 1; i < 7; i++) v2.push_back(v2[i-1] * 5);
    v2.print(); 
    cout << "Data Structure Size: " << v2.size() << endl;

    cout << "\nTest Case 3:" << endl;
    cout << "Expected Output: [0, 2, 4, 6, 8, 10]" << endl;

    myVector<int> v3;
    v3.insert(0, 0);
    v3.insert(1, 4);
    v3.insert(1, 2);
    v3.insert(3, 10);
    v3.insert(3, 6);
    v3.insert(4, 8);
    v3.print();
    cout << "Data Structure Size: " << v3.size() << endl;

    cout << "\nTest Case 4:" << endl;
    cout << "Expected Output: [yes, no, maybe, sure]" << endl;

    myVector<string> v4;
    v4.push_back("yes");
    v4.push_back("no");
    v4.push_back("could be");
    v4.pop_back();
    v4.push_back("maybe");
    v4.push_back("sure");
    v4.print();
    cout << "Data Structure Size: " << v4.size() << endl;

    cout << "\nTest Case 5:" << endl;
    cout << "Expected Output: [4.3, 9.8, 1.7, 5.5, 19.12]" << endl;

    myVector<double> v5;
    v5.push_back(4.3);
    v5.push_back(0.0);
    v5.push_back(9.8);
    v5.push_back(1.7);
    v5.push_back(0.0);
    v5.push_back(5.5);
    v5.push_back(0.0);
    v5.push_back(19.12);
    v5.erase(6);
    v5.erase(4);
    v5.erase(1);
    v5.print();
    cout << "Data Structure Size: " << v5.size() << endl;

    return 0;
}


