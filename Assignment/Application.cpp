#include "Application.h"
#include <iostream>
#include <limits>

#ifdef _DEBUG
#include <crtdbg.h>
#endif

#include <Windows.h>
#undef max // Ensure that the max macro does not interfere with std::numeric_limits

void Application::run() {
    SetConsoleCP(1250);
    SetConsoleOutputCP(1250);

    loadBusStops();
    applyFilter();

    std::cout << "Program terminated." << std::endl;
}

void Application::loadBusStops() {
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
            if (!promptRetry()) {
                exit(1); // Exit the program if the user does not want to try again
            }
        }
    }
}

bool Application::promptRetry() {
    char choice;
    std::cout << "Would you like to try again? (Y/N): ";
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer
    return (choice == 'Y' || choice == 'y');
}

void Application::applyFilter() {
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
    }
}
