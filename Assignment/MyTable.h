#pragma once

#include <libds/adt/table.h>
#include <libds/amt/implicit_sequence.h>
#include "BusStop.h"

using CompositeKey = std::tuple<std::string, std::string>;

class MyTable : public ds::adt::SortedSequenceTable<CompositeKey, BusStop> {
public:
    MyTable() : ds::adt::SortedSequenceTable<CompositeKey, BusStop>() {}
    MyTable(const MyTable& other) : ds::adt::SortedSequenceTable<CompositeKey, BusStop>(other) {}
};
