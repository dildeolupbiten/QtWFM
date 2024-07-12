#ifndef ANALYSIS_HPP
#define ANALYSIS_HPP

#include "activities.hpp"
#include "inputs.hpp"
#include "trend.hpp"
#include "shifts.hpp"
#include "results.hpp"

class Analysis : public QFrame {
public:
    Analysis(QWidget *parent = nullptr);
    ~Analysis();
    Table *values;
    Results *results;
    Inputs *inputs;
    Activities *activities;
    Trend *trend;
    Shifts *shifts;
    QFrame *profile_frame;
    QFrame *left;
    QFrame *right;
    QFrame *left_left;
    QFrame *left_right;
    QFrame *btn_frame;
    QPushButton *btn_analyze;
    QPushButton *btn_shift_plan;
    QPushButton *btn_break_plan;
    QPushButton *btn_export;
    QFrame *pbar;
    QProgressBar *pbar_analysis;
    QProgressBar *pbar_break_plan;
    QProgressBar *pbar_shift_plan;
    QLabel *page;
    QLabel *label;
    QComboBox *combobox;
    QFrame *entry_frame;
    QLineEdit *entry;
    QFrame *profile_btn_frame;
    QPushButton *btn_add;
    QPushButton *btn_del;
    void config();
    void changeValues();
    void analyze();
    void createShiftPlan();
    void createBreakPlan();
    void exportResults();
    vec<Profile> profiles;
    void loadProfiles();
    vec<double> getNeed();
    void changeColors(map<int, vec<ShiftValues>>& data, int r, int index);
    void finished_analyze(map<int, vec<ShiftValues>> output);
    void progress_analyze(int value);
    void finished_shift_plan(ShiftPlan* plan);
    void progress_shift_plan(int value);
    void finished_break_plan(BreakPlan* plan);
    void progress_break_plan(int value);
    void addProfile();
    void delProfile();
    void changeProfile();
    void loadProfileValues(Profile profile);
    map<int, vec<ShiftValues>> combs;
};

#endif // ANALYSIS_HPP
