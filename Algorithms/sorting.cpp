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
    Worst Case: O(n log n)

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
        print(v);
        T currElement = v[i];
        int j = i - 1;
        while (j >= 0 and currElement < v[j]) {
            v[j+1] = v[j];
            j--;
        }
        v[j+1] = currElement;
        
    }
}

template <typename T> void mergeSort(vector<T>& v) {

}

template <typename T> void quickSort(vector<T>& v) {

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
    mergeSort(v4);
    print(v4);
    cout << endl;

    
    return 0;
}