#include "Strategy.h"

#include "Algorithms.h"

std::vector<Command> handler(Map &map)
{
    auto me_pos = map.get_me_coords();

    auto danger_map = make_danger_map(map);

    auto command = get_next_move_to_nearest_other_bomberman(me_pos, danger_map);

    std::vector<Command> commands;
    commands = {command};

    return commands;
}

