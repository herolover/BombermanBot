#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <tuple>
#include <functional>
#include <map>

#include <Windows.h>

enum class MapObject : wchar_t
{
	Unknown = 0,

	Bomberman = L'☺',
	BombBomberman = L'☻',
	DeadBomberman = L'Ѡ',

	OtherBomberman = L'♥',
	OtherBombBomberman = L'♠',
	OtherDeadBomberman = L'♣',

	BombTimer5 = L'5',
	BombTimer4 = L'4',
	BombTimer3 = L'3',
	BombTimer2 = L'2',
	BombTimer1 = L'1',
	Boom = L'҉',

	Wall = L'☼',
	WallDestroyable = L'#',
	DestroyedWall = L'H',

	MeatChopper = L'&',
	DeadMeatChopper = L'x',

	Space = L' ',
};

inline int get_bomb_timer(MapObject map_object)
{
    const static std::map<MapObject, int> timers =
    {
        {MapObject::BombTimer1, 1},
        {MapObject::BombTimer2, 2},
        {MapObject::BombTimer3, 3},
        {MapObject::BombTimer4, 4},
        {MapObject::BombTimer5, 5},
        {MapObject::BombBomberman, 5},
        {MapObject::OtherBombBomberman, 5},
    };

    return timers.at(map_object);
}

inline bool is_other_player(MapObject map_object)
{
    return map_object == MapObject::OtherBomberman || map_object == MapObject::OtherBombBomberman;
}

inline bool is_me(MapObject map_object)
{
    return map_object == MapObject::Bomberman || map_object == MapObject::BombBomberman || map_object == MapObject::DeadBomberman;
}

inline bool is_bomb(MapObject map_object)
{
    return map_object == MapObject::BombTimer1 || map_object == MapObject::BombTimer2 || map_object == MapObject::BombTimer3 ||
        map_object == MapObject::BombTimer4 || map_object == MapObject::BombTimer5 ||
        map_object == MapObject::BombBomberman || map_object == MapObject::OtherBombBomberman;
}

inline bool is_wall(MapObject map_object)
{
    return map_object == MapObject::Wall || map_object == MapObject::WallDestroyable;
}

inline bool is_meat_chopper(MapObject map_object)
{
    return map_object == MapObject::MeatChopper;
}

inline bool is_free(MapObject map_object)
{
    return map_object == MapObject::Space || map_object == MapObject::Boom || map_object == MapObject::DestroyedWall ||
        map_object == MapObject::DeadMeatChopper || map_object == MapObject::DeadBomberman || map_object == MapObject::OtherDeadBomberman ||
        is_me(map_object);
}

struct vec2
{
    int x = 0;
    int y = 0;

    bool operator==(const vec2 &other) const
    {
        return std::tie(x, y) == std::tie(other.x, other.y);
    }

    bool operator!=(const vec2 &other) const
    {
        return !(*this == other);
    }

    bool operator<(const vec2 &other) const
    {
        return std::tie(x, y) < std::tie(other.x, other.y);
    }
};

class Map
{
private:
    int _map_size;
    std::vector<std::underlying_type_t<MapObject>> _map;

    const std::string _board_prefix = "board=";

    std::size_t get_address(int x, int y) const
    {
        return y * _map_size + x + _board_prefix.size();
    }

    std::size_t get_address(const vec2 &pos) const
    {
        return get_address(pos.x, pos.y);
    }
public:
    template<class BufferType>
    void update(const BufferType &buffer)
    {
        auto utf16_buffer_size = MultiByteToWideChar(CP_UTF8, 0, boost::asio::buffer_cast<char const*>(buffer), boost::asio::buffer_size(buffer), nullptr, 0);

        _map_size = (int)sqrt(utf16_buffer_size - _board_prefix.size());
        _map.resize(utf16_buffer_size);

        MultiByteToWideChar(CP_UTF8, 0, boost::asio::buffer_cast<char const *>(buffer), boost::asio::buffer_size(buffer), &_map[0], utf16_buffer_size);
    }

    vec2 get_me_coords() const
    {
        int distance = (int)std::distance(_map.begin(), std::find_if(_map.begin(), _map.end(),
                                                                     [](wchar_t map_object)
        {
            return is_me(MapObject(map_object));
        })) - _board_prefix.size();

        return vec2{distance % _map_size, distance / _map_size};
    }

    int size() const
    {
        return _map_size;
    }

    MapObject get(int x, int y) const
    {
        return MapObject(_map[get_address(x, y)]);
    }

    MapObject get(const vec2 &pos) const
    {
        return get(pos.x, pos.y);
    }

    void go_around(const std::function<void(const vec2 &pos, MapObject map_object)> &handler) const
    {
        for (int y = 0; y < size(); ++y)
        {
            for (int x = 0; x < size(); ++x)
            {
                vec2 pos{x, y};
                auto map_object = get(pos);
                handler(pos, map_object);
            }
        }
    }
};
