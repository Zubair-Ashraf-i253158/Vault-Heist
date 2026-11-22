#include "MaxHeap.h"

MaxHeap::MaxHeap()//heap ko initialize karega
{
    total = 0;
    heapSize = 0;
    events = nullptr;
    comparisons = 0;
    swaps = 0;
}

void MaxHeap::note(string type, int a, int b, int c, int oldVal, int newVal)//event ko record karega
{
    if (events != nullptr)
    {
        events->add(type, a, b, c, oldVal, newVal);
    }
}

void MaxHeap::swapItems(int i, int j, string reason)//dono indexes ki values swap karega
{
    int temp = data[i];
    note("heap_swap", i, j, 0, data[i], data[j]);

    data[i] = data[j];
    data[j] = temp;
    swaps++;
}

void MaxHeap::heapifyDown(int i, int size)//node ko neeche sahi position par le jayega
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < size)
    {
        comparisons++;
        if (data[left] > data[largest])
            largest = left;
    }

    if (right < size)
    {
        comparisons++;
        if (data[right] > data[largest])
            largest = right;
    }

    if (largest != i)
    {
        swapItems(i, largest, "down");
        heapifyDown(largest, size);
    }
}

void MaxHeap::siftDown(int i, int size)//node ko neeche move karke heap sahi karega
{
    note("heapify_start", i, size, 0, 0, 0);
    heapifyDown(i, size);
    note("heapify_done", i, size, 0, 0, 0);
}

void MaxHeap::siftUp(int i)//node ko upar sahi position par le jayega
{
    while (i > 0)
    {
        int parent = (i - 1) / 2;
        comparisons++;

        if (data[i] > data[parent])
        {
            swapItems(i, parent, "up");
            i = parent;
        }
        else
        {
            break;
        }
    }
}

void MaxHeap::buildMaxHeap()//array ko max heap mein convert karega
{
    heapSize = total;
    note("heap_build", 0, heapSize, 0, 0, 0);

    for (int i = heapSize / 2 - 1; i >= 0; i--)
    {
        siftDown(i, heapSize);
    }
}

int MaxHeap::extractMax()//heap ki maximum value nikal kar return karega
{
    if (heapSize == 0) return 0;

    int maxValue = data[0];
    int last = heapSize - 1;

    note("extract_max", 0, heapSize, 0, maxValue, 0);

    if (last != 0)
    {
        swapItems(0, last, "extract");
    }

    note("mark_sorted", last, 0, 0, 0, maxValue);
    note("heap_size_change", 0, 0, 0, heapSize, last);
    heapSize--;

    if (heapSize > 0)
    {
        siftDown(0, heapSize);
    }

    return maxValue;
}

bool MaxHeap::insert(int value)//new value ko heap mein insert karega
{
    if (total >= 10000 || heapSize != total) return false;

    data[total] = value;
    total++;

    note("heap_size_change", 0, 0, 0, heapSize, heapSize + 1);
    heapSize++;

    siftUp(heapSize - 1);
    return true;
}

void MaxHeap::heapSort()//heap ko use karke array sort karega
{
    buildMaxHeap();

    while (heapSize > 1)
    {
        extractMax();
    }

    if (heapSize == 1)
    {
        note("mark_sorted", 0, 0, 0, 0, data[0]);
        note("heap_size_change", 0, 0, 0, 1, 0);
        heapSize = 0;
    }

    note("heap_sort_complete", total, 0, 0, 0, 0);
}

bool MaxHeap::isMaxHeap()//check karega ke max heap sahi hai ya nahi
{
    for (int i = 1; i < heapSize; i++)
    {
        int parent = (i - 1) / 2;
        if (data[i] > data[parent])
            return false;
    }
    return true;
}

bool MaxHeap::checkSortedRegion()//sorted region aur heap region check karega
{
    if (heapSize < 0 || heapSize > total) return false;

    for (int i = heapSize; i < total - 1; i++)
    {
        if (data[i] > data[i + 1])
            return false;
    }

    if (heapSize > 0 && heapSize < total)
    {
        if (data[heapSize] < data[0])
            return false;
    }

    return true;
}

bool MaxHeap::isSorted()//check karega ke array ascending order mein hai
{
    for (int i = 0; i < total - 1; i++)
    {
        if (data[i] > data[i + 1])
            return false;
    }
    return true;
}

bool MaxHeap::validate()//heap aur sorted region dono check karega
{
    return isMaxHeap() && checkSortedRegion();
}