#pragma once
#include <iostream>
#include <ctime>
#include <cstdlib>
#include "BST.h"
#include "AVL.h"
#include "MaxHeap.h"

using namespace std;

class Benchmark {
public:
    // Experiment 1: Height & Search Cost (BST vs AVL)
    static void runTreeComparison(int n) {
        BST bst;
        AVL avl;

        // Disconnect logger during benchmarks:
        // We measure algorithm execution, not logging thousands of string copies
        bst.events = nullptr;
        avl.events = nullptr;

        // Insert ascending numbers
        for (int i = 1; i <= n; i++) {
            bst.insert(i);
            avl.insert(i);
        }

        cout << "--------------------------------------------------" << endl;
        cout << "  DATA SIZE N = " << n << endl;
        cout << "--------------------------------------------------" << endl;
        cout << "BST Height:        " << bst.treeHeight() << " (Degenerates to O(N))" << endl;
        cout << "AVL Height:        " << avl.treeHeight() << " (Balanced O(log N))" << endl;
        cout << "AVL Rotations:     " << avl.rotationCount << endl;

        // Measure Search Comparisons for the worst-case item (last element)
        bst.comparisons = 0;
        bst.search(n);
        int bstComps = bst.comparisons;

        avl.comparisons = 0;
        avl.search(n);
        int avlComps = avl.comparisons;

        cout << "BST Search Checks: " << bstComps << " comparisons" << endl;
        cout << "AVL Search Checks: " << avlComps << " comparisons" << endl;

        // Search timing via clock()
        clock_t start = clock();
        for (int i = 1; i <= n; i++) {
            bst.search(i);
        }
        clock_t end = clock();
        double bstTime = double(end - start) / CLOCKS_PER_SEC;

        start = clock();
        for (int i = 1; i <= n; i++) {
            avl.search(i);
        }
        end = clock();
        double avlTime = double(end - start) / CLOCKS_PER_SEC;

        cout << "BST Search Time:   " << bstTime << " s" << endl;
        cout << "AVL Search Time:   " << avlTime << " s" << endl;
    }

    // Experiment 2: Heap Construction vs Heap Sort
    static void runHeapBenchmark(int n) {
        MaxHeap heap;
        heap.events = nullptr; // Disconnect logger

        heap.total = n;
        for (int i = 0; i < n; i++) {
            heap.data[i] = rand() % 100000;
        }

        clock_t start = clock();
        heap.buildMaxHeap();
        clock_t end = clock();
        double buildTime = double(end - start) / CLOCKS_PER_SEC;

        start = clock();
        heap.heapSort();
        end = clock();
        double sortTime = double(end - start) / CLOCKS_PER_SEC;

        cout << "Heap Build O(n):   " << buildTime << " s" << endl;
        cout << "Heap Sort O(nlogn):" << sortTime << " s" << endl;
        cout << "Array Sorted:      " << (heap.isSorted() ? "PASS" : "FAIL") << endl;
    }
};