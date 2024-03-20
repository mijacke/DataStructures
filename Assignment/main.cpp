#include "BusStopManager.h"
#include <iostream>
#include <string>
#include <limits>
#include <Windows.h>
#undef max

int main() {
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    SetConsoleCP(1250);
    SetConsoleOutputCP(1250);

    BusStopManager manager;
    std::string path;
    bool loadSuccessful = false;
    while (!loadSuccessful) {
        std::cout << "Enter the path to the CSV file: ";
        std::getline(std::cin, path);

        manager.loadFromCSV(path);

        if (manager.getBusStopCount() > 0) {
            loadSuccessful = true;
            std::cout << "\nBus stops loaded: " << manager.getBusStopCount() << std::endl;
        }
        else {
            std::cerr << "Failed to load bus stops from the file.\n";
            char choice;
            std::cout << "Would you like to try again? (Y/N): ";
            std::cin >> choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer
            if (choice != 'Y' && choice != 'y') {
                return 1; // Exit the program if the user does not want to try again
            }
        }
    }

    std::string filterStr;
    std::vector<BusStop> filteredStops;
    bool filtering = true;

    while (filtering) {
        std::cout << "Choose a filter to apply:\n";
        std::cout << "1. Filter by starts with\n";
        std::cout << "2. Filter by contains\n";
        std::cout << "3. Select a different file\n";
        std::cout << "4. Exit\n";
        std::cout << "Enter your choice: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer

        switch (choice) {
        case 1: {
            std::cout << "Enter a string to filter bus stops that start with: ";
            std::getline(std::cin, filterStr);
            filteredStops = manager.filterBusStops([&filterStr](const BusStop& stop) {
                return stop.getName().substr(0, filterStr.size()) == filterStr;
                });
            break;
        }
        case 2: {
            std::cout << "Enter a string to filter bus stops that contain: ";
            std::getline(std::cin, filterStr);
            filteredStops = manager.filterBusStops([&filterStr](const BusStop& stop) {
                return stop.getName().find(filterStr) != std::string::npos;
                });
            break;
        }
        case 3:
            std::cout << "Enter the path to the CSV file: ";
            std::getline(std::cin, path);
            manager.loadFromCSV(path);
            continue;
        case 4:
            filtering = false;
            continue;
        default:
            std::cout << "Invalid choice.\n";
            continue;
        }

        for (const BusStop& stop : filteredStops) {
            std::cout << stop.getName() << " at " << stop.getMunicipality() << std::endl;
        }
        std::cout << "\nFiltering complete.\n";
        filteredStops.clear(); // Clear the vector for the next filtering operation
    }

    std::cout << "Program terminated." << std::endl;
    return 0;
}
