#pragma once
#include <iostream>
#include <ctime>
#include <cstdlib>
#include "BST.h"
#include "AVL.h"
#include "MaxHeap.h"

using namespace std;

class Benchmark
{
public:
    static void runTreeComparison(int n)//BST aur AVL ki height aur search cost compare karega
    {
        BST bst;
        AVL avl;

        bst.events = nullptr;//benchmark ke dauran logging band karega
        avl.events = nullptr;

        for (int i = 1; i <= n; i++)//dono trees mein ascending numbers insert karega
        {
            bst.insert(i);
            avl.insert(i);
        }

        cout << "--------------------------------------------------" << endl;
        cout << "  DATA SIZE N = " << n << endl;
        cout << "--------------------------------------------------" << endl;
        cout << "BST Height:        " << bst.treeHeight() << " (Degenerates to O(N))" << endl;
        cout << "AVL Height:        " << avl.treeHeight() << " (Balanced O(log N))" << endl;
        cout << "AVL Rotations:     " << avl.rotationCount << endl;

        bst.comparisons = 0;//BST ke comparisons reset karega
        bst.search(n);
        int bstComps = bst.comparisons;

        avl.comparisons = 0;
        avl.search(n);
        int avlComps = avl.comparisons;

        cout << "BST Search Checks: " << bstComps << " comparisons" << endl;
        cout << "AVL Search Checks: " << avlComps << " comparisons" << endl;

        clock_t start = clock();//BST search ka time check karega
        for (int i = 1; i <= n; i++)//BST mein har value ko search karega
        {
            bst.search(i);
        }
        clock_t end = clock();
        double bstTime = double(end - start) / CLOCKS_PER_SEC;

        start = clock();
        for (int i = 1; i <= n; i++)
        {
            avl.search(i);
        }
        end = clock();
        double avlTime = double(end - start) / CLOCKS_PER_SEC;

        cout << "BST Search Time:   " << bstTime << " s" << endl;
        cout << "AVL Search Time:   " << avlTime << " s" << endl;
    }

    static void runHeapBenchmark(int n)//heap build aur heap sort ka time compare karega
    {
        MaxHeap heap;
        heap.events = nullptr;//benchmark ke dauran logging band karega

        heap.total = n;//heap mein total values set karega
        for (int i = 0; i < n; i++)//heap mein random values dalega
        {
            heap.data[i] = rand() % 100000;
        }

        clock_t start = clock();//heap build ka time check karega
        heap.buildMaxHeap();
        clock_t end = clock();
        double buildTime = double(end - start) / CLOCKS_PER_SEC;

        start = clock();//heap sort ka time check karega
        heap.heapSort();
        end = clock();
        double sortTime = double(end - start) / CLOCKS_PER_SEC;

        cout << "Heap Build O(n):   " << buildTime << " s" << endl;
        cout << "Heap Sort O(nlogn):" << sortTime << " s" << endl;
        cout << "Array Sorted:      " << (heap.isSorted() ? "PASS" : "FAIL") << endl;
    }
};