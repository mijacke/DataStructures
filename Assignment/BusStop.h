#pragma once

#include <string>

//stopid;stopname;stopsite;latitude;longitude;syscode;system;municipality
class BusStop
{
private:
	std::string stopId;
	std::string name;
	std::string stopSite;
	double latitude;
	double longitude;
	std::string sysCode;
	std::string system;
	std::string municipality;

public:
	BusStop(const std::string& stopId, const std::string& name, const std::string& stopSite,
		double latitude, double longitude, const std::string& sysCode,
		const std::string& system, const std::string& municipality);
	~BusStop();

	const std::string& getStopId() const;
	const std::string& getName() const;
	const std::string& getStopSite() const;
	double getLatitude() const;
	double getLongitude() const;
	const std::string& getSysCode() const;
	const std::string& getSystem() const;
	const std::string& getMunicipality() const;
};

