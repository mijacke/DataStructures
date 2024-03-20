#include "BusStopManager.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip> // for std::setw and std::setprecision


void BusStopManager::loadAllCSVs(const std::vector<std::string>& filenames) 
{
    for (const auto& filename : filenames) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open the file: " << filename << std::endl;
            continue;
        }

        std::string line;
        std::getline(file, line); // Skip header line.
        std::vector<BusStop> stops;

        while (std::getline(file, line)) {
            std::stringstream s(line);
            std::vector<std::string> fields;
            std::string field;
            while (std::getline(s, field, ';')) {
                fields.push_back(field);
            }

            if (fields.size() == 8) {
                stops.emplace_back(fields[0], fields[1], fields[2],
                        std::stod(fields[3]), std::stod(fields[4]),
                        fields[5], fields[6], fields[7]);
            }
            else {
                std::cerr << "Error: Line does not have 8 fields as expected." << std::endl;
            }
        }
        file.close();
        busStopsData[filename] = stops;
    }
}

void BusStopManager::setCurrentBusStops(const std::string& filename) 
{
    auto it = busStopsData.find(filename);
    if (it != busStopsData.end()) {
        currentBusStops = it->second;
    }
    else {
        std::cerr << "No data found for the file: " << filename << std::endl;
        currentBusStops.clear();
    }
}

std::vector<BusStop> BusStopManager::filterBusStops(std::function<bool(const BusStop&)> predicate) const 
{
    std::vector<BusStop> filtered;
    for (const auto& stop : currentBusStops) {
        if (predicate(stop)) {
            filtered.push_back(stop);
        }
    }
    return filtered;
}

void BusStopManager::setDisplayDetails(bool choice)
{
		displayDetails = choice;
}

std::vector<std::string> BusStopManager::getCSVFileNames() const 
{
	std::vector<std::string> filenames;
	for (const auto& pair : busStopsData) {
		filenames.push_back(pair.first);
	}
	return filenames;
}
