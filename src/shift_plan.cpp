#include "../include/shift_plan.hpp"

ShiftPlan::ShiftPlan(
    vec<str> shifts, 
    int rows, 
    int cols, 
    int off, 
    double work_hour
)
    : shifts(shifts),
    rows(rows),
    cols(cols),
    size(shifts.size()),
    off(off),
    work_hour(work_hour)
{
    init();
}

ShiftPlan::~ShiftPlan() {};

map<str, vec<int>> ShiftPlan::sum()
{
    map<str, vec<int>> result;
    for (int col = 0; col < cols; col++)
    {
        for (int row = 0; row < rows; row++)
        {
            str value = data[row][col];
            if (result[value].empty())
            {
                result[value] = vec<int>(cols);
            }
            result[value][col]++;
        }
    }
    return result;
}

void ShiftPlan::init()
{
    data = vec<vec<str>>(rows);
    map<int, vec<vec<str>>> temp;
    for (int group = 0; group < size; group++) {
        groups[group] = 0;
        temp[group] = vec<vec<str>>();
    }
    for (int row = 0; row < rows; row++)
    {
        data[row] = vec<str>(cols);
        int group = row % size;
        for (int col = 0; col < cols; col++)
        {
            int shift = (group + (col / 7)) % size;
            data[row][col] = shifts[shift];
            
        }
        temp[group].push_back(data[row]);
        groups[group]++;
    }
    data = {};
    for (auto& [key, value] : temp)
    {
        data.insert(data.end(), value.begin(), value.end());
    }
}

int ShiftPlan::max(str key, int start, int end)
{
    double maximum = -INFINITY;
    int i = 0;
    map<str, vec<int>> hc = sum();
    for (int col = start; col < end; col++)
    {
        int value = hc[key][col];
        if (value >= maximum)
        {
            maximum = value;
            i = col;
        }
    }
    return i;
}

int ShiftPlan::findLastOff(int row, int col)
{
    int index = -1;
    for (int c = 0; c < col; c++)
    {
        if (data[row][c] == "Off")
        {
            index = c;
        }
    }
    return index;
}

int ShiftPlan::findGroup(int row)
{
    int start = 0;
    int end = groups[0];
    int group = 0;
    for (int i = 0; i < size; i++)
    {
        if (start <= row && row < end)
        {
            group = i;
            break;
        }
        start += groups[i];
        end += groups[i + 1];
    }
    return group;
}

void ShiftPlan::firstShift(int week, int o, int row, int col)
{
    map<str, vec<int>> hc = sum();
    if (!week)
    {
        int max_col = max(shifts[0], col, col + 7);
        data[row][max_col] = "Off";
    }
    else
    {
        if (!o)
        {
            if (data[row][col - 1] != "Off")
            {
                data[row][col] = "Off";
            }
            else
            {
                int last_off = findLastOff(row, col);
                int max_col = max(shifts[0], col + 1, last_off + 7);
                data[row][max_col] = "Off";
            }
        }
        else
        {
            int max_col = max(shifts[0], col + 1, col + 7);
            data[row][max_col] = "Off";
        }
    }
}

void ShiftPlan::lastShift(int week, int o, int row, int col)
{
    map<str, vec<int>> hc = sum();
    if (!week)
    {
        if (!o)
        {
            if (hc[shifts[size - 1]][col + 6] > groups[0] / 2)
            {
                data[row][col + 6] = "Off";
            }
            else
            {
                int max_col = max(shifts[size - 1], col, col + 7);
                data[row][max_col] = "Off";
            }
        }
        else
        {
            int max_col = max(shifts[size - 1], col, col + 7);
            data[row][max_col] = "Off";
        }
    }
    else
    {
        if (!o)
        {
            if (hc[shifts[size - 1]][col + 6] > groups[0] / 2)
            {
                data[row][col + 6] = "Off";
            }
            else
            {
                int last_off = findLastOff(row, col);
                int max_col = max(shifts[size - 1], col, last_off + 7);
                data[row][max_col] = "Off";
            }
        }
        else
        {
            int last_off_previous = findLastOff(row, col);
            int last_off_current = findLastOff(row, col + 7);
            int limit;
            if (last_off_current - last_off_previous > 6)
            {
                limit = last_off_previous + 7;
            }
            else
            {
                limit = col + 7;
            }
            int max_col = max(shifts[size - 1], col, limit);
            data[row][max_col] = "Off";
        }
    }
}

void ShiftPlan::otherShifts(
    int week, 
    int o, 
    int row, 
    int col, 
    str weekly_shift
)
{
    if (!week)
    {
        int max_col = max(weekly_shift, col, col + 7);
        data[row][max_col] = "Off";
    }
    else
    {
        if (!o)
        {
            int last_off = findLastOff(row, col);
            int max_col = max(weekly_shift, col, last_off + 7);
            data[row][max_col] = "Off";  
        }
        else
        {
            int max_col = max(weekly_shift, col, col + 7);
            data[row][max_col] = "Off"; 
        }
    }
}

void ShiftPlan::createShiftPlan()
{
    int need = (rows / (double)size) * (1 - off / (double)7);
    for (int week = 0; week < cols / 7; week++)
    {
        int row = 0;
        int col = 7 * week;
        for (int group = 0; group < size; group++)
        {
            for (int user = 0; user < groups[group]; user++)
            {
                str weekly_shift = shifts[(findGroup(row) + (col / 7)) % size];
                for (int o = 0; o < off; o++)
                {
                    if (weekly_shift == shifts[0])
                    {
                        firstShift(week, o, row, col);
                    }
                    else if (weekly_shift == shifts[size - 1])
                    {
                        lastShift(week, o, row, col);
                    }
                    else
                    {
                        otherShifts(week, o, row, col, weekly_shift);
                    }
                }                
                row++;
            }
        }
    }
    equalizeShifts();
}

void ShiftPlan::equalizeShifts()
{
    int need = (rows / (double)size) * (1 - off / (double)7);
    for (int week = 0; week < cols / 7; week++)
    {
        int col = week * 7;
        for (int row = 0; row < rows; row++)
        {
            int r = rows - row - 1;
            int group = findGroup(r);
            int shift = (group + week) % size;
            if (shift == size - 1)
            {
                continue;
            }
            map<str, vec<int>> hc = sum();
            int next = shift + 1;
            if (
                hc[shifts[next]][col + 6] < need 
                && 
                data[r][col + 6] == "Off" 
                && 
                hc[shifts[shift]][col] > need
            )
            {
                data[r][col + 6] = shifts[next];
                data[r][col] = "Off";
                continue;
            }
            if (hc[shifts[next]][col + 6] < need && data[r][col + 6] != "Off")
            {
                data[r][col + 6] = shifts[next];
            }
        }
    }
    for (int week = 0; week < cols / 7; week++)
    {
        if (!week)
        {
            continue;
        }
        int col = week * 7;
        for (int row = 0; row < rows; row++)
        {
            int group = findGroup(row);
            int shift = (group + week) % size;
            if (shift == 0)
            {
                continue;
            }
            map<str, vec<int>> hc = sum();
            int previous = shift - 1;
            if (
                hc[shifts[previous]][col] < need 
                && 
                data[row][col] != "Off"
            )
            {
                data[row][col] = shifts[previous];
            }
        }
    }
}

