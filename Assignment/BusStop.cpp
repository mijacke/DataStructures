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
	// TODO: insert return statement here
}

const std::string& BusStop::getName() const
{
	// TODO: insert return statement here
}

const std::string& BusStop::getStopSite() const
{
	// TODO: insert return statement here
}

double BusStop::getLatitude() const
{
	return 0.0;
}

double BusStop::getLongitude() const
{
	return 0.0;
}

const std::string& BusStop::getSysCode() const
{
	// TODO: insert return statement here
}

const std::string& BusStop::getSystem() const
{
	// TODO: insert return statement here
}

const std::string& BusStop::getMunicipality() const
{
	// TODO: insert return statement here
}


