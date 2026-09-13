#pragma once
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include "WorldState.h"
#include "CharacterState.h"

// 动作的"效果"：对一个属性施加归一化增减（如 +0.2 / -0.05）
struct ActionEffect {
    std::string attrName;
    double delta;   // 归一化增减，范围约 [-1,1]
};

// 动作基类：效用 AI 在决策阶段只读 effects 算分，
// 选中后才通过 perform() 把效果一次性应用到角色状态。
class Action {
public:
    std::string name;
    double intrinsic = 0.0;          // 非生存型动作的小基线分（偏好项）
    int durationTicks = 1;           // 持续 tick 数（commitment 时长）
    std::vector<ActionEffect> effects;

    virtual ~Action() = default;

    // 默认实现：把 effects 一次性应用到角色（配合 setAttr 内部 clamp 锁 0~1）
    virtual void perform(WorldState&, CharacterState& c) {
        for (const auto& e : effects)
            c.setAttr(e.attrName, c.getAttr(e.attrName) + e.delta);
    }
};

// ---- 六个具体动作：效用 AI 在它们之间做 argmax 选择 ----

class SnackAction : public Action {
public:
    SnackAction() {
        name = "吃零食"; durationTicks = 1;
        effects.push_back({ "Full", +0.2 });
        effects.push_back({ "Vigor", -0.05 });
    }
};

class SleepAction : public Action {
public:
    SleepAction() {
        name = "睡觉"; durationTicks = 8;
        effects.push_back({ "Sober", +0.4 });
        effects.push_back({ "Vigor", +0.3 });
        effects.push_back({ "Mood", +0.1 });
        effects.push_back({ "Social", -0.05 });
    }
};

class RestAction : public Action {
public:
    RestAction() {
        name = "休息"; durationTicks = 3;
        effects.push_back({ "Vigor", +0.15 });
        effects.push_back({ "Sober", +0.05 });
    }
};

class PlantAction : public Action {
public:
    PlantAction() {
        name = "种花"; durationTicks = 3;
        effects.push_back({ "Mood", +0.15 });
        effects.push_back({ "Vigor", -0.05 });
        intrinsic = 0.05;
    }
};

class CookAction : public Action {
public:
    CookAction() {
        name = "做饭"; durationTicks = 3;
        effects.push_back({ "Full", +0.3 });
        effects.push_back({ "Vigor", -0.1 });
        effects.push_back({ "Mood", +0.05 });
    }
};

// 和玩家互动：在本效用 AI 范围内是一个普通动作（补社交/心情）。
// 说明：完整架构里此动作可嵌套"LLM 闲聊 / RL 玩游戏"作为微观执行器，
// 但本仓库只覆盖效用 AI 元控制器部分，故此处用声明式效果表示。
class InteractAction : public Action {
public:
    InteractAction() {
        name = "和玩家互动"; durationTicks = 2;
        intrinsic = 0.1;
        effects.push_back({ "Social", +0.2 });
        effects.push_back({ "Mood", +0.15 });
        effects.push_back({ "Vigor", -0.1 });
    }
};
