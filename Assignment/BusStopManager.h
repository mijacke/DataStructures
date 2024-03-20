#pragma once

#include "BusStop.h"
#include <vector>
#include <functional>

class BusStopManager
{
private:
	std::vector<BusStop> busStops;
	bool displayDetails = true;

public:
	void loadFromCSV(const std::string& filename);
	std::vector<BusStop> filterBusStops(std::function<bool(const BusStop&)> predicate) const;
	size_t getBusStopCount() const { return busStops.size(); }
	void clearBusStops() { busStops.clear(); }
	void setDisplayDetails(bool choice);
	bool getDisplayDetails() const { return displayDetails; }
};
