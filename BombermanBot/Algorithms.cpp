#include "Algorithms.h"

#include <algorithm>
#include <array>
#include <queue>
#include <map>
#include <set>

void mark_danger_cross(Map &map, const vec2 &center, int danger_length)
{
    const static std::array<vec2, 4> directions =
    {
        {
            {-1, 0},
            { 1, 0},
            {0, -1},
            {0,  1}
        }
    };

    for (auto &direction : directions)
    {
        for (int i = 1; i <= danger_length; ++i)
        {
            vec2 pos{center.x + direction.x * i, center.y + direction.y * i};
            if (pos.x < 0 || pos.y < 0 || pos.x >= map.size() || pos.y >= map.size())
            {
                break;
            }

            if (is_free(map.get(pos)))
            {
                map.set(pos, MapObject::Danger);
            }
        }
    }
}

Map make_danger_map(const Map &map)
{
    Map danger_map(map);

    for (int y = 0; y < map.size(); ++y)
    {
        for (int x = 0; x < map.size(); ++x)
        {
            vec2 pos{x, y};
            auto map_object = map.get(pos);
            if (is_meat_chopper(map_object))
            {
                mark_danger_cross(danger_map, pos, 1);
            }
            else if (is_bomb(map_object))
            {
                mark_danger_cross(danger_map, pos, 3);
            }
        }
    }

    return danger_map;
}

Command get_next_move_to_nearest_other_bomberman(const vec2 &me_pos, Map &map)
{
    const static std::array<vec2, 4> directions =
    {
        {
            {-1, 0},
            { 1, 0},
            {0, -1},
            {0,  1}
        }
    };

    std::vector<vec2> path;

    std::set<vec2> visited;
    std::queue<vec2> to_visit;
    std::map<vec2, vec2> came_from;

    to_visit.push(me_pos);

    while (!to_visit.empty())
    {
        auto pos = to_visit.front();
        to_visit.pop();

        visited.insert(pos);

        for (auto &direction : directions)
        {
            vec2 new_pos{pos.x + direction.x, pos.y + direction.y};
            if (visited.count(new_pos) > 0 || pos.x < 0 || pos.y < 0 || pos.x >= map.size() || pos.y >= map.size())
            {
                continue;
            }
            auto map_object = map.get(new_pos);
            if (is_free(map_object) || is_other_player(map_object))
            {
                came_from[new_pos] = pos;
                to_visit.push(new_pos);
                if (is_other_player(map_object))
                {
                    to_visit = {};

                    path.push_back(new_pos);

                    while (path.back() != me_pos)
                    {
                        path.push_back(came_from[path.back()]);
                    }
                    break;
                }
            }
        }
    }

    if (path.size() > 2)
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

    return Command::PlaceBomb;
}
