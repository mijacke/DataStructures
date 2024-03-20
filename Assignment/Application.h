#pragma once

#include "BusStopManager.h"
#include <string>

class Application
{
public:
    void run();
private:
    BusStopManager manager;
    void applyFilter();
    void displayFilterResults(const std::vector<BusStop>& filteredStops);
    void chooseCSVFile();
};