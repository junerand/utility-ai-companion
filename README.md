# Utility AI · 虚拟角色自主行为决策系统（元控制器）

一套需求驱动（Drive-based）的效用 AI（Utility AI）元控制器：角色根据昼夜、天气与五项身心属性（心情 / 社交 / 活力 / 清醒 / 饱腹）自主决策行为，无需硬编码脚本。

> 范围说明：本仓库**仅包含效用 AI 元控制器**。原项目还规划了"RL 执行器（玩游戏）"与"LLM 反思层（写日记/调权重）"，不在此仓范围内，但接口已预留。

## 设计要点

| 模块 | 文件 | 说明 |
|------|------|------|
| 世界状态 | `WorldState.h` | 昼夜（跟随系统时间）、天气、tick 计数 |
| 角色状态 | `CharacterState.h` | 5 项属性，各含 `current / ideal / weight` |
| 响应曲线 | `ResponseCurve.h` | 软阈值：`drive = clamp((ideal−cur)², 0, 1)` |
| 动作 | `Action.h` | 6 个动作 + 声明式 `ActionEffect`（对各属性的增减） |
| 效用评分 | `UtilityAI.h` | `score = Σ w·drive·improve`，argmax 选最高分 |
| 动态 | `Dynamics.h` | 每 tick 自然衰减 + 昼夜/天气上下文修正 |
| 模拟器 | `Simulator.h` | tick 主循环 + **commitment 防抖动** |

## 关键算法

**响应曲线（软阈值）**
```cpp
double dev = abs(ideal - current);
drive = clamp(dev * dev, 0.0, 1.0);   // 偏差越大越急迫；小时≈0，无需硬 if
```

**效用评分（防过饱和）**
```cpp
improve = |ideal - cur| - |ideal - new|;   // ≥0 朝理想靠近；<0 表示过冲惩罚
score   = Σ (weight × drive × improve) + intrinsic;
```
`improve` 项保证：已满足的需求（drive≈0 或已是理想值）不会反复被选中，也避免对单一属性"加过头"。

**Commitment 防抖动**
模拟器只在 `ticksLeft ≤ 0`（动作边界）重新决策一次，并让选中动作持续 `durationTicks` 个 tick，避免每 tick 在相邻动作间高频切换。

## 构建与运行

```bash
g++ -std=c++17 -O2 main.cpp -o utility_ai_demo
./utility_ai_demo
```

程序会跑 40 个 tick，打印每 tick 的昼夜/天气、当前动作与五项属性值。典型行为序列：社交/心情偏低 →「和玩家互动」；清醒偏低 →「睡觉」；恢复后再转其他动作。

## 目录结构
```
WorldState.h  CharacterState.h  ResponseCurve.h
Action.h      UtilityAI.h       Dynamics.h  Simulator.h
main.cpp      README.md  LICENSE  .gitignore
```
