#include <iostream>
#include "Benchmark.h"

int main() {
    cout << "========================================" << endl;
    cout << "  SECTION 10: BENCHMARK SUITE" << endl;
    cout << "========================================" << endl;

    int sizes[] = { 100, 1000, 10000 };

    for (int n : sizes) {
        cout << "\n>>> RUNNING BENCHMARKS FOR N = " << n << " <<<\n" << endl;
        Benchmark::runTreeComparison(n);
        cout << "\n--- Heap Benchmarks ---" << endl;
        Benchmark::runHeapBenchmark(n);
    }

    return 0;
}