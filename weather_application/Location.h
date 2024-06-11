#ifndef LOCATION_H
#define LOCATION_H

#include <string>
#include <vector> 
struct Location {
    static int nextId;
    int id;
    std::string name;
    double latitude;
    double longitude;
    bool isFavorite;

    Location(const std::string& _name, double _latitude, double _longitude);
};
void manageLocations(std::vector<Location>& locations);
void displayWeatherMenu(const std::vector<Location>& locations);
void addNewLocation(std::vector<Location>& locations);
void listLocations(const std::vector<Location>& locations);
void modifyLocation(std::vector<Location>& locations);
void removeLocation(std::vector<Location>& locations);
void setFavoriteLocation(std::vector<Location>& locations);
void searchLocations(const std::vector<Location>& locations);
std::string toLower(const std::string& str);
Location getLocationById(const std::vector<Location>& locations, int locationId);

#endif 
