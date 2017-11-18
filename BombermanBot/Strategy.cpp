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
    if ((path.empty() || path.size() == 2) && danger_points.find(me_pos) != danger_points.end())
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
        if (to_attack && can_place_bomb())
        {
            bool direct_path = is_direct_path(path);
            if (path.size() <= 5 && get_corners_amount(path) == 1 && has_corner_at_first_move(path) ||
                    path.size() == 5 && direct_path)
            {
                _my_bomb_pos = path[path.size() - 2];
                _bomb_timers.add_bomb(_my_bomb_pos, 5);
                return {get_first_move(path), Command::PlaceBomb};
            }
            else if (path.size() <= 4 && direct_path)
            {
                _my_bomb_pos = me_pos;
                danger_points[path[0]] = 1; // make the target in a danger to ignore him in searching
                path = get_path_to_nearest_other_player(map, me_pos, danger_points);

                return {Command::PlaceBomb, get_first_move(path)};
            }
        }

        return {get_first_move(path)};
    }
}
