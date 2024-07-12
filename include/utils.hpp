#ifndef UTILS_HPP
#define UTILS_HPP

#include "consts.hpp"
#include <fstream>

str getNow();

str getFilename();

str empty(int n);

str zfill(str s, int n);

str replace(str string, str old, str val);

void createProfiles(str filename, vec<Profile>& profiles);

vec<Profile> getProfiles(str& filename);

vec<str> getColors(vec<double> values, int reverse, int pallette, int change);

str rgb(int r, int g, int b);

str hex(int number);

std::ofstream openFile(str filename);

template <typename T>
void innerCombinations(
    vec<T> array,
    vec<vec<T>>& combs,
    vec<T>& comb,
    size_t n,
    int r,
    int row,
    int col
    )
{
    if (r == col) {
        combs.push_back(comb);
        return;
    }
    for (size_t i = row; i < n; i++) {
        comb[col] = array[i];
        innerCombinations(array, combs, comb, n, r, i + 1, col + 1);
    }
}

template <typename T>
vec<vec<T>> combinations(vec<T> array, int r)
{
    vec<vec<T>> combs;
    vec<T> comb(r);
    innerCombinations(array, combs, comb, array.size(), r, 0, 0);
    return combs;
}

int combNumber(int n, int r);

vec<double> getHeadcount(
    vec<int> perm,
    vec<double> hc_per_shift,
    vec<map<str, double>>& activities,
    double work_hour
);

vec<double> getActual(
    vec<int> perm,
    vec<double> hc_per_shift1,
    vec<double> hc_per_shift2,
    vec<map<str, double>>& activities,
    double work_hour
);

vec<double> getActualWithBreak(
    vec<int> perm,
    vec<double> hc_per_shift1,
    vec<double> hc_per_shift2,
    vec<map<str, double>>& activities,
    double work_hour
);

map<str, vec<double>> hasCoverage(
    vec<double> need, 
    vec<double> actual, 
    vec<double> trend
);

void innerFloatToStr(
    double arg, 
    vec<int> units, 
    vec<int>& list, 
    int n, 
    int col
);

str floatToStr(double arg, vec<int> units);

str convertNumber(int n);

vec<str> innerSplit(str string, str sep, vec<str>& splitted);

vec<str> split(str string, str sep);

int min(vec<int> arr);

int max(vec<int> arr);

template <typename T>
vec<T> slice(vec<T> array, int start, int end)
{
    vec<T> sliced;
    sliced.insert(sliced.end(), array.begin() + start, array.begin() + end);
    return sliced;
}

str join(vec<str> strings, str sep);

double harmonicMean(vec<double> values);

#endif // UTILS_HPP
