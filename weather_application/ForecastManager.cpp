#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <regex>
#include <sstream>
#include <iomanip> 
#include "ForecastManager.h"
#include "WeatherApiClient.h" 
#include "WeatherSettings.h"

bool isValidDateFormat(const std::string& date) {
    std::regex datePattern(R"(^\d{4}-\d{2}-\d{2}$)");
    return std::regex_match(date, datePattern);
}

void displayWeatherMenu(const std::vector<Location>& locations) {
    WeatherSettings& settings = WeatherSettings::getInstance();
    std::string startDate;
    std::string endDate;
    bool validStartDate;
    bool validEndDate;
    bool exitSettings = false;

    while (!exitSettings) {
        try {
            std::cout << "\nSearch for Weather Data Menu:" << std::endl;
            std::cout << "1. Weather Forecast" << std::endl;
            std::cout << "2. Air Quality Index" << std::endl;
            std::cout << "3. Settings" << std::endl;
            std::cout << "4. Historical Weather" << std::endl;

            int choice;
            std::cout << "Enter your choice: ";
            std::cin >> choice;

            switch (choice) {
            case 1:
                searchWeatherForecast(locations, settings);
                break;
            case 2:
                airQualityIndexMenu(locations, settings);
                break;
            case 3:
                // Settings Menu
                while (true) {
                    std::cout << "\nSettings:" << std::endl;
                    std::cout << "1. Temperature Unit (" << settings.temperatureUnit << ")" << std::endl;
                    std::cout << "2. Wind Speed Unit (" << settings.windSpeedUnit << ")" << std::endl;
                    std::cout << "3. Precipitation Unit (" << settings.precipitationUnit << ")" << std::endl;
                    std::cout << "4. Start Date(" << settings.startDate << "), End Date(" << settings.endDate << ")" << std::endl;
                    std::cout << "5. Domain (" << settings.domain << ")" << std::endl;
                    std::cout << "6. Time Zone (" << settings.timeZone << ")" << std::endl;
                    std::cout << "7. Weather Model(" << settings.weatherModel << ")" << std::endl;
                    std::cout << "8. Reanalysis Model(" << settings.reanalysisModel << ")" << std::endl;
                    std::cout << "9. Go back to menu" << std::endl;
                    int settingChoice;
                    std::cout << "Enter setting to change (1-5): ";
                    std::cin >> settingChoice;

                    switch (settingChoice) {
                    case 1:
                        std::cout << "Choose Temperature Unit:" << std::endl;
                        std::cout << "1. Celsius" << std::endl;
                        std::cout << "2. Fahrenheit" << std::endl;
                        int tempChoice;
                        std::cin >> tempChoice;
                        settings.temperatureUnit = (tempChoice == 1) ? "celsius" : "fahrenheit";
                        break;
                    case 2:
                        std::cout << "Choose Wind Speed Unit:" << std::endl;
                        std::cout << "1. km/h" << std::endl;
                        std::cout << "2. m/s" << std::endl;
                        int windChoice;
                        std::cin >> windChoice;
                        settings.windSpeedUnit = (windChoice == 1) ? "km/h" : "ms";
                        break;
                    case 3:
                        std::cout << "Choose Precipitation Unit:" << std::endl;
                        std::cout << "1. mm" << std::endl;
                        std::cout << "2. inches" << std::endl;
                        int precipChoice;
                        std::cin >> precipChoice;
                        settings.precipitationUnit = (precipChoice == 1) ? "mm" : "inch";
                        break;
                    case 4:
                         validStartDate = false;
                         validEndDate = false;

                        while (!validStartDate) {
                            std::cout << "Enter Start date (Format: YYYY-MM-DD): ";
                            std::cin >> startDate;

                            if (isValidDateFormat(startDate)) {
                                settings.startDate=startDate;
                                validStartDate = true;
                            }
                            else {
                                std::cout << "Invalid date format. Please enter in YYYY-MM-DD format.\n";
                            }
                        }

                        while (!validEndDate) {
                            std::cout << "Enter End date (Format: YYYY-MM-DD): ";
                            std::cin >> endDate;

                            if (isValidDateFormat(endDate)) {
                                settings.endDate = endDate;
                                validEndDate = true;
                            }
                            else {
                                std::cout << "Invalid date format. Please enter in YYYY-MM-DD format.\n";
                            }
                        }

                        break;
                    case 5:
                        std::cout << "Choose Domain:" << std::endl;
                        std::cout << "1. Global + European" << std::endl;
                        std::cout << "2. Global" << std::endl;
                        std::cout<< "3. European" << std::endl;
                        int domainChoice;
                        std::cin >> domainChoice;
                        settings.domain = (domainChoice == 1) ? "Global + European" :
                            (domainChoice == 2) ? "cams_global" :
                            (domainChoice == 3) ? "cams_europe" :
                            "Global + European"; 
                    case 6:
                        std::cout << "Choose Time Zone:" << std::endl;
                        std::cout << "1. Automatically detect time zone" << std::endl;
                        std::cout << "2. Europe/London" << std::endl;
                        std::cout << "3. America/New York" << std::endl;
                        std::cout << "4. Africa/Cairo" << std::endl;
                        int timeZoneChoice;
                        std::cin >> timeZoneChoice;
                        settings.timeZone = (timeZoneChoice == 1) ? "auto" :
                            (timeZoneChoice == 2) ? "Europe%2FLondon" :
                            (timeZoneChoice == 3) ? "America%2FNew_York" :
                            (timeZoneChoice == 4) ? "Africa%2FCairo" :
                            "auto";
                        break;
                    case 7:
                        std::cout << "Choose Weather Model:" << std::endl;
                        std::cout << "1. Best Match" << std::endl;
                        std::cout << "2. GFS Global" << std::endl;
                        std::cout << "3. DWD ICON Global" << std::endl;
                        int weatherModelChoice;
                        std::cin >> weatherModelChoice;
                        settings.weatherModel = (weatherModelChoice == 1) ? "best_match" :
                            (weatherModelChoice == 2) ? "gfs_global" :
                            (weatherModelChoice == 3) ? "icon_global" :
                            "best_match"; 
                        break;
                    case 8:
                        std::cout << "Choose Reanalysis Model:" << std::endl;
                        std::cout << "1. Best Match" << std::endl;
                        std::cout << "2. ECMWF IFS (9 km, Global, 2017 onwards)" << std::endl;
                        std::cout << "3. ERA5 (25 km, Global)" << std::endl;
                        int reanalysisModelChoice;
                        std::cin >> reanalysisModelChoice;
                        settings.reanalysisModel = (reanalysisModelChoice == 1) ? "best_match" :
                            (reanalysisModelChoice == 2) ? "ecmwf_ifs" :
                            (reanalysisModelChoice == 3) ? "era5" :
                            "best_match";
                        break;
                    case 9:
                        displayWeatherMenu(locations);
                        break;
                    default:
                        std::cout << "Invalid setting choice." << std::endl;
                        break;
                    }

                    
                }
                break;
            case 4:
                searchHistoricalWeather(locations, settings);
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}

void searchWeatherForecast(const std::vector<Location>& locations, const WeatherSettings& settings) {
    std::cout << "\n_______Weather Forecast_______\n";
    try {
        std::vector<Location> favorites;
        std::vector<Location> nonFavorites;

        for (const Location& loc : locations) {
            if (loc.isFavorite) {
                favorites.push_back(loc);
            }
            else {
                nonFavorites.push_back(loc);
            }
        }

        std::cout << "\nLocations:" << std::endl;

        for (const Location& loc : favorites) {
            std::cout << "ID: " << loc.id << ", Name: " << loc.name << " (Favorite)" << std::endl;
        }

        for (const Location& loc : nonFavorites) {
            std::cout << "ID: " << loc.id << ", Name: " << loc.name << std::endl;
        }

        int locationId;
        std::cout << "Enter the ID of the location for weather forecast: ";
        std::cin >> locationId;
        auto it = std::find_if(locations.begin(), locations.end(),
            [locationId](const Location& loc) { return loc.id == locationId; });
        std::string outputFile;
        if (it != locations.end()) {
            const Location& location = *it;

            std::string apiUrl = constructWeatherUrl(location.latitude, location.longitude);
            std::string jsonResponse = fetchJsonResponse(location.latitude, location.longitude, apiUrl);

            if (!jsonResponse.empty()) {


                std::cout << "\nWeather Model: " << settings.weatherModel << "\n";
                std::cout << "\nHourly Weather Variables: \n\n";
                std::string temperature = getValueForHour(jsonResponse, "hourly", "temperature_2m");
                std::cout << "Temperature: " << temperature << " " << settings.temperatureUnit << std::endl;

                std::string precipitation = getValueForHour(jsonResponse, "hourly", "precipitation");
                std::cout << "Precipitation: " << precipitation << " " << settings.precipitationUnit << std::endl;

                std::string pressure = getValueForHour(jsonResponse, "hourly", "surface_pressure");
                std::cout << "Pressure: " << pressure << " hPa" << std::endl;

                std::string windSpeed = getValueForHour(jsonResponse, "hourly", "wind_speed_10m");
                std::cout << "Wind speed: " << windSpeed << " " << settings.windSpeedUnit << std::endl;

                std::string humidity = getValueForHour(jsonResponse, "hourly", "relative_humidity_2m");
                std::cout << "Humidity: " << humidity << " %" << std::endl;

                std::string cloudCover = getValueForHour(jsonResponse, "hourly", "cloud_cover");
                std::cout << "Cloud Cover: " << cloudCover << " %" << std::endl;

                std::cout << "\nAdditional Hourly Variables\n\n";


                std::string dew_point_2m = getValueForHour(jsonResponse, "hourly", "dew_point_2m");
                std::cout << "Dew Point(2m): " << dew_point_2m << " " << settings.temperatureUnit << std::endl;

                std::string apparent_temperature = getValueForHour(jsonResponse, "hourly", "apparent_temperature");
                std::cout << "Apparent Temperature: " << apparent_temperature << " " << settings.temperatureUnit << std::endl;

                std::string rain = getValueForHour(jsonResponse, "hourly", "rain");
                std::cout << "Rain: " << rain << " " << settings.precipitationUnit << std::endl;

                std::string snowfallValue = getValueForHour(jsonResponse, "hourly", "snowfall");
                std::cout << "Snowfall: " << snowfallValue << " " << settings.precipitationUnit << std::endl;

                std::string snowDepthValue = getValueForHour(jsonResponse, "hourly", "snow_depth");
                std::cout << "Snow Depth: " << snowDepthValue << " cm" << std::endl;

                std::string weatherCodeValue = getValueForHour(jsonResponse, "hourly", "weather_code");
                std::cout << "Weather Code: " << weatherCodeValue << std::endl;

                std::string pressureMSLValue = getValueForHour(jsonResponse, "hourly", "pressure_msl");
                std::cout << "Pressure MSL: " << pressureMSLValue << " hPa" << std::endl;

                std::string surfacePressureValue = getValueForHour(jsonResponse, "hourly", "surface_pressure");
                std::cout << "Surface Pressure: " << surfacePressureValue << " hPa" << std::endl;

                std::string cloudCoverValue = getValueForHour(jsonResponse, "hourly", "cloud_cover");
                std::cout << "Cloud Cover: " << cloudCoverValue << " %" << std::endl;

                std::string cloudCoverLowValue = getValueForHour(jsonResponse, "hourly", "cloud_cover_low");
                std::cout << "Cloud Cover Low: " << cloudCoverLowValue << " %" << std::endl;

                std::string cloudCoverMidValue = getValueForHour(jsonResponse, "hourly", "cloud_cover_mid");
                std::cout << "Cloud Cover Mid: " << cloudCoverMidValue << " %" << std::endl;

                std::string cloudCoverHighValue = getValueForHour(jsonResponse, "hourly", "cloud_cover_high");
                std::cout << "Cloud Cover High: " << cloudCoverHighValue << " %" << std::endl;

                std::string et0FaoEvapotranspirationValue = getValueForHour(jsonResponse, "hourly", "et0_fao_evapotranspiration");
                std::cout << "ET0 FAO Evapotranspiration: " << et0FaoEvapotranspirationValue << " " << settings.precipitationUnit << std::endl;

                std::string vapourPressureDeficitValue = getValueForHour(jsonResponse, "hourly", "vapour_pressure_deficit");
                std::cout << "Vapour Pressure Deficit: " << vapourPressureDeficitValue << std::endl;

                std::string windSpeed10mValue = getValueForHour(jsonResponse, "hourly", "wind_speed_10m");
                std::cout << "Wind Speed (10m): " << windSpeed10mValue << " " << settings.windSpeedUnit << std::endl;

                std::string windSpeed100mValue = getValueForHour(jsonResponse, "hourly", "wind_speed_100m");
                std::cout << "Wind Speed (100m): " << windSpeed100mValue << " " << settings.windSpeedUnit << std::endl;

                std::string windDirection10mValue = getValueForHour(jsonResponse, "hourly", "wind_direction_10m");
                std::cout << "Wind Direction (10m): " << windDirection10mValue << " degrees" << std::endl;

                std::string windDirection100mValue = getValueForHour(jsonResponse, "hourly", "wind_direction_100m");
                std::cout << "Wind Direction (100m): " << windDirection100mValue << " degrees" << std::endl;

                std::string windGusts10mValue = getValueForHour(jsonResponse, "hourly", "wind_gusts_10m");
                std::cout << "Wind Gusts (10m): " << windGusts10mValue << " " << settings.windSpeedUnit << std::endl;


                std::cout << "\nSolar Radiation\n\n";

                std::string shortwave_radiation = getValueForHour(jsonResponse, "hourly", "shortwave_radiation_instant");
                std::cout << "Shortwave Radiation: " << shortwave_radiation << " W/m^2" << std::endl;

                std::string direct_radiation = getValueForHour(jsonResponse, "hourly", "direct_radiation_instant");
                std::cout << "Direct Radiation: " << direct_radiation << " W/m^2" << std::endl;

                std::string diffuse_radiation = getValueForHour(jsonResponse, "hourly", "diffuse_radiation_instant");
                std::cout << "Diffuse Radiation: " << diffuse_radiation << " W/m^2" << std::endl;

                std::string direct_normal_irradiance_instant = getValueForHour(jsonResponse, "hourly", "direct_normal_irradiance_instant");
                std::cout << "Direct Normal Irradiance Instant: " << direct_normal_irradiance_instant << " W/m^2" << std::endl;

                std::string terrestrial_radiation_instant = getValueForHour(jsonResponse, "hourly", "terrestrial_radiation_instant");
                std::cout << "Terrestrial Radiation Instant: " << terrestrial_radiation_instant << " W/m^2" << std::endl;

                std::cout << "\nPressure Level Variables\n\n";

                std::string relativeHumidity950hPa = getValueForHour(jsonResponse, "hourly", "relative_humidity_950hPa");
                std::cout << "Relative Humidity at 950hPa: " << relativeHumidity950hPa << " %" << std::endl;

                std::string relativeHumidity250hPa = getValueForHour(jsonResponse, "hourly", "relative_humidity_250hPa");
                std::cout << "Relative Humidity at 250hPa: " << relativeHumidity250hPa << " %" << std::endl;

                std::string relativeHumidity30hPa = getValueForHour(jsonResponse, "hourly", "relative_humidity_30hPa");
                std::cout << "Relative Humidity at 30hPa: " << relativeHumidity30hPa << " %" << std::endl;

                std::string cloudCover975hPa = getValueForHour(jsonResponse, "hourly", "cloud_cover_975hPa");
                std::cout << "Cloud Cover at 975hPa: " << cloudCover975hPa << " %" << std::endl;

                std::string cloudCover600hPa = getValueForHour(jsonResponse, "hourly", "cloud_cover_600hPa");
                std::cout << "Cloud Cover at 600hPa: " << cloudCover600hPa << " %" << std::endl;

                std::string cloudCover50hPa = getValueForHour(jsonResponse, "hourly", "cloud_cover_50hPa");
                std::cout << "Cloud Cover at 50hPa: " << cloudCover50hPa << " %" << std::endl;

                std::string windDirection975hPa = getValueForHour(jsonResponse, "hourly", "winddirection_975hPa");
                std::cout << "Wind Direction at 975hPa: " << windDirection975hPa << " degrees" << std::endl;

                std::string windDirection600hPa = getValueForHour(jsonResponse, "hourly", "winddirection_600hPa");
                std::cout << "Wind Direction at 600hPa: " << windDirection600hPa << " degrees" << std::endl;

                std::string windDirection50hPa = getValueForHour(jsonResponse, "hourly", "winddirection_50hPa");
                std::cout << "Wind Direction at 50hPa: " << windDirection50hPa << " degrees" << std::endl;

                std::string geopotentialHeight1000hPa = getValueForHour(jsonResponse, "hourly", "geopotential_height_1000hPa");
                std::cout << "Geopotential Height at 1000hPa: " << geopotentialHeight1000hPa << " m" << std::endl;

                std::string geopotentialHeight850hPa = getValueForHour(jsonResponse, "hourly", "geopotential_height_850hPa");
                std::cout << "Geopotential Height at 850hPa: " << geopotentialHeight850hPa << " m" << std::endl;

                std::string geopotentialHeight500hPa = getValueForHour(jsonResponse, "hourly", "geopotential_height_500hPa");
                std::cout << "Geopotential Height at 500hPa: " << geopotentialHeight500hPa << " m" << std::endl;

                std::string geopotentialHeight200hPa = getValueForHour(jsonResponse, "hourly", "geopotential_height_200hPa");
                std::cout << "Geopotential Height at 200hPa: " << geopotentialHeight200hPa << " m" << std::endl;

                std::string geopotentialHeight30hPa = getValueForHour(jsonResponse, "hourly", "geopotential_height_30hPa");
                std::cout << "Geopotential Height at 30hPa: " << geopotentialHeight30hPa << " m" << std::endl;



                std::cout << "\nDaily Weather Variables: \n\n";

                std::string temperature_2m_maxD = getValueDaily(jsonResponse, "daily", "temperature_2m_max");
                std::cout << "Maximum Temperature: " << temperature_2m_maxD << " " << settings.temperatureUnit << std::endl;

                std::string temperature_2m_minD = getValueDaily(jsonResponse, "daily", "temperature_2m_min");
                std::cout << "Minimum Temperature: " << temperature_2m_minD << " " << settings.temperatureUnit << std::endl;

                std::string sunrise = getValueDaily(jsonResponse, "daily", "sunrise");
                std::cout << "Sunrise: " << sunrise << std::endl;

                std::string sunset = getValueDaily(jsonResponse, "daily", "sunset");
                std::cout << "Sunset: " << sunset << std::endl;

                std::string sunshine_duration = getValueDaily(jsonResponse, "daily", "sunshine_duration");
                std::cout << "Sunshine Duration " << sunshine_duration << " seconds" << std::endl;


                std::string daylight_duration = getValueDaily(jsonResponse, "daily", "daylight_duration");
                std::cout << "Daylight Duration " << daylight_duration << " seconds" << std::endl;

                std::string rain_sum = getValueDaily(jsonResponse, "daily", "rain_sum");
                std::cout << "Rain Sum: " << rain_sum << " " << settings.precipitationUnit << std::endl;

                std::string uv_index_max = getValueDaily(jsonResponse, "daily", "uv_index_max");
                std::cout << "UV Index Max: " << uv_index_max << std::endl;

                std::string precipitation_sum = getValueDaily(jsonResponse, "daily", "precipitation_sum");
                std::cout << "Precipitation Sum " << precipitation_sum << " " << settings.precipitationUnit << std::endl;


                std::cout << "\nAdditional Variables\n\n";

                std::string uv_index = getValueForHour(jsonResponse, "hourly", "uv_index");
                std::cout << "UV Index: " << uv_index << std::endl;

                std::string uv_index_clear_sky = getValueForHour(jsonResponse, "hourly", "uv_index_clear_sky");
                std::cout << "UV Index Clear Sky: " << uv_index_clear_sky << std::endl;

                std::string is_day = getValueForHour(jsonResponse, "hourly", "is_day");
                if (is_day == "1") {
                    std::cout << "Is Day: " << "Yes"  << std::endl;
                }
                else {
                    std::cout << "Is Day: " << "No" << std::endl;
                }

                std::string freezing_level_height = getValueForHour(jsonResponse, "hourly", "freezing_level_height");
                std::cout << "Freezing Level Height: " << freezing_level_height << " m" << std::endl;
                int exportFile;
                std::cout << "Would you like to save file as CSV, JSON. Yes(1), No(0)";
                std::cin >> exportFile;
                while (exportFile != 1 && exportFile != 0) {
                    std::cout << "You can insert yes or no only";
                    std::cin >> exportFile;
                }
                if (exportFile == 1) {
                    outputFile = "weather.csv";
                    exportDataAsCSV(jsonResponse, outputFile);

                    outputFile = "weather.json";
                    exportDataAsJSON(jsonResponse, outputFile);
                }
                


            }

        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void airQualityIndexMenu(const std::vector<Location>& locations, const WeatherSettings& settings) {
    //WeatherSettings& settings = WeatherSettings::getInstance();
    std::cout << "\n_______Air Quality Forecast_______\n";
    try{
    std::vector<Location> favorites;
    std::vector<Location> nonFavorites;

    for (const Location& loc : locations) {
        if (loc.isFavorite) {
            favorites.push_back(loc);
        }
        else {
            nonFavorites.push_back(loc);
        }
    }

    std::cout << "\nLocations:" << std::endl;

    for (const Location& loc : favorites) {
        std::cout << "ID: " << loc.id << ", Name: " << loc.name << " (Favorite)" << std::endl;
    }

    for (const Location& loc : nonFavorites) {
        std::cout << "ID: " << loc.id << ", Name: " << loc.name << std::endl;
    }

    int locationId;
    std::cout << "Enter the ID of the location for air quality index: ";
    std::cin >> locationId;
    auto it = std::find_if(locations.begin(), locations.end(),
        [locationId](const Location& loc) { return loc.id == locationId; });

    if (it != locations.end()) {
        const Location& location = *it;
        std::string apiUrl = constructAirQualityUrl(location.latitude, location.longitude);
        std::string jsonResponse = fetchJsonResponse(location.latitude, location.longitude, apiUrl);

        if (!jsonResponse.empty()) {
            std::cout << "\nDomain: ";

            if (settings.domain == "cams_global") {
                std::cout << "Global";
            }
            else if (settings.domain == "cams_europe") {
                std::cout << "European";
            }
            else {
                // Default case if domain is neither "cams_global" nor "cams_europe"
                std::cout << settings.domain; // Output the actual domain value
            }
            std::cout << "\n\nHourly Air Quality Variables: \n\n";
           
            std::string carbon_monoxide = getValueDaily(jsonResponse, "hourly", "carbon_monoxide");
            std::cout << "Carbon Monoxide: " << carbon_monoxide << " ug/m^3" <<  std::endl;
            std::string ozone = getValueDaily(jsonResponse, "hourly", "ozone");
            std::cout << "Ozone: " << ozone << " ug/m^3" << std::endl;
            std::string sulphur_dioxide = getValueDaily(jsonResponse, "hourly", "sulphur_dioxide");
            std::cout << "Sulphur Dioxide: " << sulphur_dioxide << " ug/m^3" << std::endl;
            std::string pm10 = getValueDaily(jsonResponse, "hourly", "pm10");
            std::cout << "Particulate Matter PM10: " << pm10 << " ug/m^3" << std::endl;

            std::string pm2_5 = getValueDaily(jsonResponse, "hourly", "pm2_5");
            std::cout << "Particulate Matter PM2.5: " << pm2_5 << " ug/m^3" << std::endl;
            std::cout << "\nEuropean Air Quality Index: \n\n";

            std::string european_aqi = getValueDaily(jsonResponse, "hourly", "european_aqi");
            std::cout << "European AQI: " << european_aqi << std::endl;

            std::string european_aqi_nitrogen_dioxide = getValueDaily(jsonResponse, "hourly", "european_aqi_nitrogen_dioxide");
            std::cout << "European AQI(nitrogen dioxide): " << european_aqi_nitrogen_dioxide<< std::endl;

            std::string european_aqi_ozone = getValueDaily(jsonResponse, "hourly", "european_aqi_ozone");
            std::cout << "European AQI(ozone): " << european_aqi_ozone << std::endl;

            std::string european_aqi_sulphur_dioxide = getValueDaily(jsonResponse, "hourly", "european_aqi_sulphur_dioxide");
            std::cout << "European AQI(sulphur dioxide): " << european_aqi_sulphur_dioxide << std::endl;

        }

    }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void searchHistoricalWeather(const std::vector<Location>& locations, const WeatherSettings& settings) {
    std :: cout << "\n_______Historical Weather_______\n";
    try{
    std::vector<Location> favorites;
    std::vector<Location> nonFavorites;

    for (const Location& loc : locations) {
        if (loc.isFavorite) {
            favorites.push_back(loc);
        }
        else {
            nonFavorites.push_back(loc);
        }
    }

    std::cout << "\nLocations:" << std::endl;

    for (const Location& loc : favorites) {
        std::cout << "ID: " << loc.id << ", Name: " << loc.name << " (Favorite)" << std::endl;
    }

    for (const Location& loc : nonFavorites) {
        std::cout << "ID: " << loc.id << ", Name: " << loc.name << std::endl;
    }

    int locationId;
    std::cout << "Enter the ID of the location for air quality index: ";
    std::cin >> locationId;

    displayHistoricalWeatherDay(locations, settings, locationId);
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

int countDaysInDateRange(const std::string& startDateStr, const std::string& endDateStr) {
    std::tm startDate = {};
    std::tm endDate = {};

    std::stringstream startDateStream(startDateStr);
    startDateStream >> std::get_time(&startDate, "%Y-%m-%d");

    std::stringstream endDateStream(endDateStr);
    endDateStream >> std::get_time(&endDate, "%Y-%m-%d");

    if (startDateStream.fail() || endDateStream.fail()) {
        std::cerr << "Failed to parse dates." << std::endl;
        return -1; 
    }

    std::chrono::system_clock::time_point start = std::chrono::system_clock::from_time_t(std::mktime(&startDate));
    std::chrono::system_clock::time_point end = std::chrono::system_clock::from_time_t(std::mktime(&endDate));

    std::chrono::duration<double> duration = end - start;

    int days = std::chrono::duration_cast<std::chrono::hours>(duration).count() / 24;

    return days + 1;
}

void displayHistoricalWeatherDay(const std::vector<Location>& locations, const WeatherSettings& settings, int locationId) {
    Location location = getLocationById(locations, locationId);
    std::string apiUrl = constructHistoricalUrl(location.latitude, location.longitude);
    std::string jsonResponse = fetchJsonResponse(location.latitude, location.longitude, apiUrl);

    if (!jsonResponse.empty()) {

        std::cout << "\n\nStart Date: " << settings.startDate << std::endl;
        std::cout << "End Date: " << settings.endDate << std::endl;
        std::cout << "Reanalysis Model: " << settings.reanalysisModel << "\n";
        int numDays = countDaysInDateRange(settings.startDate, settings.endDate);

        
        for (int i=1; i <= numDays; i++) {

            std::cout << "\nDay "<<i<<": \n\n";
            std::cout << "Historical Weather Variables: \n\n";
            std::string temperature_2m_maxD = getHistoricalValues(jsonResponse, "daily", "temperature_2m_max", i);
            std::cout << "Maximum Temperature: " << temperature_2m_maxD << " " << settings.temperatureUnit << std::endl;

            std::string temperature_2m_minD = getHistoricalValues(jsonResponse, "daily", "temperature_2m_min",i);
            std::cout << "Minimum Temperature: " << temperature_2m_minD << " " << settings.temperatureUnit << std::endl;

            std::string temperature_2m_mean = getHistoricalValues(jsonResponse, "daily", "temperature_2m_mean",i);
            std::cout << "Mean Temperature: " << temperature_2m_mean << " " << settings.temperatureUnit << std::endl;

            std::string apparent_temperature_max = getHistoricalValues(jsonResponse, "daily", "apparent_temperature_max",i);
            std::cout << "Aparent Max Temperature: " << apparent_temperature_max << " " << settings.temperatureUnit << std::endl;

            std::string apparent_temperature_min = getHistoricalValues(jsonResponse, "daily", "apparent_temperature_min",i);
            std::cout << "Aparent Min Temperature: " << apparent_temperature_min << " " << settings.temperatureUnit << std::endl;

            std::string apparent_temperature_mean = getHistoricalValues(jsonResponse, "daily", "apparent_temperature_mean",i);
            std::cout << "Aparent Mean Temperature: " << apparent_temperature_mean << " " << settings.temperatureUnit << std::endl;

            std::string sunrise = getHistoricalValues(jsonResponse, "daily", "sunrise", i);
            std::cout << "Sunrise: " << sunrise << std::endl;

            std::string sunset = getHistoricalValues(jsonResponse, "daily", "sunset", i);
            std::cout << "Sunset: " << sunset << std::endl;

            std::string sunshine_duration = getHistoricalValues(jsonResponse, "daily", "sunshine_duration",i);
            std::cout << "Sunshine Duration: " << sunshine_duration << " seconds" << std::endl;


            std::string daylight_duration = getHistoricalValues(jsonResponse, "daily", "daylight_duration",i);
            std::cout << "Daylight Duration: " << daylight_duration << " seconds" << std::endl;

            std::string rain_sum = getHistoricalValues(jsonResponse, "daily", "rain_sum",i);
            std::cout << "Rain Sum: " << rain_sum << " " << settings.precipitationUnit << std::endl;

            std::string precipitation_sum = getHistoricalValues(jsonResponse, "daily", "precipitation_sum",i);
            std::cout << "Precipitation Sum: " << precipitation_sum << settings.precipitationUnit << std::endl;

            std::string precipitation_hours = getHistoricalValues(jsonResponse, "daily", "precipitation_hours",i);
            std::cout << "Precipitation Hours: " << precipitation_hours << settings.precipitationUnit << std::endl;


            std::string is_day = getHistoricalValues(jsonResponse, "hourly", "is_day",i);
            std::cout << "Day/Night: ";
            if (is_day == "yes") {
                std::cout << "Day\n\n";
            }
            else {
                std::cout << "Night\n\n";
            }
        }

    }

}

void displayMainMenu(std::vector<Location>& locations) {
    int choice;
    while (true) {
        std::cout << "\nMain Menu:" << std::endl;
        std::cout << "1. Manage Locations" << std::endl;
        std::cout << "2. Search for Weather Data" << std::endl;
        std::cout << "3. Exit" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            manageLocations(locations);
            break;
        case 2:
            displayWeatherMenu(locations);
            break;
        case 3:
            std::cout << "Exiting program..." << std::endl;
            return;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }
}