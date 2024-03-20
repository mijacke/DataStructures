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
    void displayFilterResults(const std::vector<BusStop>& stops);
    void handleFiltering(int choice);
};