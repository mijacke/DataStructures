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
	int lineCounter = 1;

	while (std::getline(file, line))
	{
		//stopid; stopname; stopsite; latitude; longitude; syscode; system; municipality
		std::stringstream s(line);
		std::string field;
		std::vector<std::string> fields;

		// Read each field from the line into the fields vector
		while (std::getline(s, field, ';')) {
			fields.push_back(field);
		}


		if (fields.size() == 8) {
			std::string stopid = fields[0];
			std::string stopname = fields[1];
			std::string stopsite = fields[2];
			double latitude = std::stod(fields[3]);
			double longitude = std::stod(fields[4]);
			std::string syscode = fields[5];
			std::string system = fields[6];
			std::string municipality = fields[7];

			busStops.emplace_back(stopid, stopname, stopsite, latitude, longitude, syscode, system, municipality);

			std::cout << lineCounter << ". " << stopid << "; " << stopname << "; " << stopsite << "; "
				<< latitude << "; " << longitude << "; " << syscode << "; "
				<< system << "; " << municipality << std::endl;
		}
		else {
			std::cerr << "Error: Line " << lineCounter << " does not have 8 fields as expected." << std::endl;
		}
		lineCounter++;
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
