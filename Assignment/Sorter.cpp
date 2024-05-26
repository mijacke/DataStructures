#include "Sorter.h"
#include <algorithm>

void Sorter::sort(std::vector<BusStop>& data, std::function<bool(const BusStop&, const BusStop&)> comparator) {
    std::sort(data.begin(), data.end(), comparator);
}
