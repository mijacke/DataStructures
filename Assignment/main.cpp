#include "BusStopManager.h"
#include <iostream>
#include <filesystem>
#include <Windows.h>
#undef max
#include <cstdio>

int main()
{
	#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// Optional: _CrtSetBreakAlloc(x); where x is the allocation number reported as a leak
	#endif
	SetConsoleCP(1250);
	SetConsoleOutputCP(1250);

	std::string path = ""; // Path to the CSV file
	std::cout << "Path to CSV file: " << path << std::endl;
	std::cin >> path;

	std::cout << "Loading bus stops from CSV...\n" << std::endl;
	BusStopManager manager;
	manager.loadFromCSV(path);

	std::string startsWithStr = "";
	std::string containsStr = "";
	//std::string endsWithStr = "";

	//std::cout << "Enter a string to filter bus stops that starts with: ";
	//std::cin >> startsWithStr;
	//std::cout << "Enter a string to filter bus stops that contain: ";
	//std::cin >> containsStr;

	// Check how many bus stops have been loaded
	std::cout << "\nBus stops loaded: " << manager.getBusStopCount() << std::endl;

	std::cout << "Choose a filter to apply:\n";
	std::cout << "1. Filter by starts with\n";
	std::cout << "2. Filter by contains\n";
	std::cout << "Enter your choice: ";

	int choice;
	std::cin >> choice;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer
	std::cout << "\n";

	std::string filterStr;
	std::vector<BusStop> filteredStops;

	switch (choice) {
	case 1: {
		std::cout << "Enter a string to filter bus stops that start with: ";
		std::getline(std::cin, filterStr);

		filteredStops = manager.filterBusStops([&filterStr](const BusStop& stop) {
			return stop.getName().substr(0, filterStr.size()) == filterStr;
			});

		system("cls"); // Clear the console
		std::cout << "Filtering bus stops that start with '" << filterStr << "':\n";
		break;
	}
	case 2: {
		std::cout << "Enter a string to filter bus stops that contain: ";
		std::getline(std::cin, filterStr);

		filteredStops = manager.filterBusStops([&filterStr](const BusStop& stop) {
			return stop.getName().find(filterStr) != std::string::npos;
			});

		system("cls"); // Clear the console
		std::cout << "Filtering bus stops that contain '" << filterStr << "':\n";
		break;
	}
	default:
		std::cout << "Invalid choice.\n";
		return 1;
	}

	for (const BusStop& stop : filteredStops) {
		std::cout << stop.getName() << " at " << stop.getMunicipality() << std::endl;
	}

	std::cout << "\nFinished filtering bus stops." << std::endl; // Need to implement to go again.
	return 0;
}