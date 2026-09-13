#pragma once
#include <string>
#include <chrono>
#include <ctime>
#include <cstdlib>

class WorldState {
public:
    WorldState() : timeOfDay(getSystemTimeOfDay()), weather("晴"), tickCount(0) {}

    // 读取系统时间，返回 0..24 的浮点小时
    static double getSystemTimeOfDay() {
        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        std::tm* local = std::localtime(&t);
        if (!local) return 12.0;
        return local->tm_hour + local->tm_min / 60.0;
    }

    bool isDay() const { return timeOfDay >= 6.0 && timeOfDay < 18.0; }

    void advance() {
        timeOfDay = getSystemTimeOfDay();       // 昼夜跟随真实时间
        ++tickCount;
        // 演示用：每 20 tick 随机换一次天气
        if (tickCount % 20 == 0) {
            const char* opts[] = { "晴", "雨", "阴" };
            weather = opts[std::rand() % 3];
        }
    }

    const std::string& getWeather() const { return weather; }
    void setWeather(const std::string& w) { weather = w; }
    int  getTickCount() const { return tickCount; }
    double getTimeOfDay() const { return timeOfDay; }

private:
    double timeOfDay;
    std::string weather;
    int tickCount;
};
