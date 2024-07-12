#ifndef SHIFT_PLAN_HPP
#define SHIFT_PLAN_HPP

#include "consts.hpp"
#include <iostream>

class ShiftPlan {
public:
    ShiftPlan(vec<str> shifts, int rows, int cols, int off, double work_hour);
    ~ShiftPlan();
    int size;
    int rows;
    int cols;
    int off;
    double work_hour;
    vec<str> shifts;
    vec<vec<str>> data;
    map<int, int> groups;
    template <typename T>
    void print(vec<vec<T>> arg)
    {
        for (auto& row : arg)
        {
            for (auto& col : row)
            {
                std::cout << col << " ";
            }
            std::cout << std::endl;
        }
    }
    void createShiftPlan();
    void equalizeShifts();
    void firstShift(int week, int o, int row, int col);
    void lastShift(int week, int o, int row, int col);
    void otherShifts(int week, int o, int row, int col, str weekly_shift);
    map<str, vec<int>> sum();
private:
    void init();
    int findGroup(int row);
    int findLastOff(int row, int col);
    int max(str key, int start, int end);
};

#endif // SHIFT_PLAN_HPP

