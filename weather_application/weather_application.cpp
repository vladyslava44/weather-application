#include <iostream>
#include <vector>
#include "WeatherApiClient.h"
#include "Location.h"
#include "ForecastManager.h"
#include "WeatherSettings.h"

int main() {
    std::vector<Location> locations;

    locations.emplace_back("Nottingham", 52.9540, 1.1550);
    locations.emplace_back("Irpin", 50.5188, 30.2398);
    locations.emplace_back( "New York", 40.7128, -74.0060); 

    displayMainMenu(locations); 

    return 0;
}

