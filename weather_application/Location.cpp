#include "Location.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <limits> 
#include <stdexcept> 

int Location::nextId = 1;

Location::Location(const std::string& _name, double _latitude, double _longitude)
    : id(nextId++), name(_name), latitude(_latitude), longitude(_longitude), isFavorite(false) {}

Location getLocationById(const std::vector<Location>& locations, int locationId) {
    auto it = std::find_if(locations.begin(), locations.end(),
        [locationId](const Location& loc) { return loc.id == locationId; });

    if (it != locations.end()) {
        return *it; 
    }
    else {
        throw std::runtime_error("Location ID not found");
    }
}

void addNewLocation(std::vector<Location>& locations) {
    std::string name;
    double latitude, longitude;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 

    try {
        std::cout << "Enter Location Name: ";
        std::getline(std::cin, name);

        std::cout << "Enter Latitude: ";
        std::cin >> latitude;
        if (std::cin.fail()) {
            throw std::invalid_argument("Invalid input for Latitude.");
        }

        std::cout << "Enter Longitude: ";
        std::cin >> longitude;
        if (std::cin.fail()) {
            throw std::invalid_argument("Invalid input for Longitude.");
        }

        locations.emplace_back(name, latitude, longitude);
        std::cout << "New location added successfully! (ID: " << locations.back().id << ")" << std::endl;
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << " Please try again." << std::endl;
        std::cin.clear(); 
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
    }
}

void listLocations(const std::vector<Location>& locations) {
    std::cout << "List of Locations:" << std::endl;
    for (const Location& loc : locations) {
        std::cout << "ID: " << loc.id << ", Name: " << loc.name << ", Latitude: " << loc.latitude
            << ", Longitude: " << loc.longitude;
        if (loc.isFavorite) {
            std::cout << " (Favorite)";
        }
        std::cout << std::endl;
    }
}

void modifyLocation(std::vector<Location>& locations) {
    std::string name;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 

    std::cout << "Enter Location ID or Name to modify: ";
    std::getline(std::cin, name);

    auto it = std::find_if(locations.begin(), locations.end(), [&](const Location& loc) {
        return (std::to_string(loc.id) == name || loc.name == name);
        });

    if (it != locations.end()) {
        Location& location = *it;

        try {
            std::cout << "Enter new Location Name: ";
            std::getline(std::cin, location.name);

            std::cout << "Enter new Latitude: ";
            std::cin >> location.latitude;
            if (std::cin.fail()) {
                throw std::invalid_argument("Invalid input for Latitude.");
            }

            std::cout << "Enter new Longitude: ";
            std::cin >> location.longitude;
            if (std::cin.fail()) {
                throw std::invalid_argument("Invalid input for Longitude.");
            }

            std::cout << "Location updated successfully!" << std::endl;
        }
        catch (const std::invalid_argument& e) {
            std::cerr << "Error: " << e.what() << " Modification failed." << std::endl;
            std::cin.clear(); 
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
        }
    }
    else {
        std::cout << "Location not found." << std::endl;
    }
}

void removeLocation(std::vector<Location>& locations) {
    std::string name;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 

    std::cout << "Enter Location Name or ID to remove: ";
    std::getline(std::cin, name);

    auto it = std::find_if(locations.begin(), locations.end(), [&](const Location& loc) {
        return (std::to_string(loc.id) == name || loc.name == name);
        });

    if (it != locations.end()) {
        locations.erase(it);
        std::cout << "Location removed successfully!" << std::endl;
    }
    else {
        std::cout << "Location not found." << std::endl;
    }
}

void setFavoriteLocation(std::vector<Location>& locations) {
    std::string name;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 

    std::cout << "Enter Location Name or ID to set as favorite: ";
    std::getline(std::cin, name);

    auto it = std::find_if(locations.begin(), locations.end(), [&](const Location& loc) {
        return (std::to_string(loc.id) == name || loc.name == name);
        });

    if (it != locations.end()) {
        Location& location = *it;
        location.isFavorite = true;
        std::cout << "Location set as favorite!" << std::endl;
    }
    else {
        std::cout << "Location not found." << std::endl;
    }
}

void searchLocations(const std::vector<Location>& locations) {
    std::string query;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 

    std::cout << "Enter Location Name or ID to search: ";
    std::getline(std::cin, query);

    std::string lowercaseQuery = toLower(query);

    auto it = std::find_if(locations.begin(), locations.end(), [&](const Location& loc) {
        std::string lowercaseName = toLower(loc.name);
        return (std::to_string(loc.id) == lowercaseQuery || lowercaseName == lowercaseQuery ||
            lowercaseName.find(lowercaseQuery) != std::string::npos);
        });

    if (it != locations.end()) {
        const Location& location = *it;
        std::cout << "Location found:" << std::endl;
        std::cout << "ID: " << location.id << ", Name: " << location.name << ", Latitude: " << location.latitude
            << ", Longitude: " << location.longitude << std::endl;
    }
    else {
        std::cout << "Location not found." << std::endl;
    }
}

std::string toLower(const std::string& str) {
    std::string result;
    result.reserve(str.size());
    for (char c : str) {
        result += std::tolower(static_cast<unsigned char>(c));
    }
    return result;
}

void manageLocations(std::vector<Location>& locations) {
    int choice;
    while (true) {
        std::cout << "\nManage Locations" << std::endl;
        std::cout << "1. Add New Location" << std::endl;
        std::cout << "2. Modify Location" << std::endl;
        std::cout << "3. Remove Location" << std::endl;
        std::cout << "4. Search for Location" << std::endl;
        std::cout << "5. Set Favorite Location" << std::endl;
        std::cout << "6. List Locations" << std::endl;
        std::cout << "7. Back to Main Menu" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            addNewLocation(locations);
            break;
        case 2:
            modifyLocation(locations);
            break;
        case 3:
            removeLocation(locations);
            break;
        case 4:
            searchLocations(locations);
            break;
        case 5:
            setFavoriteLocation(locations);
            break;
        case 6:
            listLocations(locations);
            break;
        case 7:
            std::cout << "Returning to Main Menu..." << std::endl;
            return;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }
}
