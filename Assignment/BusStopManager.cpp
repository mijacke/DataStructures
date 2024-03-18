#include "BusStopManager.h"

#include <fstream>
#include <sstream> // for std::stringstream
#include <iostream>

void BusStopManager::loadFromCSV(const std::string& filename)
{
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Failed to open the file: " << filename << std::endl;
		return;
	}

	std::string line;
	std::getline(file, line); // skip header line

	while (std::getline(file, line))
	{
		//stopid; stopname; stopsite; latitude; longitude; syscode; system; municipality
		std::stringstream s(line);
		std::string cell; // skiped header line
		std::string stopid;
		std::string stopname;
		std::string stopsite;
		double latitude;
		double longitude;
		std::string syscode;
		std::string system;
		std::string municipality;

		std::getline(s, cell, ';'); // skiped header line
		std::getline(s, stopid, ';');
		std::getline(s, stopname, ';');
		std::getline(s, stopsite, ';');
		s >> latitude; s.ignore(); // s.ignore() to skip the separator
		s >> longitude; s.ignore(); // s.ignore() to skip the separator
		std::getline(s, syscode, ';');
		std::getline(s, system, ';');
		std::getline(s, municipality, ';');

		busStops.emplace_back(stopid, stopname, stopsite, latitude, longitude, syscode, system, municipality);
	}
	file.close();
}

std::vector<BusStop> BusStopManager::filterBusStops(std::function<bool(const BusStop&)> predicate) const
{
	std::vector<BusStop> filteredBusStops;
	for (const auto& stop : busStops)
	{
		if (predicate(stop))
		{
			filteredBusStops.push_back(stop);
		}
	}
	return filteredBusStops;
}
