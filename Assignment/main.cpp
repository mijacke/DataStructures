#include "BusStopManager.h"
#include <iostream>
#include <filesystem>

int main()
{
	//#include <libds/amt/implicit_sequence.h>
	//ds::amt::ImplicitSequence<int> seq;

	std::cout << "Loading bus stops from CSV..." << std::endl;

	std::string path = "cow_busstops.csv";
	BusStopManager manager;
	manager.loadFromCSV(path);

	// Check how many bus stops have been loaded
	std::cout << "Bus stops loaded: " << manager.getBusStopCount() << std::endl;

	auto startsWith = [](const std::string& prefix) {
		return [&prefix](const BusStop& stop) {
			return stop.getName().substr(0, prefix.size()) == prefix;
		};
	};

	auto contains = [](const std::string& substr) {
		return [&substr](const BusStop& stop) {
			return stop.getName().find(substr) != std::string::npos;
		};
	};

	// Filter bus stops that start with ...
	std::cout << "Filtering bus stops..." << std::endl;

	std::vector<BusStop> filteredStops = manager.filterBusStops(startsWith("Davis"));
	for (const BusStop& stop : filteredStops)
	{
		std::cout << stop.getName() << " at " << stop.getMunicipality() << std::endl;
	}

	// Debug
	std::cout << "Finished filtering bus stops." << std::endl;

	return 0;
}