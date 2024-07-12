#ifndef CONSTS_HPP
#define CONSTS_HPP

#include "lib.hpp"

#include <map>
#include <string>
#include <vector>

using str = std::string;

template <typename T>
using vec = std::vector<T>;

template <typename T1, typename T2>
using map = std::map<T1, T2>;

struct Profile {
    str name;
    vec<map<str, double>> trends;
    vec<map<str, double>> inputs;
    vec<map<str, double>> activities;
};

struct ShiftValues {
    str shift;
    vec<double> actual;
    vec<double> coverage;
    vec<double> occupancy;
    vec<double> waiting_time;
};

extern map<str, map<str, str>> COLORS;

void init_COLORS();

#endif // CONSTS_HPP
