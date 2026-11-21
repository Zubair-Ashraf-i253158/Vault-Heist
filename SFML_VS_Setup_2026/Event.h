#pragma once
#include <iostream>
#include <string>

using namespace std;

struct Event {
    string type;  // "insert", "compare", "unlink", "swap", "rotate", "heap_swap"
    int a;
    int b;
    int c;
    int oldVal;
    int newVal;

    Event() {
        type = "";
        a = 0; b = 0; c = 0;
        oldVal = 0; newVal = 0;
    }

    Event(string t, int p1, int p2, int p3, int o, int n) {
        type = t;
        a = p1; b = p2; c = p3;
        oldVal = o; newVal = n;
    }
};

class EventLog {
public:
    Event list[2000];
    int count;

    EventLog() {
        count = 0;
    }

    void add(string type, int a, int b, int c, int oldVal, int newVal) {
        if (count < 2000) {
            list[count] = Event(type, a, b, c, oldVal, newVal);
            count++;
        }
    }

    void clear() {
        count = 0;
    }
};