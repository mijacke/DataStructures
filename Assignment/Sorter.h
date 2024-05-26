#pragma once
#include <vector>
#include <functional>
#include "BusStop.h"

class Sorter {
public:
    void sort(std::vector<BusStop>& data, std::function<bool(const BusStop&, const BusStop&)> comparator);
};
