#include "Application.h"
#include <iostream>
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

    std::cout << "=====================================\n";
    std::cout << "   Welcome to the Bus Stop Manager   \n";
    std::cout << "=====================================\n";

    // Preload all CSV data into memory.
    manager.loadAllCSVs(csvFiles);

    // Ask the user if they want to display bus stop details.
    char detailChoice;
    std::cout << "Do you want to display bus stop details? (Y/N): ";
    std::cin >> detailChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    manager.setDisplayDetails(detailChoice == 'Y' || detailChoice == 'y');

    chooseCSVFile(); // Let the user select which CSV file to work with initially.
    applyFilter();

    std::cout << "\nThank you for using Bus Stop Manager!\n";
    std::cout << "Program terminated.\n";
}

void Application::chooseCSVFile() {
    std::cout << "Please choose a CSV file to work with by entering the corresponding number:\n";
    for (size_t i = 0; i < csvFiles.size(); ++i) {
        std::cout << i + 1 << ". " << csvFiles[i] << "\n";
    }

    size_t fileIndex;
    std::cin >> fileIndex;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (fileIndex > 0 && fileIndex <= csvFiles.size()) {
        manager.setCurrentBusStops(csvFiles[fileIndex - 1]);
    }
    else {
        std::cerr << "Invalid file number.\n";
        exit(1); // or you could loop back and ask again
    }
    // Display a message about the selected file
    std::cout << "Selected: " << csvFiles[fileIndex - 1] << "\n";
    std::cout << "Loaded bus stops: " << manager.getBusStopCount() << "\n";

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
                chooseCSVFile();
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