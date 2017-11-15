#pragma once

#include "Map.h"
#include "Command.h"

Map make_danger_map(const Map &map);
Command get_next_move_to_nearest_other_bomberman(const vec2 &me_pos, Map &danger_map);