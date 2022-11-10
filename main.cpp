#include <chrono>
#include <gtest/gtest.h>
#include <iostream>
#include <new>
#include <set>
#include <sstream>

#include "biginteger.h"

using std::chrono::steady_clock::time_point;
using std::chrono::duration_cast;
using std::chrono::milliseconds;

class OperatorNewCounter {
  private:
    int counter = 0;
    size_t total_size = 0;
    static std::set<OperatorNewCounter*> instances;
    
    void notify(size_t size) {
        ++counter;
        total_size += size;
    }

  public:
    static void notify_all(size_t size) {
        for (item : instances) {
            item->notify(size);
        }
    }

    int get_counter() {
        return counter;
    }

    size_t get_total_allocated() {
        return total_size;
    }
    

    ~OperatorNewCounter() {
        instances.erase(this);
    }
};

void* operator new[] (size_t size) {
    OperatorNewCounter::notify_all(size);
    void* p = malloc(size);
    if (p == nullptr) throw std::bad_alloc();
    return p;
}


