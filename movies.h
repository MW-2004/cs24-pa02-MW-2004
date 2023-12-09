#ifndef MOVIES_H
#define MOVIES_H
#include <iostream>
#include <functional>
#include <chrono>
using namespace std;

struct Timer {
    std::chrono::time_point<std::chrono::system_clock> startTime;
    Timer() {
        startTime = std::chrono::system_clock::now();
    }
    ~Timer() {
        auto endTime = std::chrono::system_clock::now();
        auto elapsed_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
        std::cout << "Time difference: " << elapsed_milliseconds << " milliseconds." << std::endl;
    }
};
void timer();
struct movie {
    string name;
    double rat;
    movie(string& s, double d);
};
struct best {
    string pr;
    reference_wrapper<movie> mov;
    best(string& s, movie& m);
};
struct comparer {
    bool operator()(const movie& movie, const string& prefix);
    bool operator()(const string& prefix, const movie& movie);
};
#endif