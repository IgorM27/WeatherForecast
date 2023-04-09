#include "Weather.h"


std::vector<std::string> Picture1 = {
        "                ",
        "      .--.      ",
        "   .-(    ).    ",
        "  (___.__)__)   ",
        "                ",
};

std::vector<std::string> Picture2 = {
        "     \\__|__/    ",
        "     (     )    ",
        "   -(       )-  ",
        "     (_____)    ",
        "     /  |  \\    ",
};

std::vector<std::string> Picture3 = {
        "      .-.       ",
        "     (   ).     ",
        "    (___(__)    ",
        "    * * * *     ",
        "   * * * *      ",
};

std::vector<std::string> Picture4 = {
        "                ",
        "  -_-_-_-_-_    ",
        "  _-_-_-_-_-    ",
        "  -_-_-_-_-_    ",
        "                ",
};

std::vector<std::string> Picture5 = {
        "      .-.       ",
        "     (   ).     ",
        "    (___(__)    ",
        "    *   *       ",
        "   *   *        ",
};
std::vector<std::string> Picture6 = {
        "      .-.       ",
        "     (   ).     ",
        "    (___(__)    ",
        "    / / / /     ",
        "   / / / /      ",
};

std::vector<std::string> Picture7 = {
        "      .-.       ",
        "     (   ).     ",
        "    (___(__)    ",
        "    /   /       ",
        "     /   /      ",
};

std::vector<std::string> Picture8 = {
        "      .-.       ",
        "     (   ).     ",
        "    (___(__)    ",
        "      \\\\      ",
        "       //       ",
};


std::vector<std::string> Months{"January", "February", "March", "April", "May", "June", "July", "August", "September",
                                "October", "November", "December"};

std::vector<std::string> Parameters{"Weather", "Temperature", "Wind", "Precipitation", "Surface_pressure"};

void WeatherForecast::CreateWeatherForecast() {
    for (auto& i: config["cities"]) {
        std::string tmp_city = i;
        std::vector<print_block> tmp;
        cities.push_back(UpdateCityInfo(tmp_city, tmp));
        print_cities.push_back(tmp);
    }
}

std::vector<std::vector<weather_block>>
WeatherForecast::UpdateCityInfo(const std::string& city, std::vector<print_block>& print_days) {
    cpr::Response coordinates = cpr::Get(cpr::Url{"https://api.api-ninjas.com/v1/city"},
                                         cpr::Parameters{{"name", city}},
                                         cpr::Header{{"X-Api-Key", "1v2DEz5HWpcfV1SvJ+MYLQ==pOqmRFfuY7j4iLc4"}});
    nlohmann::json json_coordinates = nlohmann::json::parse(coordinates.text);

    std::string latitude = std::to_string(static_cast<double>(json_coordinates[0]["latitude"]));
    std::string longitude = std::to_string(static_cast<double>(json_coordinates[0]["longitude"]));

    std::string weather_url = "https://api.open-meteo.com/v1/forecast?latitude=" + latitude + "&longitude=" + longitude
                              + "&hourly=temperature_2m," + "relativehumidity_2m," + "precipitation_probability,"
                              + "precipitation," + "weathercode," + "surface_pressure," + "windspeed_10m," +
                              "winddirection_10m"+"&forecast_days=10";
    cpr::Response weather = cpr::Get(cpr::Url{weather_url});

    nlohmann::json json_whether = nlohmann::json::parse(weather.text);

    std::vector<weather_block> day;
    std::vector<std::vector<weather_block>> days;
    for (int i = 0; i < json_whether["hourly"]["time"].size(); ++i) {
        weather_block tmp;
        tmp.time = json_whether["hourly"]["time"][i];
        tmp.precipitation = std::to_string(static_cast<double>(json_whether["hourly"]["precipitation"][i]));
        tmp.precipitation_probability = json_whether["hourly"]["precipitation_probability"][i];
        tmp.relative_humidity = json_whether["hourly"]["relativehumidity_2m"][i];
        tmp.surface_pressure = static_cast<int>(json_whether["hourly"]["surface_pressure"][i]);
        tmp.temperature = static_cast<int>(json_whether["hourly"]["temperature_2m"][i]);
        tmp.weather_code = json_whether["hourly"]["weathercode"][i];
        tmp.wind_direction = json_whether["hourly"]["winddirection_10m"][i];
        tmp.wind_speed = static_cast<int>(json_whether["hourly"]["windspeed_10m"][i]);
        day.push_back(tmp);
        if ((i + 1) % 24 == 0) {
            print_days.push_back(UpdatePrintDay(day));
            days.push_back(day);
            day = {};
        }
    }
    return days;
}

print_block WeatherForecast::UpdatePrintDay(std::vector<weather_block> day) {
    print_block block;
    block.month += day[0].time[5];
    block.month += day[0].time[6];
    int tmp_month = std::atoi(block.month.c_str());
    block.month = "";
    block.month += day[0].time[8];
    block.month += day[0].time[9];
    block.month += " " + Months[tmp_month - 1];
    for (size_t i = 0; i < day.size(); i += 6) {
        std::string tmp_wind;
        int min_wind = 1000;
        int max_wind = -1;
        int min_temperature = 1000;
        int max_temperature = -1000;
        for (int j = 0; j < 6; ++j) {
            if (day[i + j].wind_speed > max_wind) {
                max_wind = day[i + j].wind_speed;
            }
            if (day[i + j].wind_speed < min_wind) {
                min_wind = day[i + j].wind_speed;
            }
            if (day[i + j].temperature > max_temperature) {
                max_temperature = day[i + j].temperature;
            }
            if (day[i + j].temperature < min_temperature) {
                min_temperature = day[i + j].temperature;
            }
            if (j == 2) {
                tmp_wind = WindDirection(day[i + j].wind_direction) + " ";
                std::vector<std::string> tmp;
                block.weather.push_back("Weather: " + WeatherCode(day[i + j].weather_code, tmp));
                block.picture.push_back(tmp);
                block.surface_pressure.push_back("Surface pressure: " + std::to_string(day[i + j].surface_pressure));
                std::string tmp_precipitation;
                if (day[i + j].precipitation.size() <= 3) {
                    tmp_precipitation = day[i + j].precipitation;
                } else {
                    tmp_precipitation = day[i + j].precipitation.substr(0, 3);
                    if (day[i + j].precipitation[3] == '5' || day[i + j].precipitation[3] == '6' ||
                        day[i + j].precipitation[3] == '7' || day[i + j].precipitation[3] == '8' ||
                        day[i + j].precipitation[3] == '9') {
                        double tmp_double = std::atof(tmp_precipitation.c_str()) + 0.1;
                        tmp_precipitation = std::to_string(tmp_double);
                        tmp_precipitation = tmp_precipitation.substr(0, 3);
                    }
                }
                tmp_precipitation += " | " + std::to_string(day[i + j].precipitation_probability) + "%";
                block.precipitation.push_back("Precipitation: " + tmp_precipitation);
            }
        }
        tmp_wind += std::to_string(min_wind) + "-" + std::to_string(max_wind) + " km/h";
        block.wind.push_back("Wind: " + tmp_wind);
        if (max_temperature > 0) {
            block.temperature.push_back("Temperature: +" +
                                        (std::to_string(max_temperature) + "(" + std::to_string(min_temperature) +
                                         ") C"));
        } else {
            block.temperature.push_back("Temperature: " +
                                        (std::to_string(max_temperature) + "(" + std::to_string(min_temperature) +
                                         ") C"));
        }
    }
    return block;
}

std::string WeatherForecast::WindDirection(int code) {
    if ((code >= 0 && code <= 22) || (code >= 338 && code <= 360)) {
        return "N";
    }
    if (code >= 23 && code <= 67) {
        return "NE";
    }
    if (code >= 68 && code <= 112) {
        return "E";
    }
    if (code >= 113 && code <= 157) {
        return "SE";
    }
    if (code >= 158 && code <= 202) {
        return "S";
    }
    if (code >= 203 && code <= 247) {
        return "SW";
    }
    if (code >= 248 && code <= 292) {
        return "W";
    }
    return "NW";
}

std::string WeatherForecast::WeatherCode(int code, std::vector<std::string>& picture) {
    if (code == 0) {
        picture = Picture2;
        return "Sunny";
    }
    if (code == 1 || code == 2 || code == 3) {
        picture = Picture1;
        return "Cloudy";
    }
    if (code == 45 || code == 48) {
        picture = Picture4;
        return "Fog";
    }
    if (code == 51 || code == 53 || code == 55) {
        picture = Picture7;
        return "Drizzle";
    }
    if (code == 56 || code == 57) {
        picture = Picture5;
        return "Icy drizzle";
    }
    if (code == 61 || code == 63 || code == 65 || code == 66 || code == 67 || code == 80 || code == 81 || code == 82) {
        picture = Picture6;
        return "Rain";
    }
    if (code == 71 || code == 73 || code == 75 || code == 77 || code == 85 || code == 86) {
        picture = Picture3;
        return "Snow";
    }
    picture = Picture8;
    return "Thunderstorm";
}

void WeatherForecast::PrintInCenter(const std::string& value, size_t length) {
    size_t start = length / 2 - (value.size() + 4) / 2;
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < length; ++j) {
            if ((i == 0 || i == 2) && j == start) {
                for (size_t k = 0; k < value.size() + 4; ++k) {
                    std::cout << char(-60);
                }
                j += value.size() + 4;
                continue;
            }
            if (i == 1 && j == start) {
                std::cout << char(-77) << ' ' << value << ' ' << char(-77);
                j += value.size() + 4;
                continue;
            }
            std::cout << ' ';
        }
        std::cout << '\n';
    }
}

void WeatherForecast::PrintLine(size_t length) {
    for (size_t i = 0; i < length; ++i) {
        std::cout << char(-60);
    }
    std::cout << '\n';
}

void WeatherForecast::AddSpace(size_t size, size_t count) {
    for (size_t i = size; i < count; ++i) {
        std::cout << ' ';
    }
}

void WeatherForecast::PrintWeather() {
    PrintInCenter(config["cities"][iterator], kSize);
    for (size_t i = 0; i < number_days; ++i) {
        PrintOneDay(print_cities[iterator][i]);
    }
}

void WeatherForecast::UpdateInfo() {
    frequency = (((clock() / 1000) / frequency_delta) + 1) * frequency_delta;
    std::vector<std::vector<print_block>> tmp_print_cities{};
    std::vector<std::vector<std::vector<weather_block>>> tmp_cities{};
    for (auto& i: config["cities"]) {
        std::string tmp_city = i;
        std::vector<print_block> tmp;
        tmp_cities.push_back(UpdateCityInfo(tmp_city, tmp));
    }
    if (tmp_cities.size() == config["cities"].size() && tmp_print_cities.size() == config["cities"].size()) {
        cities = tmp_cities;
        print_cities = tmp_print_cities;
    }
}

void WeatherForecast::NextCity(bool problem) {
    iterator = (iterator + 1) % config["cities"].size();
    system("cls");
    if (problem) {
        std::cout << "Check your internet connection to get up-to-date weather data" << '\n';
    }
    PrintWeather();
}

void WeatherForecast::PreviousCity(bool problem) {
    iterator = ((iterator + cities.size()) - 1) % cities.size();
    system("cls");
    if (problem) {
        std::cout << "Check your internet connection to get up-to-date weather data" << '\n';
    }
    PrintWeather();
}

void WeatherForecast::AddDay(bool problem) {
    if (number_days != 10) {
        number_days++;
    }
    system("cls");
    if (problem) {
        std::cout << "Check your internet connection to get up-to-date weather data" << '\n';
    }
    PrintWeather();
}

void WeatherForecast::RemoveDay(bool problem) {
    if (number_days != 1) {
        number_days--;
    }
    system("cls");
    if (problem) {
        std::cout << "Check your internet connection to get up-to-date weather data" << '\n';
    }
    PrintWeather();
}

size_t WeatherForecast::GetFrequency() const {
    return frequency;
}

void WeatherForecast::PrintOneDay(print_block print) {
    PrintInCenter(print.month, kSize);
    PrintLine(kSize);
    for (size_t i = 0; i < kSize; ++i) {
        if (i == 0 || i == 41 || i == 82 || i == 123 || i == 164) {
            std::cout << char(-77);
            continue;
        }
        if (i == 19) {
            std::cout << "Night";
            i += 4;
            continue;
        }
        if (i == 59) {
            std::cout << "Morning";
            i += 6;
            continue;
        }
        if (i == 102) {
            std::cout << "Day";
            i += 2;
            continue;
        }
        if (i == 140) {
            std::cout << "Evening";
            i += 6;
            continue;
        }
        std::cout << ' ';
    }
    std::cout << '\n';
    PrintLine(kSize);
    size_t counter = 0;
    for (size_t i = 0; i < 5; ++i) {
        std::cout << char(-77);
        for (size_t j = 0; j < 4; ++j) {
            if (j != 0) {
                std::cout << char(-77);
            }
            std::cout << print.picture[j][i];
            bool space = false;
            for (; counter < Parameters.size(); ++counter) {
                if (config[Parameters[counter]] == "true") {
                    if (counter == 0) {
                        std::cout << print.weather[j];
                        AddSpace(print.picture[j][i].size() + print.weather[j].size(), kBlockSize);
                        space = true;
                        break;
                    }
                    if (counter == 1) {
                        std::cout << print.temperature[j];
                        AddSpace(print.picture[j][i].size() + print.temperature[j].size(), kBlockSize);
                        space = true;
                        break;
                    }
                    if (counter == 2) {
                        std::cout << print.wind[j];
                        AddSpace(print.picture[j][i].size() + print.wind[j].size(), kBlockSize);
                        space = true;
                        break;
                    }
                    if (counter == 3) {
                        std::cout << print.precipitation[j];
                        AddSpace(print.picture[j][i].size() + print.precipitation[j].size(), kBlockSize);
                        space = true;
                        break;
                    }
                    if (counter == 4) {
                        std::cout << print.surface_pressure[j];
                        AddSpace(print.picture[j][i].size() + print.surface_pressure[j].size(), kBlockSize);
                        space = true;
                        break;
                    }
                }
            }
            if (!space) {
                AddSpace(print.picture[j][i].size(), kBlockSize);
            }

        }
        counter++;
        std::cout << char(-77) << '\n';
    }
    PrintLine(kSize);
    std::cout << '\n';
}

void WeatherForecast::Settings(bool problem) {
    while (true) {
        system("cls");
        std::cout << "Settings:" << '\n' << "Please press the button:" << '\n'
                  << "1 if you want to add a weather parameter" << '\n'
                  << "2 if you want to remove the weather parameter" << '\n' << "To return, press <" << '\n';
        int command = getch();
        if (command == 49) {
            AddWeatherParameter();
        }
        if (command == 50) {
            RemoveWeatherParameter();
        }
        if (command == 60) {
            system("cls");
            if (problem) {
                std::cout << "Check your internet connection to get up-to-date weather data" << '\n';
            }
            PrintWeather();
            break;
        }
    }


}

void WeatherForecast::AddWeatherParameter() {
    while (true) {
        system("cls");
        std::cout << "Add weather parameter:" << '\n' << "Please press the button:" << '\n';
        for (size_t i = 0; i < 5; ++i) {
            if (i != 4) {
                std::cout << i + 1 << " if you want to add a " << Parameters[i];
            } else {
                std::cout << i + 1 << " if you want to add a Surface pressure";
            }
            if (config[Parameters[i]] == "true") {
                std::cout << " +" << '\n';
            } else {
                std::cout << " -" << '\n';
            }
        }
        std::cout << "To return, press <" << '\n';
        int command = getch();
        if (command >= 49 && command <= 53) {
            config[Parameters[command - 49]] = "true";
        }
        if (command == 60) {
            break;
        }
    }

}

void WeatherForecast::RemoveWeatherParameter() {
    while (true) {
        system("cls");
        std::cout << "Remove weather parameter:" << '\n' << "Please press the button:" << '\n';
        for (size_t i = 0; i < 5; ++i) {
            if (i != 4) {
                std::cout << i + 1 << " if you want to remove a " << Parameters[i];
            } else {
                std::cout << i + 1 << " if you want to remove a Surface pressure";
            }
            if (config[Parameters[i]] == "true") {
                std::cout << " +" << '\n';
            } else {
                std::cout << " -" << '\n';
            }
        }
        std::cout << "To return, press <" << '\n';
        int command = getch();
        if (command >= 49 && command <= 53) {
            config[Parameters[command - 49]] = "false";
        }
        if (command == 60) {
            break;
        }
    }
}
