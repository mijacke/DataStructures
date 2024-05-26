#pragma once

#include "BusStop.h"
#include <libds/amt/explicit_hierarchy.h>
#include <libds/adt/table.h>
#include "MyTable.h"

#include <functional> 
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

using CompositeKey = std::tuple<std::string, std::string>;

struct TransitNode {
    std::string name; // Operator or municipality name
    std::vector<BusStop> busStops;
    ds::amt::ExplicitHierarchyBlock<TransitNode>* parent;
    std::vector<ds::amt::ExplicitHierarchyBlock<TransitNode>*> children;

    TransitNode() : parent(nullptr) {}

    bool operator==(const TransitNode& other) const {
        return name == other.name;
    }
};

class BusStopManager
{
private:
	std::unordered_map<std::string, std::vector<BusStop>> busStopsData; // Iba pomocna, Key: filename, Value: BusStop vector
    std::unordered_map<std::string, std::vector<BusStop>> operatorBusStopsCache; // Iba pomocna
    std::vector<BusStop> currentBusStops; 
	bool displayDetails = true;

    ds::amt::MultiWayExplicitHierarchy<TransitNode> hierarchy; // Uroven 2
    MyTable sortedBusStops; // Uroven 3

public:
    BusStopManager() {};

    void findUsingName(const std::string& name) {
        std::vector<BusStop> foundBusStops;
        for (const auto& busStop : currentBusStops) {
            if (busStop.getName() == name) {
                foundBusStops.push_back(busStop);
            }
        }
        currentBusStops = foundBusStops;
        if (currentBusStops.empty()) {
            std::cout << "No bus stop found with name: " << name << std::endl;
        }
        else {
            for (const auto& busStop : currentBusStops) {
                std::cout << "Bus stop found: " << busStop.getStopId() << "; "
                    << busStop.getName() << "; " << busStop.getStopSite() << "; "
                    << busStop.getLatitude() << "; " << busStop.getLongitude() << "; "
                    << busStop.getSysCode() << "; " << busStop.getSystem() << "; "
                    << busStop.getMunicipality() << std::endl;
            }
        }
    }

    void loadFromVectorIntoExplicitHierarchy() {
        std::cout << "Loading data into explicit hierarchy and Table..." << std::endl;
        hierarchy.clear();
        sortedBusStops.clear();
        auto& rootBlock = hierarchy.emplaceRoot();
        rootBlock.data_.name = "Root";

        for (const auto& operatorPair : busStopsData) {
            auto& transitOperatorBlock = hierarchy.emplaceSon(rootBlock, hierarchy.degree(rootBlock));
            transitOperatorBlock.data_.name = operatorPair.second.at(0).getSystem();

            std::vector<BusStop>& operatorStops = operatorBusStopsCache[transitOperatorBlock.data_.name];

            for (const auto& busStop : operatorPair.second) {
                std::string municipalityName = busStop.getMunicipality();
                auto& municipalityBlock = ensureMunicipalityNode(transitOperatorBlock, municipalityName);
                municipalityBlock.data_.busStops.push_back(busStop);

                operatorStops.push_back(busStop);

                CompositeKey key = std::make_tuple(busStop.getStopId(), busStop.getName());
                try {
                    sortedBusStops.insert(key, busStop);
                }
                catch (const std::exception& e) {
                    std::cerr << "Error inserting bus stop into sortedBusStops: " << e.what() << std::endl;
                    continue;
                }
            }
        }
    }

    ds::amt::MWEHBlock<TransitNode>& ensureMunicipalityNode(ds::amt::MWEHBlock<TransitNode>& operatorBlock, const std::string& name) {
        static std::map<std::string, ds::amt::MWEHBlock<TransitNode>*> municipalityCache;
        if (municipalityCache.find(name) == municipalityCache.end()) {
            auto& municipalityBlock = hierarchy.emplaceSon(operatorBlock, hierarchy.degree(operatorBlock));
            municipalityBlock.data_.name = name;
            municipalityCache[name] = &municipalityBlock;
            return municipalityBlock;
        }
        return *municipalityCache[name];
    }

    void setCurrentBusStopsBasedOnNode(const TransitNode& node) {
        if (node.children.empty()) { // Municipality
            currentBusStops = node.busStops;
        }
         // Operator
            auto it = operatorBusStopsCache.find(node.name);
            if (it != operatorBusStopsCache.end()) {
                currentBusStops = it->second;
            }
            else {
                std::cerr << "No cached bus stops found for operator: " << node.name << std::endl;
            }
        
    }

    void loadBusStopsForOperator(const TransitNode& node) {
        std::vector<BusStop> aggregatedBusStops;
        for (auto child : node.children) {
            aggregatedBusStops.insert(aggregatedBusStops.end(), child->data_.busStops.begin(), child->data_.busStops.end());
        }
        operatorBusStopsCache[node.name] = aggregatedBusStops;
        currentBusStops = aggregatedBusStops;
        std::cout << "Loaded " << currentBusStops.size() << " bus stops for operator: " << node.name << std::endl;
    }


    ds::amt::ExplicitHierarchyBlock<TransitNode>* getHierarchyRoot() {
        if (hierarchy.isEmpty()) {
            return nullptr;
        }
        return hierarchy.accessRoot();
    }

    void loadAllCSVs(const std::vector<std::string>& filenames)
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
                    BusStop stop(fields[0], fields[1], fields[2], std::stod(fields[3]), std::stod(fields[4]), fields[5], fields[6], fields[7]);
                    stops.push_back(stop); 
                }
                else {
                    std::cerr << "Error: Line does not have 8 fields as expected." << std::endl;
                }
            }
            file.close();
            busStopsData[filename] = stops;
        }

        loadFromVectorIntoExplicitHierarchy();
    }

    void setDisplayDetails(bool choice)
    {
        displayDetails = choice;
    }

    bool getDisplayDetails() const { return displayDetails; }

    void printCurrentBusStopsDetails() const {
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

    void setCurrentBusStops(const std::string& filename)
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
        
public:

    std::vector<BusStop> filterBusStops(std::function<bool(const BusStop&)> predicate) const {
        std::vector<BusStop> filtered;
        for (auto it = currentBusStops.begin(); it != currentBusStops.end(); ++it) {
            if (predicate(*it)) {
                filtered.push_back(*it);
            }
        }
        return filtered;
    }


    size_t getBusStopCount() const { return currentBusStops.size(); }

    std::vector<std::string> getCSVFileNames() const
    {
        std::vector<std::string> filenames;
        for (const auto& pair : busStopsData) {
            filenames.push_back(pair.first);
        }
        return filenames;
    }
};
