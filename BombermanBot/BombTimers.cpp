#include "BombTimers.h"

#include <set>

void BombTimers::check_bombs()
{
    for (auto bomb_it = _bombs.begin(); bomb_it != _bombs.end();)
    {
        bomb_it->second -= 1;
        if (bomb_it->second == 0)
        {
            bomb_it = _bombs.erase(bomb_it);
        }
        else
        {
            ++bomb_it;
        }
    }
}

void BombTimers::update(const Map &map)
{
    check_bombs();

    map.go_around([this](const vec2 &pos, MapObject map_object)
    {
        if (is_bomb(map_object) && _bombs.find(pos) == _bombs.end())
        {
            _bombs[pos] = get_bomb_timer(map_object);
        }
    });
}
