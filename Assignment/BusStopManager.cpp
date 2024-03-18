#include "BusStopManager.h"

#include <fstream>

void BusStopManager::loadFromCSV(const std::string& filename)
{
	std::ifstream file(filename);
	std::string line;
	std::getline(file, line); // skip header line

	while (std::getline(file, line))
	{
		//stopid; stopname; stopsite; latitude; longitude; syscode; system; municipality
		std::stringstream s(line);
		std::string stopid;
		std::string stopname;
		std::string stopsite;
		double latitude;
		double longitude;
		std::string syscode;
		std::string system;
		std::string municipality;
	}
}
