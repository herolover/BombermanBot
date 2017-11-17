#pragma once

#include "BaseStrategy.h"
#include "BombTimers.h"

class Strategy : public BaseStrategy
{
private:
    BombTimers _bomb_timers;
public:
    std::vector<Command> get_decisions(const Map &map) override;
};