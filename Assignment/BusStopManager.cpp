#include "BusStopManager.h"

#include <fstream>
#include <sstream> // for std::stringstream

void BusStopManager::loadFromCSV(const std::string& filename)
{
	std::ifstream file(filename);
	std::string line;
	std::getline(file, line); // skip header line

	while (std::getline(file, line))
	{
		//stopid; stopname; stopsite; latitude; longitude; syscode; system; municipality
		std::stringstream s(line);
		std::string cell; // skiped header line
		std::string stopid;
		std::string stopname;
		std::string stopsite;
		double latitude;
		double longitude;
		std::string syscode;
		std::string system;
		std::string municipality;

		std::getline(s, cell, ';'); // skiped header line
		std::getline(s, stopid, ';');
		std::getline(s, stopname, ';');
		std::getline(s, stopsite, ';');
		s >> latitude; s.ignore(); // s.ignore() to skip the separator
		s >> longitude; s.ignore(); // s.ignore() to skip the separator
		std::getline(s, syscode, ';');
		std::getline(s, system, ';');
		std::getline(s, municipality, ';');

		busStops.emplace_back(stopid, stopname, stopsite, latitude, longitude, syscode, system, municipality);
	}
	file.close();
}
