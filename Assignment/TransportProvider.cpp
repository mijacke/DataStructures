#include "TransportProvider.h"

TransportProvider::TransportProvider(const std::string& name) : name(name) {}

void TransportProvider::addMunicipality(const Municipality& municipality) {
    municipalities.push_back(municipality);
}

const std::vector<Municipality>& TransportProvider::getMunicipalities() const {
    return municipalities;
}

const std::string& TransportProvider::getName() const {
    return name;
}
