/*
Basic implementation of selection sort, bubble sort, insertion sort, merge sort, and quick sort. 

Selection Sort:
    Best Case: O(n^2)
    Average Case: O(n^2)
    Worst Case: O(n^2)

    Benefits: In-place

Bubble Sort:
    Best Case: O(n)
    Average Case: O(n^2)
    Worst Case: O(n^2)

    Benefits: Stable, In-place, Adaptive

Insertion Sort:
    Best Case: O(n)
    Average Case: O(n^2)
    Worst Case: O(n^2)

    Benefits: Stable, In-place, Adaptive

Merge Sort:
    Best Case: O(n log n)
    Average Case: O(n log n)
    Worst Case: O(n log n)
    
    Benefits: Stable

Quick Sort:
    Best Case: O(n log n)
    Average Case: O(n log n)
    Worst Case: O(n^2)

    Benefits: In-place

*/

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

template <typename T> void selectionSort(vector<T>& v) {
    for (int i = 0; i < v.size() - 1; i++) {
        for (int j = i + 1; j < v.size(); j++) {
            if (v[j] < v[i]) swap(v[j], v[i]);
        }
    }
}

template <typename T> void bubbleSort(vector<T>& v) {
    bool flag = true;
    for (int i = 0; i < v.size() - 1; i++) {
        flag = true;
        for (int j = 0; j < v.size() - i - 1; j++) {
            if (v[j] > v[j+1]) {
                swap(v[j], v[j+1]);
                flag = false; 
            }
        }
        if (flag) break;
    }
}

template <typename T> void insertionSort(vector<T>& v) {
    for (int i = 1; i < v.size(); i++) {
        T currElement = v[i];
        int j = i - 1;
        while (j >= 0 and currElement < v[j]) {
            v[j+1] = v[j];
            j--;
        }
        v[j+1] = currElement;
    }
}

template <typename T> vector<T> merge(const vector<T>& v1, const vector<T>& v2) {
    vector<T> merged_vector; int i = 0; int j = 0;
    while (i < v1.size() && j < v2.size()) {
        if (v1[i] <= v2[j]) merged_vector.push_back(v1[i++]); 
        else merged_vector.push_back(v2[j++]);
    }

    while (i < v1.size()) merged_vector.push_back(v1[i++]);
    while (j < v2.size()) merged_vector.push_back(v2[j++]);

    return merged_vector;
}

template <typename T> vector<T> mergeSort(vector<T> v) {
    vector<T> left(v.begin(), v.begin() + v.size()/2);
    vector<T> right(v.begin() + v.size()/2, v.end());
    if (left.size() > 1) left = mergeSort(left);
    if (right.size() > 1) right = mergeSort(right);
    return merge(left, right);
}

template <typename T> int partition(std::vector<T>& v, int low, int high) {
    T pivot = v[high];          
    int i = low - 1;

    for (int j = low; j < high; j++) {  
        if (v[j] < pivot) {
            i++;
            std::swap(v[i], v[j]);
        }
    }
    i++;
    std::swap(v[i], v[high]);
    return i;                            
}

template <typename T> void quickSortImpl(std::vector<T>& v, int low, int high) {
    if (low >= high) return;

    int p = partition(v, low, high);  
    quickSortImpl(v, low, p - 1);     
    quickSortImpl(v, p + 1, high);    
}

template <typename T> void quickSort(std::vector<T>& v) {
    if (v.empty()) return;
    quickSortImpl(v, 0, static_cast<int>(v.size()) - 1);
}





template <typename T> void print(vector<T>& v) {
    for (T element : v) cout << element << " ";
    cout << endl;
}

int main() {
    cout << "Test Case 1:" << endl;
    cout << "Expected Output: [1, 2, 3, 4, 5]" << endl;
    vector<int> v = {3, 2, 1, 5, 4};
    selectionSort(v);
    print(v);
    cout << endl;

    cout << "Test Case 2:" << endl;
    cout << "Expected Output: [6, 7, 8, 9, 10]" << endl;
    vector<int> v2 = {10, 9, 8, 6, 7};
    bubbleSort(v2);
    print(v2);
    cout << endl;

    cout << "Test Case 3:" << endl;
    cout << "Expected Output: [11, 12, 13, 14, 15]" << endl;
    vector<int> v3 = {15, 14, 11, 12, 13};
    insertionSort(v3);
    print(v3);
    cout << endl;

    cout << "Test Case 4:" << endl;
    cout << "Expected Output: ['a', 'b', 'c', 'd', 'e']" << endl;
    vector<char> v4 = {'e', 'c', 'a', 'b', 'd'};
    v4 = mergeSort(v4);
    print(v4);
    cout << endl;

    cout << "Test Case 5:" << endl;
    cout << "Expected Output: [10, 15, 20, 25, 30]" << endl;
    vector<int> v5 = {25, 10, 15, 30, 20};
    quickSort(v5);
    print(v5);
    cout << endl;


    return 0;
}