#include "Application.h"
#include <iostream>
#include <limits>

#ifdef _DEBUG
#include <crtdbg.h>
#endif

#include <Windows.h>
#undef max

void Application::run() {
    SetConsoleCP(1250);
    SetConsoleOutputCP(1250);

    std::cout << "=====================================\n";
    std::cout << "   Welcome to the Bus Stop Manager   \n";
    std::cout << "=====================================\n";

    // Ask the user if they want to display bus stop details
    char detailChoice;
    std::cout << "Do you want to display bus stop details? (Y/N): ";
    std::cin >> detailChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    manager.setDisplayDetails(detailChoice == 'Y' || detailChoice == 'y');

    loadBusStops();
    applyFilter();

    std::cout << "\nThank you for using Bus Stop Manager!\n";
    std::cout << "Program terminated.\n";
}


void Application::loadBusStops() {
    std::string path;
    bool loadSuccessful = false;
    while (!loadSuccessful) {
        std::cout << "Enter the path to the CSV file: ";
        std::getline(std::cin, path);
        manager.loadFromCSV(path);
        loadSuccessful = manager.getBusStopCount() > 0;

        if (loadSuccessful) {
            std::cout << "\nBus stops loaded successfully! Total: " << manager.getBusStopCount() << "\n";
        }
        else {
            std::cerr << "Failed to load bus stops from the file. Do you want to try again? (Y/N): ";
            char retryChoice;
            std::cin >> retryChoice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (retryChoice != 'Y' && retryChoice != 'y') {
                exit(1);
            }
        }
    }
}

void Application::applyFilter() {
    bool filtering = true;

    while (filtering) {
        std::cout << "\n+-----------------------------------+\n";
        std::cout << "|           Filter Menu             |\n";
        std::cout << "+-----------------------------------+\n";
        std::cout << "| 1. Filter by 'starts with'        |\n";
        std::cout << "| 2. Filter by 'contains'           |\n";
        std::cout << "| 3. Select a different file        |\n";
        std::cout << "| 4. Toggle display bus stop details|\n";
        std::cout << "| 5. Exit                           |\n";
        std::cout << "+-----------------------------------+\n";
        std::cout << "Enter your choice: ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number from 1 to 5.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::string filterStr;
        std::vector<BusStop> filteredStops;

        switch (choice) {
            case 1: {
                std::cout << "Enter a string to filter bus stops that start with: ";
                std::getline(std::cin, filterStr);
                displayFilterResults(manager.filterBusStops(
                    [&filterStr](const BusStop& stop) { return stop.getName().substr(0, filterStr.size()) == filterStr; }
                ));
                break;
            }
            case 2: {
                std::cout << "Enter a string to filter bus stops that contain: ";
                std::getline(std::cin, filterStr);
                displayFilterResults(manager.filterBusStops(
                    [&filterStr](const BusStop& stop) { return stop.getName().find(filterStr) != std::string::npos; }
                ));
                break;
            }
            case 3: {
                std::string path;
                std::cout << "\n=====[File Reload]=====\n";
                std::cout << "Enter the path to the new CSV file: ";
                std::getline(std::cin, path);
                manager.clearBusStops(); // Clear the current bus stops
                manager.loadFromCSV(path);
                if (manager.getBusStopCount() > 0) {
                    std::cout << "\nNew bus stops loaded: " << manager.getBusStopCount() << std::endl;
                }
                else {
                    std::cerr << "Failed to load bus stops from the new file.\n";
                }
                break;
            }
            case 4: {
                manager.setDisplayDetails(!manager.getDisplayDetails());
                std::cout << (manager.getDisplayDetails() ? "Bus stop details will be shown.\n" : "Bus stop details will be hidden.\n");
                break;
            }
            case 5: {
                filtering = false;
                break;
            }
            default: {
                std::cout << "Invalid choice. Please enter a number from 1 to 5.\n";
                break;
            }
        }
    }
}

void Application::displayFilterResults(const std::vector<BusStop>& stops) {
    if (!stops.empty()) {
        std::cout << "\n[Filtered Results]\n";
        for (const BusStop& stop : stops) {
            std::cout << "• " << stop.getName() << " at " << stop.getMunicipality() << std::endl;
        }
        std::cout << "\nFiltering complete.\n";
    }
    else {
        std::cout << "No results found for the given filter.\n";
    }
}