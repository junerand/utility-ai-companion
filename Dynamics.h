#pragma once
#include "WorldState.h"
#include "CharacterState.h"

// 每 tick 的属性自然变化（衰减 + 上下文修正）
inline void applyDynamics(WorldState& w, CharacterState& c) {
    c.setAttr("Full",   c.getAttr("Full")   - 0.01);                        // 饱腹随时间降
    c.setAttr("Sober",  c.getAttr("Sober")  + (w.isDay() ? 0.01 : -0.02)); // 清醒：白天升夜晚降
    c.setAttr("Vigor",  c.getAttr("Vigor")  + ((w.getWeather() == "雨") ? -0.01 : 0.0)); // 活力：雨天降
    c.setAttr("Social", c.getAttr("Social") - 0.005);                      // 社交随时间降
    double moodAdj = (w.isDay() ? 0.005 : -0.005)                          // 心情受昼夜+天气
                   + ((w.getWeather() == "晴") ? 0.005 : -0.005);
    c.setAttr("Mood", c.getAttr("Mood") + moodAdj);
}
