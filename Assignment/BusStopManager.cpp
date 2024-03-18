#include "BusStopManager.h"

#include <fstream>
#include <sstream> // for std::stringstream
#include <iostream>
#include <iomanip> // for std::setw and std::setprecision

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


	std::cout << std::setw(4) << std::left << "Line" << "|"
		<< std::setw(10) << std::left << "StopID" << "|"
		<< std::setw(50) << std::left << "StopName" << "|"
		<< std::setw(50) << std::left << "StopSite" << "|"
		<< std::setw(12) << std::left << "Latitude" << "|"
		<< std::setw(12) << std::left << "Longitude" << "|"
		<< std::setw(5) << std::left << "SysCode" << "|"
		<< std::setw(40) << std::left << "System" << "|"
		<< std::setw(20) << std::left << "Municipality"
		<< std::endl;
	std::cout << std::string(210, '-') << std::endl;

	while (std::getline(file, line))
	{
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

			std::cout << std::setw(4) << std::left << lineCounter << "|"
				<< std::setw(10) << std::left << stopid << "|"
				<< std::setw(50) << std::left << stopname << "|"
				<< std::setw(50) << std::left << stopsite << "|"
				<< std::setw(12) << std::left << std::fixed << std::setprecision(6) << latitude << "|"
				<< std::setw(12) << std::left << std::fixed << std::setprecision(6) << longitude << "|"
				<< std::setw(5) << std::left << syscode << "|" 
				<< std::setw(40) << std::left << system << "|"
				<< std::setw(20) << std::left << municipality
				<< std::endl;
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
