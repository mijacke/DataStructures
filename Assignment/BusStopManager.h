#pragma once

#include "BusStop.h"
#include <vector>
#include <functional>
#include <map>

class BusStopManager
{
private:
	std::unordered_map<std::string, std::vector<BusStop>> busStopsData;
	std::vector<BusStop> currentBusStops;
	bool displayDetails = true;

public:
    void loadAllCSVs(const std::vector<std::string>& filenames);
    void setCurrentBusStops(const std::string& filename);
    std::vector<BusStop> filterBusStops(std::function<bool(const BusStop&)> predicate) const;
    size_t getBusStopCount() const { return currentBusStops.size(); }
    void setDisplayDetails(bool choice);
    bool getDisplayDetails() const { return displayDetails; }
    std::vector<std::string> getCSVFileNames() const;
    void printCurrentBusStopsDetails() const;
};
