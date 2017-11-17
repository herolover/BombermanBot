#pragma once

#include "Command.h"
#include "Map.h"

class BaseStrategy
{
public:
    virtual std::vector<Command> get_decisions(const Map &map) = 0;
};
