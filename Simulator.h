#pragma once
#include <vector>
#include <memory>
#include "WorldState.h"
#include "CharacterState.h"
#include "Action.h"
#include "UtilityAI.h"
#include "Dynamics.h"

class Simulator {
public:
    // 推进一步：推进世界 → 自然衰减 → （边界处）决策并执行一次动作
    void step(const std::vector<std::unique_ptr<Action>>& actions,
              WorldState& w, CharacterState& c) {
        w.advance();
        applyDynamics(w, c);

        if (ticksLeft <= 0) {                  // 只在动作边界重新决策
            current = chooseAction(actions, w, c);
            ticksLeft = current ? current->durationTicks : 1;
            if (current) current->perform(w, c);   // 动作效果一次性应用
        }
        --ticksLeft;
    }

    const Action* currentAction() const { return current; }

private:
    Action* current = nullptr;
    int ticksLeft = 0;
};
