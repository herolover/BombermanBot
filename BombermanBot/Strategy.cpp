#include "Strategy.h"

#include <iostream>

#include "Algorithms.h"

bool Strategy::can_place_bomb() const
{
    return !_bomb_timers.has_bomb(_my_bomb_pos);
}

std::vector<Command> Strategy::get_decisions(const Map &map)
{
    _bomb_timers.update(map);
    if (can_place_bomb())
    {
        _my_bomb_pos = vec2{-1, -1};
    }

    auto danger_points = get_danger_points(map, _bomb_timers);
    auto me_pos = map.get_me_coords();

    bool to_attack = true;
    std::vector<vec2> path = get_path_to_nearest_other_player(map, me_pos, danger_points);
    if (path.empty() && danger_points.find(me_pos) != danger_points.end())
    {
        to_attack = false;
        path = get_path_to_nearest_point(map, me_pos, danger_points,
                                         [&](const vec2 &pos, MapObject map_object, int move_counter)
        {
            return is_safe_point_at_move(pos, danger_points, move_counter) && is_free(map_object);
        });
    }

    if (path.empty())
    {
        return {Command::PlaceBomb};
    }
    else
    {
        if (to_attack && can_place_bomb()
                && path.size() <= 5 && (is_direct_path(path)
                                        || get_corners_amount(path) == 1 && has_corner_at_first_move(path)))
        {
            _my_bomb_pos = path[path.size() - 2];
            return {get_first_move(path), Command::PlaceBomb};
        }

        return {get_first_move(path)};
    }
}
