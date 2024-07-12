#ifndef ANALYSIS_RESULTS_HPP
#define ANALYSIS_RESULTS_HPP

#include "table.hpp"

class AnalysisResults : public Table {
public:
    AnalysisResults(
        str title,
        vec<str> columns,
        QWidget *parent = nullptr
    );
    ~AnalysisResults();
    void init() override;
};

#endif // ANALYSIS_RESULTS_HPP
