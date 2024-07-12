#ifndef BREAK_PLAN_HPP
#define BREAK_PLAN_HPP

#include "datetime.hpp"

class BreakPlan {
public:
    BreakPlan(
        vec<str> shifts,
        vec<vec<int>> hc,
        vec<map<str, double>> activities_values,
        vec<double> need,
        double work_hour
    );
    ~BreakPlan();
    vec<str> shifts;
    vec<vec<int>> hc;
    vec<map<str, double>> activities_values;
    vec<double> need;
    map<str, double> needs;
    map<str, double> actuals;
    map<str, double> coverages;
    void createBreakPlan();
    double work_hour;
    int cols;
    vec<vec<double>> hourlyActuals();
    vec<vec<vec<str>>> data;
protected:
    Datetime *datetime;
    
private:
    void getNeeds();
    void getActuals();
    int max(vec<double> values);
};

#endif // BREAK_PLAN_HPP

