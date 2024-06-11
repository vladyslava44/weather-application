#ifndef WEATHERAPICLIENT_H
#define WEATHERAPICLIENT_H
#include <string>
#include "WeatherSettings.h"

void exportDataAsJSON(const std::string& data, const std::string& outputFile);
void exportDataAsCSV(const std::string& data, const std::string& outputFile);
std::string fetchJsonResponse(double latitude, double longitude, std::string apiUrl); 
std::string constructHistoricalUrl(double latitude, double longitude);
std::string getHistoricalValues(const std::string& jsonStr, const std::string& sectionName, const std::string& variableName, int dayValue);
std::string getValueForHour(const std::string& jsonStr, const std::string& sectionName, const std::string& variableName);
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* data); 
std::string getValueDaily(const std::string& jsonStr, const std::string& sectionName, const std::string& variableName);
double getValueForSolar(const std::string& jsonStr, const std::string& sectionName, const std::string& variableName);
std::string constructWeatherUrl(double latitude, double longitude);
std::string constructAirQualityUrl(double latitude, double longitude);
#endif 
	