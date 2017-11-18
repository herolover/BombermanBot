#pragma once

#include <map>

#include "Map.h"

class BombTimers
{
private:
    std::map<vec2, int> _bombs;

    void check_bombs();
public:
    void update(const Map &map);

    bool has_bomb(const vec2 &pos) const
    {
        return _bombs.find(pos) != _bombs.end();
    }

    int get_timer(const vec2 &pos) const
    {
        return _bombs.at(pos);
    }

    void add_bomb(const vec2 &pos, int timer)
    {
        _bombs[pos] = timer;
    }
};
