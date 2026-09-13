#pragma once
#include <algorithm>
#include <cmath>

// 响应曲线：输入当前值与理想值，返回 0..1 的"急迫度"
// 偏差越大急迫度越高；二次曲线在偏差小时≈0，形成软阈值（无需硬 if 判断）
inline double responseCurve(double current, double ideal) {
    double dev = std::abs(ideal - current);
    return std::clamp(dev * dev, 0.0, 1.0);
}
