#include <iostream>
#include "WorldState.h"
#include "CharacterState.h"
#include "Action.h"
#include "Simulator.h"

int main() {
    WorldState w;
    CharacterState c;

    std::vector<std::unique_ptr<Action>> actions;
    actions.push_back(std::make_unique<SnackAction>());
    actions.push_back(std::make_unique<SleepAction>());
    actions.push_back(std::make_unique<RestAction>());
    actions.push_back(std::make_unique<PlantAction>());
    actions.push_back(std::make_unique<CookAction>());
    actions.push_back(std::make_unique<InteractAction>());

    Simulator sim;

    for (int i = 0; i < 40; ++i) {
        sim.step(actions, w, c);
        std::cout << "tick " << w.getTickCount()
                  << " | " << (w.isDay() ? "白天" : "夜晚")
                  << " 天气:" << w.getWeather()
                  << " | 动作:" << sim.currentAction()->name
                  << " | 心:" << c.getAttr("Mood")
                  << " 社:" << c.getAttr("Social")
                  << " 活:" << c.getAttr("Vigor")
                  << " 醒:" << c.getAttr("Sober")
                  << " 饱:" << c.getAttr("Full") << "\n";
    }
    return 0;
}
