#pragma once
#include <iostream>
#include <string>
#include "Event.h"

using namespace std;

class MaxHeap {
public:
    int data[10000];
    int total;
    int heapSize;
    int comparisons;
    int swaps;
    EventLog* events;

    MaxHeap();

    void note(string type, int a, int b, int c, int oldVal, int newVal);

    void swapItems(int i, int j, string reason);
    void heapifyDown(int i, int size);
    void siftDown(int i, int size);
    void siftUp(int i);

    void buildMaxHeap();
    int extractMax();
    bool insert(int value);
    void heapSort();

    bool isMaxHeap();
    bool checkSortedRegion();
    bool isSorted();
    bool validate();
};