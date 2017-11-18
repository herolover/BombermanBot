#pragma once

#include "BaseStrategy.h"
#include "BombTimers.h"

class Strategy : public BaseStrategy
{
private:
    BombTimers _bomb_timers;
    vec2 _my_bomb_pos;

    bool can_place_bomb() const;
public:
    std::vector<Command> get_decisions(const Map &map) override;
};