#include "Strategy.h"

#include <iostream>

#include "Algorithms.h"

std::vector<Command> Strategy::get_decisions(const Map &map)
{
    _bomb_timers.update(map);

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
        if (to_attack && path.size() <= 4 && is_direct_path(path))
        {
            danger_points[path[0]] = 1; // make the target in a danger to ignore him in searching
            path = get_path_to_nearest_other_player(map, me_pos, danger_points);

            return {Command::PlaceBomb, get_first_move(path)};
        }
        else
        {
            return {get_first_move(path)};
        }
    }
}
