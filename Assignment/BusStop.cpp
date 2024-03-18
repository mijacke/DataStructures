#include "BusStop.h"

BusStop::BusStop(const std::string& stopId, const std::string& name, const std::string& stopSite,
	double latitude, double longitude, const std::string& sysCode, 
	const std::string& system, const std::string& municipality) 
	: stopId(stopId), name(name), stopSite(stopSite), latitude(latitude), longitude(longitude), 
	sysCode(sysCode), system(system), municipality(municipality) {}

BusStop::~BusStop()
{
}

const std::string& BusStop::getStopId() const
{
	return stopId;
}

const std::string& BusStop::getName() const
{
	return name;
}

const std::string& BusStop::getStopSite() const
{
	return stopSite;
}

double BusStop::getLatitude() const
{
	return latitude;
}

double BusStop::getLongitude() const
{
	return longitude;
}

const std::string& BusStop::getSysCode() const
{
	return sysCode;
}

const std::string& BusStop::getSystem() const
{
	return system;
}

const std::string& BusStop::getMunicipality() const
{
	return municipality;
}


