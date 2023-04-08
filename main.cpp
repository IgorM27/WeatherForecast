#include "conio.h"
#include "Weather.h"

int main() {
    WeatherForecast weather;
    bool problem_create = false;
    while (true) {
        try {
            weather.CreateWeatherForecast();
            break;
        } catch (...) {
            if (!problem_create) {
                std::cout
                        << "It is impossible to get weather information. Check your internet connection or try again later."
                        << '\n';
            }
            problem_create = true;
        }
    }
    if (problem_create) {
        system("cls");
    }
    weather.PrintWeather();
    bool problem = false;
    while (true) {
        int command = getch();
        if (problem || (clock() / 1000) >= weather.GetFrequency()) {
            try {
                weather.UpdateInfo();
                problem = false;
            } catch (...) {
                problem = true;
            }
        }
        if (command == 27) {
            break;
        }
        if (command == 110) {
            weather.NextCity(problem);
        }
        if (command == 112) {
            weather.PreviousCity(problem);
        }
        if (command == 43) {
            weather.AddDay(problem);
        }
        if (command == 45) {
            weather.RemoveDay(problem);
        }
        if (command == 115) {
            weather.Settings(problem);
        }
    }
    return 0;
}
