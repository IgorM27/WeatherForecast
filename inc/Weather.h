#pragma once

#include "iostream"
#include "cpr/cpr.h"
#include "conio.h"
#include <nlohmann/json.hpp>
#include "string"
#include "cmath"
#include "ctime"

const int kSize = 165;
const int kBlockSize = 40;

struct weather_block {
    std::string time;
    std::string precipitation{};
    int precipitation_probability{};
    int relative_humidity{};
    int surface_pressure{};
    int temperature{};
    int weather_code{};
    int wind_direction{};
    int wind_speed{};
};

struct print_block {
    std::vector<std::string> weather;
    std::vector<std::string> temperature;
    std::vector<std::string> wind;
    std::vector<std::string> precipitation;
    std::vector<std::string> surface_pressure;
    std::vector<std::vector<std::string>> picture;
    std::string month;
};

class WeatherForecast {
private:
    std::vector<std::vector<print_block>> print_cities{};
    std::vector<std::vector<std::vector<weather_block>>> cities{};
    nlohmann::json config;
    size_t iterator = 0;
    size_t number_days = 0;
    size_t frequency_delta;
    size_t frequency;
public:
    WeatherForecast() {
        std::filesystem::path config_path = "Config.txt";
        std::ifstream input(config_path, std::ios::binary);
        config = nlohmann::json::parse(input);
        if (config["days"] >= 10) {
            number_days = 10;
        } else if (config["days"] >= 1) {
            number_days = config["days"];
        }
        frequency_delta = config["frequency"];
        frequency = config["frequency"];
    }

    void CreateWeatherForecast();

    static std::vector<std::vector<weather_block>>
    UpdateCityInfo(const std::string& city, std::vector<print_block>& print_days);

    static print_block UpdatePrintDay(std::vector<weather_block> day);

    static std::string WindDirection(int code);

    static std::string WeatherCode(int code, std::vector<std::string>& picture);

    static void PrintInCenter(const std::string& value, size_t length);

    static void PrintLine(size_t length);

    static void AddSpace(size_t size, size_t count);

    void PrintOneDay(print_block print);

    void PrintWeather();

    void UpdateInfo();

    void NextCity(bool problem);

    void PreviousCity(bool problem);

    void AddDay(bool problem);

    void RemoveDay(bool problem);

    [[nodiscard]] size_t GetFrequency() const;

    void Settings(bool problem);

    void AddWeatherParameter();

    void RemoveWeatherParameter();

    void AddCity();

};