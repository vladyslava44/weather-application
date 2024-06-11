#ifndef WEATHER_SETTINGS_H
#define WEATHER_SETTINGS_H

#include <string>

struct WeatherSettings {
    std::string temperatureUnit;   
    std::string windSpeedUnit;      
    std::string precipitationUnit; 
    std::string startDate;
    std::string endDate;
    std::string domain;
    std::string timeZone;
    std::string weatherModel;
    std::string reanalysisModel;

    static WeatherSettings& getInstance() {
        static WeatherSettings instance; 
        return instance;
    }

private:

    WeatherSettings() {
        temperatureUnit = "celsius";
        windSpeedUnit = "km/h";
        precipitationUnit = "mm";
        domain = "Global + European";
        startDate = "2024-04-01";
        endDate = "2024-04-04";
        timeZone = "auto";
        weatherModel = "best_match";
        reanalysisModel = "best_match";
    }
};

#endif 
