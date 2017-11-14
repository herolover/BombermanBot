#pragma once

#include <string>
#include <vector>

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

	Space = L' '
};

class Map
{
private:
    std::size_t _map_size;
    std::vector<std::underlying_type_t<MapObject>> _map;

    const std::string _board_prefix = "board=";

    std::size_t get_address(int x, int y)
    {
        return y * _map_size + x + _board_prefix.size();
    }
public:
    template<class BufferType>
    void update(const BufferType &buffer)
    {
        auto utf16_buffer_size = MultiByteToWideChar(CP_UTF8, 0, boost::asio::buffer_cast<char const*>(buffer), boost::asio::buffer_size(buffer), nullptr, 0);
        _map.resize(utf16_buffer_size);

        MultiByteToWideChar(CP_UTF8, 0, boost::asio::buffer_cast<char const *>(buffer), boost::asio::buffer_size(buffer), &_map[0], utf16_buffer_size);
    }

    std::size_t map_size() const
    {
        return _map_size;
    }

    MapObject get(int x, int y)
    {
        return MapObject(_map[get_address(x, y)]);
    }

    void set(int x, int y, MapObject object)
    {
        _map[get_address(x, y)] = static_cast<std::underlying_type_t<MapObject>>(object);
    }
};