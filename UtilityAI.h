#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include "Action.h"
#include "ResponseCurve.h"

// 评分：对每个属性算 improve，加权求和
inline double scoreAction(const Action& act, const WorldState&, const CharacterState& c) {
    double total = act.intrinsic;
    for (const auto& e : act.effects) {
        CharacterAttribute a = c.getAttribute(e.attrName);
        double dev = std::abs(a.idealValue - a.currentValue);
        double drive = responseCurve(a.currentValue, a.idealValue);
        double newVal = std::clamp(a.currentValue + e.delta, 0.0, 1.0);
        double improve = dev - std::abs(a.idealValue - newVal);  // ≥0 朝理想靠近
        total += a.weight * drive * improve;
    }
    return total;
}

// 选择：argmax（配合 Simulator 的 commitment 防抖动）
inline Action* chooseAction(const std::vector<std::unique_ptr<Action>>& actions,
                            const WorldState& w, const CharacterState& c) {
    Action* best = nullptr;
    double bestScore = -1e9;
    for (const auto& a : actions) {
        double s = scoreAction(*a, w, c);
        if (s > bestScore) { bestScore = s; best = a.get(); }
    }
    return best;
}
