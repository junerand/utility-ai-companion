#pragma once
#include <string>
#include <vector>
#include <algorithm>

struct CharacterAttribute {
    std::string name;
    double currentValue;
    double idealValue;
    double weight;
};

class CharacterState {
public:
    CharacterState() {
        // current 起步低于 ideal，才有"需求"驱动行为
        attrs.push_back({ "Mood",   0.5, 0.8, 1.0 });
        attrs.push_back({ "Social", 0.5, 0.7, 0.8 });
        attrs.push_back({ "Vigor",  0.5, 0.8, 1.0 });
        attrs.push_back({ "Sober",  0.5, 0.9, 1.0 });
        attrs.push_back({ "Full",   0.5, 0.8, 1.0 });
    }

    double getAttr(const std::string& name) const {
        for (const auto& a : attrs) if (a.name == name) return a.currentValue;
        return 0.0;
    }
    void setAttr(const std::string& name, double v) {
        for (auto& a : attrs)
            if (a.name == name) { a.currentValue = std::clamp(v, 0.0, 1.0); return; }
    }
    CharacterAttribute getAttribute(const std::string& name) const {
        for (const auto& a : attrs) if (a.name == name) return a;
        return { "", 0.0, 0.0, 0.0 };
    }

    void readCharacterState() {}   // 持久化：从文件载入
    void saveCharacterState() {}   // 持久化：写入文件

    const std::vector<CharacterAttribute>& all() const { return attrs; }

private:
    std::vector<CharacterAttribute> attrs;
};
