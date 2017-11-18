#include "Algorithms.h"

#include <algorithm>
#include <array>
#include <queue>
#include <map>
#include <set>

const std::array<vec2, 4> directions =
{
    {
        {-1, 0},
        {1, 0},
        {0, -1},
        {0,  1}
    }
};

std::set<vec2> get_cross_danger(const Map &map, const vec2 &center, int cross_size)
{
    std::set<vec2> danger_cross = {center};

    for (auto &direction : directions)
    {
        for (int i = 1; i <= cross_size; ++i)
        {
            vec2 pos{center.x + direction.x * i, center.y + direction.y * i};
            if (pos.x < 0 || pos.y < 0 || pos.x >= map.size() || pos.y >= map.size())
            {
                continue;
            }

            danger_cross.insert(pos);

            if (!is_free(map.get(pos)))
            {
                continue;
            }
        }
    }

    return danger_cross;
}

std::map<vec2, int> get_danger_points(const Map &map, const BombTimers &bomb_timers)
{
    std::map<vec2, int> danger_points;

    map.go_around([&](const vec2 &pos, MapObject map_object)
    {
        if (is_meat_chopper(map_object))
        {
            for (auto &danger_point : get_cross_danger(map, pos, 1))
            {
                danger_points[danger_point] = 1;
            }
        }
        else if (bomb_timers.has_bomb(pos))
        {
            for (auto &danger_point : get_cross_danger(map, pos, 3))
            {
                danger_points[danger_point] = std::min(danger_points[danger_point], bomb_timers.get_timer(pos));
            }
        }
    });

    return danger_points;
}

bool is_safe_point_at_move(const vec2 &pos, const std::map<vec2, int> &danger_points, int move_counter)
{
    auto danger_point_it = danger_points.find(pos);
    return danger_point_it == danger_points.end() || danger_point_it->second != move_counter;
}

std::vector<vec2> get_path_to_nearest_point(const Map &map, const vec2 &from_pos, const std::map<vec2, int> &danger_points,
                                            const std::function<bool(const vec2 &pos, MapObject map_object, int move_counter)> &is_search_point)
{
    std::vector<vec2> path;

    int move_counter = 0;
    std::set<vec2> visited;
    std::queue<vec2> to_visit;
    std::map<vec2, vec2> came_from;

    to_visit.push(from_pos);

    while (!to_visit.empty())
    {
        auto pos = to_visit.front();
        to_visit.pop();

        visited.insert(pos);

        move_counter += 1;

        for (auto &direction : directions)
        {
            vec2 new_pos{pos.x + direction.x, pos.y + direction.y};
            if (visited.count(new_pos) > 0 || new_pos.x < 0 || new_pos.y < 0 || new_pos.x >= map.size() || new_pos.y >= map.size())
            {
                continue;
            }

            auto map_object = map.get(new_pos);
            if (is_safe_point_at_move(new_pos, danger_points, move_counter) && is_free(map_object) || is_search_point(new_pos, map_object, move_counter))
            {
                came_from[new_pos] = pos;
                to_visit.push(new_pos);
                if (is_search_point(new_pos, map_object, move_counter))
                {
                    to_visit = {};

                    path.push_back(new_pos);

                    while (path.back() != from_pos)
                    {
                        path.push_back(came_from[path.back()]);
                    }
                    break;
                }
            }
        }
    }

    return path;
}

Command get_first_move(const std::vector<vec2> &path)
{
    if (path.size() > 1)
    {
        vec2 diff{path[path.size() - 2].x - path.back().x, path[path.size() - 2].y - path.back().y};

        for (int i = 0; i < (int)directions.size(); ++i)
        {
            if (directions[i] == diff)
            {
                return Command(i);
            }
        }
    }

    return Command::Nothing;
}

bool is_direct_path(const std::vector<vec2> &path)
{
    if (path.size() < 2)
    {
        return false;
    }

    int diff_x = path[0].x - path[1].x;
    int diff_y = path[0].y - path[1].y;

    for (std::size_t i = 0; i < path.size() - 1; ++i)
    {
        if (path[i].x - path[i + 1].x != diff_x || path[i].y - path[i + 1].y != diff_y)
        {
            return false;
        }
    }

    return true;
}

bool is_corner(const vec2 &a, const vec2 &b, const vec2 &c)
{
    return a.x - b.x != b.x - c.x &&
           a.y - b.y != b.y - c.y;
}

int get_corners_amount(const std::vector<vec2> &path)
{
    if (path.size() < 3)
    {
        return 0;
    }

    int corners_amount = 0;
    for (std::size_t i = 0; i < path.size() - 2; ++i)
    {
        if (is_corner(path[i], path[i + 1], path[i + 2]))
        {
            corners_amount += 1;
        }
    }

    return corners_amount;
}

bool has_corner_at_first_move(const std::vector<vec2> &path)
{
    if (path.size() < 3)
    {
        return false;
    }

    auto i = path.size() - 1;

    return is_corner(path[i], path[i - 1], path[i - 2]);
}
