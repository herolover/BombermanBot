#pragma once

#include <vector>
#include <string>

enum class Command : int
{
    MoveLeft = 0,
    MoveRight,
    MoveUp,
    MoveDown,
    PlaceBomb
};

inline std::string command_to_string(Command command)
{
    return command == Command::MoveLeft ? "LEFT" :
        command == Command::MoveRight ? "RIGHT" :
        command == Command::MoveUp ? "UP" :
        command == Command::MoveDown ? "DOWN" :
        command == Command::PlaceBomb ? "ACT" : "";
}

inline std::string commands_to_string(std::vector<Command> &&commands)
{
    std::string commands_string;

    if (commands.size() > 0)
    {
        commands_string.append(command_to_string(commands[0]));
    }
    if (commands.size() > 1)
    {
        commands_string.append(",");
        commands_string.append(command_to_string(commands[1]));
    }

    return commands_string;
}
