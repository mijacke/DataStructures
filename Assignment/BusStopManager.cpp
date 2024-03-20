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

void BusStopManager::printCurrentBusStopsDetails() const {
    if (!getDisplayDetails()) return; // Only print if detail display is enabled

    std::cout << std::setw(4) << "Line" << "|"
        << std::setw(10) << "StopID" << "|"
        << std::setw(50) << "StopName" << "|"
        << std::setw(50) << "StopSite" << "|"
        << std::setw(12) << "Latitude" << "|"
        << std::setw(12) << "Longitude" << "|"
        << std::setw(7) << "SysCode" << "|"
        << std::setw(40) << "System" << "|"
        << std::setw(20) << "Municipality" << std::endl;
    std::cout << std::string(210, '-') << std::endl;

    int lineCounter = 1;
    for (const auto& stop : currentBusStops) {
        std::cout << std::setw(4) << std::left << lineCounter++ << "|"
            << std::setw(10) << stop.getStopId() << "|"
            << std::setw(50) << stop.getName() << "|"
            << std::setw(50) << stop.getStopSite() << "|"
            << std::setw(12) << std::fixed << std::setprecision(6) << stop.getLatitude() << "|"
            << std::setw(12) << std::fixed << std::setprecision(6) << stop.getLongitude() << "|"
            << std::setw(7) << stop.getSysCode() << "|"
            << std::setw(40) << stop.getSystem() << "|"
            << std::setw(20) << stop.getMunicipality() << std::endl;
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
