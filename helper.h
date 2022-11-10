#include <chrono>
#include <gtest/gtest.h>
#include <iostream>
#include <new>
#include <set>

using std::chrono::steady_clock;
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
        for (auto item : instances) {
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

std::set<OperatorNewCounter*> OperatorNewCounter::instances = std::set<OperatorNewCounter*>();

void* operator new[] (size_t size) {
    OperatorNewCounter::notify_all(size);
    void* p = malloc(size);
    if (p == nullptr) throw std::bad_alloc();
    return p;
}

class Timer {
  private:
    steady_clock::time_point begin;
    steady_clock::time_point end;
  public:
    void start() {
        begin = steady_clock::now();
    }

    void finish() {
        end = steady_clock::now();
    }

    int get_time_milliseconds() {
        return duration_cast<milliseconds>(end - begin).count();
    }
};

