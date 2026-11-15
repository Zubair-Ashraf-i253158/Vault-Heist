#include "MaxHeap.h"

MaxHeap::MaxHeap() {
    total = 0;
    heapSize = 0;
    events = nullptr;
    comparisons = 0;
    swaps = 0;
}

void MaxHeap::note(string type, int a, int b, int c, int oldVal, int newVal) {
    if (events != nullptr) {
        events->add(type, a, b, c, oldVal, newVal);
    }
}

void MaxHeap::swapItems(int i, int j, string reason) {
    int temp = data[i];
    note("heap_swap", i, j, 0, data[i], data[j]);

    data[i] = data[j];
    data[j] = temp;
    swaps++;
}

void MaxHeap::heapifyDown(int i, int size) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < size) {
        comparisons++;
        if (data[left] > data[largest])
            largest = left;
    }

    if (right < size) {
        comparisons++;
        if (data[right] > data[largest])
            largest = right;
    }

    if (largest != i) {
        swapItems(i, largest, "down");
        heapifyDown(largest, size);
    }
}

void MaxHeap::siftDown(int i, int size) {
    note("heapify_start", i, size, 0, 0, 0);
    heapifyDown(i, size);
    note("heapify_done", i, size, 0, 0, 0);
}

void MaxHeap::siftUp(int i) {
    while (i > 0) {
        int parent = (i - 1) / 2;
        comparisons++;

        if (data[i] > data[parent]) {
            swapItems(i, parent, "up");
            i = parent;
        }
        else {
            break;
        }
    }
}

void MaxHeap::buildMaxHeap() {
    heapSize = total;
    note("heap_build", 0, heapSize, 0, 0, 0);

    for (int i = heapSize / 2 - 1; i >= 0; i--) {
        siftDown(i, heapSize);
    }
}

int MaxHeap::extractMax() {
    if (heapSize == 0) return 0;

    int maxValue = data[0];
    int last = heapSize - 1;

    note("extract_max", 0, heapSize, 0, maxValue, 0);

    if (last != 0) {
        swapItems(0, last, "extract");
    }

    note("mark_sorted", last, 0, 0, 0, maxValue);
    note("heap_size_change", 0, 0, 0, heapSize, last);
    heapSize--;

    if (heapSize > 0) {
        siftDown(0, heapSize);
    }

    return maxValue;
}

bool MaxHeap::insert(int value) {
    if (total >= 500) return false;

    data[total] = value;
    total++;

    note("heap_size_change", 0, 0, 0, heapSize, heapSize + 1);
    heapSize++;

    siftUp(heapSize - 1);
    return true;
}

void MaxHeap::heapSort() {
    buildMaxHeap();

    while (heapSize > 1) {
        extractMax();
    }

    if (heapSize == 1) {
        note("mark_sorted", 0, 0, 0, 0, data[0]);
        note("heap_size_change", 0, 0, 0, 1, 0);
        heapSize = 0;
    }

    note("heap_sort_complete", total, 0, 0, 0, 0);
}

bool MaxHeap::isMaxHeap() {
    for (int i = 1; i < heapSize; i++) {
        int parent = (i - 1) / 2;
        if (data[i] > data[parent])
            return false;
    }
    return true;
}

bool MaxHeap::checkSortedRegion() {
    if (heapSize < 0 || heapSize > total) return false;

    for (int i = heapSize; i < total - 1; i++) {
        if (data[i] > data[i + 1])
            return false;
    }

    if (heapSize > 0 && heapSize < total) {
        if (data[heapSize] < data[0])
            return false;
    }

    return true;
}

bool MaxHeap::isSorted() {
    for (int i = 0; i < total - 1; i++) {
        if (data[i] > data[i + 1])
            return false;
    }
    return true;
}

bool MaxHeap::validate() {
    return isMaxHeap() && checkSortedRegion();
}