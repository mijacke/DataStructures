#include "Application.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include "Sorter.h"

#ifdef _DEBUG
#include <crtdbg.h>
#endif

#include <Windows.h>
#include "HierarchyIterator.h"
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

    std::cout << "=========================================================\n"
        << "|                                                       |\n"
        << "|            Welcome to the Bus Stop Manager            |\n"
        << "|                                                       |\n"
        << "=========================================================\n";

    manager.loadAllCSVs(csvFiles);

    bool running = true;
    while (running) {
    
        int levelChoice;
        std::cout << "Choose the hierarchy level for filtering:\n"
            << "1. Uroven 1 (Simple Filtering)\n"
            << "2. Uroven 2 (Advanced Filtering with Hierarchy Navigation)\n"
            << "3. Uroven 3 (Search by Stop Name and Operator)\n"
            << "4. Uroven 4 (Universal Sorting)\n"
            << "5. Exit\n"
            << "Enter your choice: ";
        std::cin >> levelChoice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (levelChoice) {
        case 1:
            chooseCSVFile();
            applyFilter();
            break;
        case 2:
            applyFilterUroven2();
            break;
        case 3:
            applyFilterUroven3();
            break;
        case 4:
            applyFilterUroven4();
			break;
        case 5:
            std::cout << "\nExiting Bus Stop Manager. Thank you for using the application!\n";
            running = false;
            break;
        default:
            std::cerr << "Invalid choice. Please try again.\n";
            break;
        }
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
            << "| 4. Return to Urovne               |\n"
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

void Application::applyFilterUroven2() {
    HierarchyIterator<TransitNode> it(manager.getHierarchyRoot());

    int choice;
    while (true) {
        std::cout << "+-----------------------------------+\n"
            << "|     Uroven 2 Filter Navigation    |\n"
            << "+-----------------------------------+\n"
            << "| 1. Move to parent                 |\n"
            << "| 2. Move to child by index         |\n"
            << "| 3. Apply filter to current node   |\n"
            << "| 4. Return to Urovne               |\n"
            << "+-----------------------------------+\n"
            << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
        case 1:
            if (it.moveToParent()) {
                std::cout << "Moved to parent node: " << it->name << std::endl;
                manager.setCurrentBusStopsBasedOnNode(*it);
            }
            else {
                std::cout << "No parent to move to or already at root node." << std::endl;
            }
            break;
        case 2: {
            if (!it.isValid()) {
                std::cout << "Current node is null.\n";
                break;
            }

            auto* multiNode = static_cast<ds::amt::MultiWayExplicitHierarchyBlock<TransitNode>*>(it.getCurrentNode());
            for (size_t i = 0; i < multiNode->sons_->size(); ++i) {
                auto childNode = multiNode->sons_->access(i);
                if (childNode && childNode->data_) {
                    std::cout << i + 1 << ": " << childNode->data_->data_.name << std::endl;
                }
            }

            size_t index;
            std::cout << "Enter index of child to move to: ";
            std::cin >> index;
            if (!it.moveToChild(index - 1)) {
                std::cout << "Invalid child index.\n";
            }
            else {
                std::cout << "Moved to child: " << it->name << "\n";
                if (it.getCurrentNode()->parent_ && it.getCurrentNode()->parent_->parent_) {
                    std::cout << "Bus stops in " << it->name << ":" << std::endl;
                    for (auto& busStop : it->busStops) {
                        std::cout << "- " << busStop.getName() << " at " << busStop.getMunicipality() << std::endl;
                    }
                }

            }
            break;
        }
        case 3:
            if (it.isValid()) {
                manager.setCurrentBusStopsBasedOnNode(*it);
                std::cout << "Currently at node: " << it->name << " with " << manager.getBusStopCount() << " bus stops loaded.\n";

                int filterType;
                std::string filterStr;
                std::cout << "Choose filter type: (1) Starts with, (2) Contains: ";
                std::cin >> filterType;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Enter filter string: ";
                std::getline(std::cin, filterStr);

                std::vector<BusStop> results;
                if (filterType == 1) {
                    results = manager.filterBusStops([&](const BusStop& bs) { return bs.getName().substr(0, filterStr.size()) == filterStr; });
                }
                else if (filterType == 2) {
                    results = manager.filterBusStops([&](const BusStop& bs) { return bs.getName().find(filterStr) != std::string::npos; });
                }

                std::cout << "Found " << results.size() << " records matching the filter.\n";

                displayFilterResults(results);
            }
            else {
                std::cout << "Current node is not valid for filtering.\n";
            }
            break;
        case 4:
            return;  // Exit Uroven 2 and return to main menu
        default:
            std::cout << "Invalid option. Please try again.\n";
            break;
        }
    }
}

void Application::applyFilterUroven3() {
    bool searching = true;
    while (searching) {
        int operatorChoice;
        std::cout << "Select an operator:\n";
        for (size_t i = 0; i < csvFiles.size(); ++i) {
            std::cout << i + 1 << ". " << csvFiles[i] << "\n"; // Začiatok od 1
        }
        std::cout << "Option: ";
        std::cin >> operatorChoice;

        if (operatorChoice >= 1 && operatorChoice <= csvFiles.size()) {
            manager.setCurrentBusStops(csvFiles[operatorChoice - 1]); // Začiatok od 1
        }
        else {
            std::cerr << "Invalid choice. Please try again.\n";
            continue;
        }

        std::cout << "+-----------------------------------+\n"
            << "|          Filter Menu              |\n"
            << "+-----------------------------------+\n"
            << "| 0. find using name                |\n"
            << "| 1. find contains                  |\n"
            << "| 2. find starts with               |\n"
            << "+-----------------------------------+\n"
            << "Option: ";
        int filterType;
        std::cin >> filterType;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::string filterStr;
        std::cout << "Enter filter string: ";
        std::getline(std::cin, filterStr);

        std::vector<BusStop> results;
        switch (filterType) {
        case 0:
            results = manager.filterBusStops([&](const BusStop& bs) { return bs.getName() == filterStr; });
            break;
        case 1:
            results = manager.filterBusStops([&](const BusStop& bs) { return bs.getName().find(filterStr) != std::string::npos; });
            break;
        case 2:
            results = manager.filterBusStops([&](const BusStop& bs) { return bs.getName().substr(0, filterStr.size()) == filterStr; });
            break;
        default:
            std::cerr << "Invalid filter type.\n";
            continue;
        }

        displayFilterResults(results);

        char repeatChoice;
        std::cout << "Do you want to perform another search? (Y/N): ";
        std::cin >> repeatChoice;
        if (repeatChoice != 'Y' && repeatChoice != 'y') {
            searching = false;
        }
    }
}

void Application::applyFilterUroven4() {
    HierarchyIterator<TransitNode> it(manager.getHierarchyRoot());
    if (!it.isValid()) {
        std::cerr << "Hierarchy root is invalid.\n";
        return;
    }

    bool selecting = true;
    while (selecting) {
        std::cout << "Select an operator:\n";
        for (size_t i = 0; i < csvFiles.size(); ++i) {
            std::cout << i + 1 << ". " << csvFiles[i] << "\n"; // Začiatok od 1
        }
        std::cout << "Option: ";
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice >= 1 && choice <= csvFiles.size()) {
            manager.setCurrentBusStops(csvFiles[choice - 1]); // Začiatok od 1
            selecting = false;
        } else {
            std::cerr << "Invalid choice. Please try again.\n";
        }
    }

    std::vector<BusStop> data = manager.filterBusStops([](const BusStop& bs) {
        return true;
    });

    auto compareAlphabetical = [](const BusStop& a, const BusStop& b) {
        return a.getName() < b.getName();
    };

    auto countConsonants = [](const std::string& s) {
        int count = 0;
        for (char c : s) {
            if (std::isalpha(c) && !std::strchr("AEIOUaeiou", c)) {
                ++count;
            }
        }
        return count;
    };

    auto compareConsonantCount = [countConsonants](const BusStop& a, const BusStop& b) {
        return countConsonants(a.getName()) < countConsonants(b.getName());
    };

    int comparatorChoice;
    std::cout << "Choose a comparator:\n"
              << "1. Alphabetical Order\n"
              << "2. Consonant Count\n"
              << "Option: ";
    std::cin >> comparatorChoice;

    std::function<bool(const BusStop&, const BusStop&)> comparator;
    switch (comparatorChoice) {
    case 1:
        comparator = compareAlphabetical;
        break;
    case 2:
        comparator = compareConsonantCount;
        break;
    default:
        std::cerr << "Invalid choice. Using alphabetical order as default.\n";
        comparator = compareAlphabetical;
        break;
    }

    Sorter sorter;
    sorter.sort(data, comparator);

    std::cout << "\nSorted bus stops:\n";
    for (const auto& stop : data) {
        std::string additionalInfo;
        if (comparatorChoice == 1) {
            additionalInfo = stop.getName();
        } else if (comparatorChoice == 2) {
            additionalInfo = std::to_string(countConsonants(stop.getName()));
        }

        std::cout << "Bus stop found: " << stop.getStopId() << "; "
                  << stop.getName() << "; " << stop.getStopSite() << "; "
                  << stop.getLatitude() << "; " << stop.getLongitude() << "; "
                  << stop.getSysCode() << "; " << stop.getSystem() << "; "
                  << stop.getMunicipality() << "; "
                  << "Comparator value: " << additionalInfo << "\n";
    }
}

void Application::chooseCSVFile() {
    std::cout << "Please choose a CSV file to work with by entering the corresponding number:\n";
    for (size_t i = 0; i < csvFiles.size(); ++i) {
        std::cout << i + 1 << ". " << csvFiles[i] << "\n"; // Začiatok od 1
    }

    size_t fileIndex;
    if (!(std::cin >> fileIndex) || fileIndex < 1 || fileIndex > csvFiles.size()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cerr << "Invalid file number. Please try again.\n";
        chooseCSVFile(); // Rekurzívny výber, kým sa nezadá správne číslo
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    manager.setCurrentBusStops(csvFiles[fileIndex - 1]); // Začiatok od 1
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

void Application::handleFiltering(int choice) {
    std::string filterStr;
    std::cout << "Enter a string to filter bus stops that "
        << (choice == 1 ? "start with: " : "contain: ");
    std::getline(std::cin, filterStr);

    std::vector<BusStop> results = manager.filterBusStops([&filterStr, choice](const BusStop& stop) {
        return choice == 1
                   ? stop.getName().substr(0, filterStr.size()) == filterStr
                   : stop.getName().find(filterStr) != std::string::npos;
    });

    displayFilterResults(results);
}

void Application::displayFilterResults(const std::vector<BusStop>& stops) {
    for (const auto& stop : stops) {
        std::cout << "Bus stop found: " << stop.getStopId() << "; "
            << stop.getName() << "; " << stop.getStopSite() << "; "
            << stop.getLatitude() << "; " << stop.getLongitude() << "; "
            << stop.getSysCode() << "; " << stop.getSystem() << "; "
            << stop.getMunicipality() << "\n";
    }

    std::cout << "\nFiltering complete. Found " << stops.size() << " results.\n";
}
