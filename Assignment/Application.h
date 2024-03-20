#pragma once

#include "BusStopManager.h"
#include <string>

class Application
{
public:
    void run();
private:
    BusStopManager manager;
    void loadBusStops();
    void applyFilter();
    bool promptRetry();
};