#include "BusStop.h"

BusStop::BusStop(const std::string& stopId, const std::string& name, const std::string& stopSite,
	double latitude, double longitude, const std::string& sysCode, 
	const std::string& system, const std::string& municipality) 
	: stopId(stopId), name(name), stopSite(stopSite), latitude(latitude), longitude(longitude), 
	sysCode(sysCode), system(system), municipality(municipality) {}


