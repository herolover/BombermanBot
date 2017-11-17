#pragma once

#include <map>
#include <functional>

#include "Map.h"
#include "Command.h"
#include "BombTimers.h"

std::map<vec2, int> get_danger_points(const Map &map, const BombTimers &bomb_timers);
bool is_safe_point_at_move(const vec2 &pos, const std::map<vec2, int> &danger_points, int move_counter);
std::vector<vec2> get_path_to_nearest_point(const Map &map, const vec2 &from_pos, const std::map<vec2, int> &danger_points,
                                            const std::function<bool(const vec2 &pos, MapObject map_object, int move_counter)> &is_search_point);
Command get_first_move(const std::vector<vec2> &path);

bool is_direct_path(const std::vector<vec2> &path);

inline std::vector<vec2> get_path_to_nearest_other_player(const Map &map, const vec2 &from_pos, const std::map<vec2, int> &danger_points)
{
    return get_path_to_nearest_point(map, from_pos, danger_points,
                                     [&](const vec2 &pos, MapObject map_object, int move_counter)
    {
        return danger_points.find(pos) == danger_points.end() && map_object == MapObject::OtherBomberman;
    });
}

inline std::vector<vec2> get_path_to_nearest_safe_point(const Map &map, const vec2 &from_pos, const std::map<vec2, int> &danger_points)
{
    return get_path_to_nearest_point(map, from_pos, danger_points,
                                     [&](const vec2 &pos, MapObject map_object, int move_counter)
    {
        return is_safe_point_at_move(pos, danger_points, move_counter) && is_free(map_object);
    });
}
