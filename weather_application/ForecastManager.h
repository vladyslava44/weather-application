#ifndef FORECAST_MANAGER_H
#define FORECAST_MANAGER_H

#include "Location.h"
#include "WeatherSettings.h"

void displayWeatherMenu(const std::vector<Location>& locations);
void searchWeatherForecast(const std::vector<Location>& locations, const WeatherSettings& settings);
void searchHistoricalWeather(const std::vector<Location>& locations, const WeatherSettings& settings);
void displayHistoricalWeatherDay(const std::vector<Location>& locations, const WeatherSettings& settings, int locationId);
void airQualityIndexMenu(const std::vector<Location>& locations, const WeatherSettings& settings);
void displayMainMenu(std::vector<Location>& locations);
#endif 
