#include "Application.h"
#include <iostream>
#include <iomanip>
#include <limits>

#ifdef _DEBUG
#include <crtdbg.h>
#endif

#include <Windows.h>
#undef max

std::vector<std::string> csvFiles = {
	"cow_busstops.csv",
    "diakritika_busstops.csv",
    "kam_busstops.csv",
    "nan_busstops.csv",
    "vic_busstops.csv",
    "vly_busstops.csv",
    "whi_busstops.csv",
    "wil_busstops.csv",
    "wkt_busstops.csv",
};

void Application::run() {
    SetConsoleCP(1250);
    SetConsoleOutputCP(1250);

    std::cout << "=====================================\n"
        << "   Welcome to the Bus Stop Manager   \n"
        << "=====================================\n";

    manager.loadAllCSVs(csvFiles);
    chooseCSVFile();
    applyFilter();

    std::cout << "\nThank you for using Bus Stop Manager!\n"
        << "Program terminated.\n";
}

void Application::chooseCSVFile() {
    std::cout << "Please choose a CSV file to work with by entering the corresponding number:\n";
    for (size_t i = 0; i < csvFiles.size(); ++i) {
        std::cout << i + 1 << ". " << csvFiles[i] << "\n";
    }

    size_t fileIndex;
    if (!(std::cin >> fileIndex) || fileIndex < 1 || fileIndex > csvFiles.size()) {
        std::cin.clear(); // Clear the error flag
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard the rest of the line
        std::cerr << "Invalid file number.\nExiting...\n";
        exit(1);
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard the rest of the line
    manager.setCurrentBusStops(csvFiles[fileIndex - 1]);
    // Display a message about the selected file
    std::cout << "Selected: " << csvFiles[fileIndex - 1] << "\n"
              << "Loaded bus stops: " << manager.getBusStopCount() << "\n\n";

    char printChoice;
    std::cout << "Do you want to print detailed information for the selected file? (Y/N): ";
    std::cin >> printChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    manager.setDisplayDetails(printChoice == 'Y' || printChoice == 'y');

    if (manager.getDisplayDetails()) {
        manager.printCurrentBusStopsDetails();
    }
}

void Application::applyFilter() {
    bool filtering = true;

    while (filtering) {
        std::cout << "+-----------------------------------+\n"
                  << "|           Filter Menu             |\n"
                  << "+-----------------------------------+\n"
                  << "| 1. Filter by 'starts with'        |\n"
                  << "| 2. Filter by 'contains'           |\n"
                  << "| 3. Select a different file        |\n"
                  << "| 4. Exit                           |\n"
                  << "+-----------------------------------+\n"
                  << "Enter your choice: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1:
            case 2:
                handleFiltering(choice);
                break;
            case 3:
                chooseCSVFile();
                break;
            case 4:
                filtering = false;
                break;
            default:
                std::cout << "Invalid choice. Please enter a number from 1 to 4.\n";
                break;
        }
    }
}

void Application::handleFiltering(int choice) {
    std::string filterStr;
    std::cout << "Enter a string to filter bus stops that "
        << (choice == 1 ? "start with: " : "contain: ");
    std::getline(std::cin, filterStr);

    displayFilterResults(manager.filterBusStops([&filterStr, choice](const BusStop& stop) {
        return choice == 1
            ? stop.getName().substr(0, filterStr.size()) == filterStr
            : stop.getName().find(filterStr) != std::string::npos;
        }));
}

void Application::displayFilterResults(const std::vector<BusStop>& stops) {
    if (manager.getDisplayDetails()) {
        // Display detailed information if enabled
        for (const auto& stop : stops) {
            std::cout << "• " << stop.getName() << " at " << stop.getMunicipality() << "\n";
        }
    }
    else {
        // Display basic information if detail display is disabled
        for (const auto& stop : stops) {
            std::cout << "• " << stop.getName() << "\n";
        }
    }

    std::cout << "\nFiltering complete. Found " << stops.size() << " results.\n";
}