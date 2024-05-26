#include "Municipality.h"

Municipality::Municipality(const std::string& name) : name(name) {}

void Municipality::addBusStop(const BusStop& busStop) {
    busStops.push_back(busStop);
}

const std::vector<BusStop>& Municipality::getBusStops() const {
    return busStops;
}

const std::string& Municipality::getName() const {
    return name;
}
