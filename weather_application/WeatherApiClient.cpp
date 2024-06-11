#define _CRT_SECURE_NO_WARNINGS
#include "WeatherApiClient.h"
#include "ForecastManager.h"
#include "WeatherSettings.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <curl/curl.h>
#include <chrono>
#include <ctime>
#include <fstream>


size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* data) {
    data->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string constructWeatherUrl(double latitude, double longitude) {
    WeatherSettings& weatherSettings = WeatherSettings::getInstance();
    std::string apiUrl = "https://api.open-meteo.com/v1/forecast?latitude=" + std::to_string(latitude) +
        "&longitude=" + std::to_string(longitude) +
        "&hourly=temperature_2m,relative_humidity_2m,dew_point_2m,apparent_temperature,precipitation,rain,snowfall,snow_depth,weather_code,pressure_msl,surface_pressure,cloud_cover,cloud_cover_low,cloud_cover_mid,cloud_cover_high,et0_fao_evapotranspiration,vapour_pressure_deficit,wind_speed_10m,wind_speed_100m,wind_direction_10m,wind_direction_100m,wind_gusts_10m,soil_temperature_0_to_7cm,soil_temperature_7_to_28cm,soil_temperature_28_to_100cm,soil_temperature_100_to_255cm,soil_moisture_0_to_7cm,soil_moisture_7_to_28cm,soil_moisture_28_to_100cm,soil_moisture_100_to_255cm,shortwave_radiation_instant,direct_radiation_instant,diffuse_radiation_instant,direct_normal_irradiance_instant,terrestrial_radiation_instant,relative_humidity_950hPa,relative_humidity_250hPa,relative_humidity_30hPa,cloud_cover_975hPa,cloud_cover_600hPa,cloud_cover_50hPa,winddirection_975hPa,winddirection_600hPa,winddirection_50hPa,geopotential_height_1000hPa,geopotential_height_850hPa,geopotential_height_500hPa,geopotential_height_200hPa,geopotential_height_30hPa,uv_index,uv_index_clear_sky,is_day,freezing_level_height,sunshine_duration&daily=temperature_2m_max,temperature_2m_min,sunrise,sunset,daylight_duration,sunshine_duration,uv_index_max,precipitation_sum,rain_sum";
    if (!weatherSettings.temperatureUnit.empty() && weatherSettings.temperatureUnit!="celsius") {
        apiUrl += "&temperature_unit=" + weatherSettings.temperatureUnit;
    }

    if (!weatherSettings.windSpeedUnit.empty() && weatherSettings.windSpeedUnit != "km/h") {
        apiUrl += "&wind_speed_unit=" + weatherSettings.windSpeedUnit;
    }

    if (!weatherSettings.precipitationUnit.empty() && weatherSettings.precipitationUnit != "mm") {
        apiUrl += "&precipitation_unit=" + weatherSettings.precipitationUnit;
    }
    if (!weatherSettings.timeZone.empty()) {
        apiUrl += "&timezone=" + weatherSettings.timeZone;
    }
    if (!weatherSettings.weatherModel.empty() ) {
        apiUrl += "&models=" + weatherSettings.weatherModel;
    }
   

    return apiUrl;
}

std::string constructAirQualityUrl(double latitude, double longitude) {
    WeatherSettings& weatherSettings = WeatherSettings::getInstance();
    std::string apiUrl = "https://air-quality-api.open-meteo.com/v1/air-quality?latitude=" + std::to_string(latitude) +
        "&longitude=" + std::to_string(longitude) +
        "&hourly=pm10,pm2_5,carbon_monoxide,nitrogen_dioxide,sulphur_dioxide,ozone,aerosol_optical_depth,dust,uv_index,european_aqi,european_aqi_nitrogen_dioxide,european_aqi_ozone,european_aqi_sulphur_dioxide&domains=cams_europe"; 
    if (!weatherSettings.domain.empty() && weatherSettings.domain != "Global + European") {
        apiUrl += "&domain=" + weatherSettings.domain;
    }
    if (!weatherSettings.reanalysisModel.empty()) {
        apiUrl += "&models=" + weatherSettings.reanalysisModel;
    }
    
    return apiUrl;
}

std::string constructHistoricalUrl(double latitude, double longitude) {
    WeatherSettings& weatherSettings = WeatherSettings::getInstance();
    std::string apiUrl = "https://archive-api.open-meteo.com/v1/archive?latitude=" + std::to_string(latitude) +
        "&longitude=" + std::to_string(longitude) +
        "&start_date=" + weatherSettings.startDate + "&end_date=" + weatherSettings.endDate + "&hourly=is_day&daily=temperature_2m_max,temperature_2m_min,temperature_2m_mean,apparent_temperature_max,apparent_temperature_min,apparent_temperature_mean,sunrise,sunset,daylight_duration,sunshine_duration,precipitation_sum,rain_sum,precipitation_hours";

    if (!weatherSettings.temperatureUnit.empty() && weatherSettings.temperatureUnit != "celsius") {
        apiUrl += "&temperature_unit=" + weatherSettings.temperatureUnit;
    }

    if (!weatherSettings.windSpeedUnit.empty() && weatherSettings.windSpeedUnit != "km/h") {
        apiUrl += "&wind_speed_unit=" + weatherSettings.windSpeedUnit;
    }

    if (!weatherSettings.precipitationUnit.empty() && weatherSettings.precipitationUnit != "mm") {
        apiUrl += "&precipitation_unit=" + weatherSettings.precipitationUnit;
    }
    if (!weatherSettings.startDate.empty() && weatherSettings.startDate != "2024-04-01") {
        apiUrl += "&start_date=" + weatherSettings.startDate;
    }
    if (!weatherSettings.endDate.empty() && weatherSettings.endDate != "2024-04-04") {
        apiUrl += "&end_date=" + weatherSettings.endDate;
    }

    return apiUrl;
}

std::vector<std::string> splitString(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream iss(s);
    std::string token;
    while (std::getline(iss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

void exportDataAsCSV(const std::string& jsonData, const std::string& outputFile) {
    std::ofstream file(outputFile);
    if (file.is_open()) {
        std::istringstream iss(jsonData);
        std::string line;
        while (std::getline(iss, line)) {
            line.erase(0, line.find_first_not_of(" \t\n\r")); 
            line.erase(line.find_last_not_of(" \t\n\r") + 1); 
            if (line.empty()) {
                continue;
            }

            std::vector<std::string> fields = splitString(line, ',');
            bool firstField = true;
            for (const auto& field : fields) {
                size_t colonPos = field.find(':');
                if (colonPos != std::string::npos) {
                    std::string value = field.substr(colonPos + 1);
                    value.erase(0, value.find_first_not_of(" \t\"")); 
                    value.erase(value.find_last_not_of(" \t\"\n\r") + 1); 
                    if (!firstField) {
                        file << ",";
                    }
                    file << value;
                    firstField = false;
                }
            }
            file << std::endl;
        }
        std::cout << "Data exported to CSV: " << outputFile << std::endl;
        file.close();
    }
    else {
        std::cerr << "Error: Unable to open file for CSV export." << std::endl;
    }
}

void exportDataAsJSON(const std::string& data, const std::string& outputFile) {
    std::ofstream file(outputFile);
    if (file.is_open()) {
        file << data << std::endl; 
        std::cout << "Data exported to JSON: " << outputFile << std::endl;
        file.close();
    }
    else {
        std::cerr << "Error: Unable to open file for JSON export." << std::endl;
    }
}

std::string fetchJsonResponse(double latitude, double longitude, std::string apiUrl) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;


    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, apiUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
       
        
    }
   

    return readBuffer;
}

double getValueForSolar(const std::string& jsonStr, const std::string& sectionName, const std::string& variableName) {
    std::vector<double> values;
   
    int hour = 12;

    size_t sectionPos = jsonStr.find("\"" + sectionName + "\":{");
    if (sectionPos != std::string::npos) {
        std::string variableSearchStr = "\"" + variableName + "\":[";

        size_t variablePos = jsonStr.find(variableSearchStr, sectionPos);
        if (variablePos != std::string::npos) {
            variablePos += variableSearchStr.length();

            size_t arrayEndPos = jsonStr.find(']', variablePos);
            if (arrayEndPos != std::string::npos) {
                std::string valuesStr = jsonStr.substr(variablePos, arrayEndPos - variablePos);

                std::istringstream iss(valuesStr);
                std::string token;
                while (std::getline(iss, token, ',')) {
                    double value = std::stod(token);
                    values.push_back(value);
                }
                if (hour >= 0 && hour < values.size()) {
                    return values[hour]; 
                }
                else {
                    std::cerr << "Current hour out of range." << std::endl;
                }
            }
            else {
                std::cerr << "End of array not found for variable: " << variableName << std::endl;
            }
        }
        else {
            std::cerr << "Variable not found: " << variableName << std::endl;
        }
    }
    else {
        std::cerr << "Section not found: " << sectionName << std::endl;
    }

    return 0.0; 
}

std::string getValueDaily(const std::string& jsonStr, const std::string& sectionName, const std::string& variableName) {
    
    size_t sectionPos = jsonStr.find("\"" + sectionName + "\":{");
    if (sectionPos != std::string::npos) {
        std::string variableSearchStr = "\"" + variableName + "\":["; 
        size_t variablePos = jsonStr.find(variableSearchStr, sectionPos);
        if (variablePos != std::string::npos) {
            variablePos += variableSearchStr.length();

            size_t arrayEndPos = jsonStr.find_first_of(",}]", variablePos);
            if (arrayEndPos != std::string::npos) {
                std::string valueStr = jsonStr.substr(variablePos, arrayEndPos - variablePos);

                if (!valueStr.empty() && (valueStr.front() == '"' && valueStr.back() == '"')) {
                    valueStr = valueStr.substr(1, valueStr.length() - 2);
                }

                return valueStr; 
            }
            else {
                std::cerr << "End of array not found for variable: " << variableName << std::endl;
            }
        }
        else {
            std::cerr << "Variable not found: " << variableName << std::endl;
        }
    }
    else {
        std::cerr << "Section not found: " << sectionName << std::endl;
    }

    return ""; 
}

std::string getHistoricalValues(const std::string& jsonStr, const std::string& sectionName, const std::string& variableName, int dayValue) {
    std::string result;

    size_t sectionPos = jsonStr.find("\"" + sectionName + "\":{");
    if (sectionPos != std::string::npos) {
        std::string variableSearchStr = "\"" + variableName + "\":[";

        size_t variablePos = jsonStr.find(variableSearchStr, sectionPos);
        if (variablePos != std::string::npos) {
            variablePos += variableSearchStr.length();

            size_t arrayEndPos = jsonStr.find(']', variablePos);
            if (arrayEndPos != std::string::npos) {
                std::string valuesStr = jsonStr.substr(variablePos, arrayEndPos - variablePos);

                std::istringstream iss(valuesStr);
                std::string token;
                int currentIndex = 0;
                while (std::getline(iss, token, ',')) {
                    if (currentIndex == dayValue-1) {
                        result = token; // Store the token corresponding to the specified dayValue
                        break;
                    }
                    currentIndex++;
                }

                if (result.empty()) {
                    std::cerr << "Specified day value out of range." << std::endl;
                }
            }
            else {
                std::cerr << "End of array not found for variable: " << variableName << std::endl;
            }
        }
        else {
            std::cerr << "Variable not found: " << variableName << std::endl;
        }
    }
    else {
        std::cerr << "Section not found: " << sectionName << std::endl;
    }

    return result;
}

std::string getValueForHour(const std::string& jsonStr, const std::string& sectionName, const std::string& variableName) {
    time_t now = time(0);
    char dt[26];

    if (ctime_s(dt, sizeof(dt), &now) != 0) {
        std::cerr << "Error converting time to string\n";
        return ""; 
    }

    int currentHour = (dt[11] - '0') * 10 + (dt[12] - '0');

    size_t sectionPos = jsonStr.find("\"" + sectionName + "\":{");
    if (sectionPos != std::string::npos) {
        std::string variableSearchStr = "\"" + variableName + "\":[";

        size_t variablePos = jsonStr.find(variableSearchStr, sectionPos);
        if (variablePos != std::string::npos) {
            variablePos += variableSearchStr.length();

            size_t arrayEndPos = jsonStr.find(']', variablePos);
            if (arrayEndPos != std::string::npos) {
                std::string valuesStr = jsonStr.substr(variablePos, arrayEndPos - variablePos);

                std::istringstream iss(valuesStr);
                std::string token;
                int currentIndex = 0;
                while (std::getline(iss, token, ',')) {
                    if (currentIndex == currentHour) {
                        token.erase(0, token.find_first_not_of(" \t\r\n"));
                        token.erase(token.find_last_not_of(" \t\r\n") + 1);
                        return token; 
                    }
                    currentIndex++;
                }

                std::cerr << "Current hour value not found in array for variable: " << variableName << std::endl;
            }
            else {
                std::cerr << "End of array not found for variable: " << variableName << std::endl;
            }
        }
        else {
            std::cerr << "Variable not found: " << variableName << std::endl;
        }
    }
    else {
        std::cerr << "Section not found: " << sectionName << std::endl;
    }

    return ""; // Return empty string if value retrieval fails
}

double getValueForForecast(const std::string& jsonStr, const std::string& sectionName, const std::string& variableName) {
    std::vector<double> values;
    time_t now = time(0);

    char dt[26];

    if (ctime_s(dt, sizeof(dt), &now) != 0) {
        std::cerr << "Error converting time to string\n";
        return 1;
    }
    int currentHour = (dt[11] - '0') * 10 + (dt[12] - '0');

    size_t sectionPos = jsonStr.find("\"" + sectionName + "\":{");
    if (sectionPos != std::string::npos) {
        std::string variableSearchStr = "\"" + variableName + "\":[";

        size_t variablePos = jsonStr.find(variableSearchStr, sectionPos);
        if (variablePos != std::string::npos) {
            variablePos += variableSearchStr.length();

            size_t arrayEndPos = jsonStr.find(']', variablePos);
            if (arrayEndPos != std::string::npos) {
                std::string valuesStr = jsonStr.substr(variablePos, arrayEndPos - variablePos);

                std::istringstream iss(valuesStr);
                std::string token;
                while (std::getline(iss, token, ',')) {
                    double value = std::stod(token);
                    values.push_back(value);
                }
                if (currentHour >= 0 && currentHour < values.size()) {
                    return values[currentHour];
                }
                else {
                    std::cerr << "Current hour out of range." << std::endl;
                }
            }
            else {
                std::cerr << "End of array not found for variable: " << variableName << std::endl;
            }
        }
        else {
            std::cerr << "Variable not found: " << variableName << std::endl;
        }
    }
    else {
        std::cerr << "Section not found: " << sectionName << std::endl;
    }

    return 0.0;
}