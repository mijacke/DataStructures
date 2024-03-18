#include "BusStopManager.h"
#include <iostream>
#include <filesystem>

int main()
{
	//#include <libds/amt/implicit_sequence.h>
	//ds::amt::ImplicitSequence<int> seq;

	std::cout << "Loading bus stops from CSV...\n" << std::endl;

	std::string path = "cow_busstops.csv";
	BusStopManager manager;
	manager.loadFromCSV(path);

	// Check how many bus stops have been loaded
	std::cout << "\nBus stops loaded: " << manager.getBusStopCount() << std::endl;

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

	std::string startsWithStr = "Davis";
	std::string containsStr = "Davis";

	std::vector<BusStop> filteredStopsStartsWith = manager.filterBusStops(startsWith(startsWithStr));
	std::vector<BusStop> filteredStopsContaining = manager.filterBusStops(contains(containsStr));

	// Filter bus stops that start with ...
	std::cout << "Filtering bus stops that starts with '" << startsWithStr << "'...\n" << std::endl;

	for (const BusStop& stop : filteredStopsStartsWith)
	{
		std::cout << stop.getName() << " at " << stop.getMunicipality() << std::endl;
	}
	
	// Filter bus stops that contain ...
	std::cout << "\nFiltering bus stops that contain '" << containsStr << "'...\n" << std::endl;

	for (const BusStop& stop : filteredStopsContaining)
	{
		std::cout << stop.getName() << " at " << stop.getMunicipality() << std::endl;
	}

	std::cout << "\nFinished filtering bus stops." << std::endl;

	return 0;
}