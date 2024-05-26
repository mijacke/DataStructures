#pragma once

#include "BusStopManager.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <limits>

class Application
{
public:
    void run();
private:
    BusStopManager manager;
    void chooseCSVFile();
    void applyFilter();
    void applyFilterUroven2();
    void applyFilterUroven3();
    void applyFilterUroven4();
    void displayFilterResults(const std::vector<BusStop>& stops);
    void handleFiltering(int choice);
};