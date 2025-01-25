#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <vector>
#include <utility>

using map_t = std::vector<std::string>;
using visited_t = std::unordered_set<std::string>;
using coord_t = std::pair<int,int>;

struct Guard
{
    coord_t position;
    coord_t direction;
};

inline std::string coord_to_str(const coord_t& coord)
{
    const auto [x,y] = coord;
    return std::to_string(x) + "," + std::to_string(y);
}

inline void turn(Guard& guard)
{
    auto &[xdir,ydir] = guard.direction;

    if (xdir != 0)
    {
        ydir = xdir == -1 ? -1 : 1;
        xdir = 0;
    }
    else if (ydir != 0)
    {
        xdir = ydir == -1 ? 1 : -1;
        ydir = 0;
    }
}

coord_t find_guard_pos(map_t& map)
{
    int xctr = 0;
    int yctr = 0;
    for(const auto& row : map)
    {
        auto it = std::find(row.begin(), row.end(), '^');

        if (it == row.end())
        {
            ++yctr;
            continue;
        }

        xctr = static_cast<int>(std::distance(row.begin(), it));
        return std::make_pair(xctr, yctr);
    }

    return std::make_pair(-1,-1);
}

[[nodiscard]] int process_row(map_t& map, visited_t& visited, Guard& guard)
{
    auto &[x,y] = guard.position;
    auto &[xdir,__] = guard.direction;

    if (xdir == -1)
    {
        auto& row = map.at(y);
        int i = x;
        for(; i > 0; --i)
        {
            auto& itm = row.at(i - 1);
            if (itm == '#')
            {
                x = i;
                return 0;
            }
            visited.insert(coord_to_str(std::make_pair(i,y)));
        }
        visited.insert(coord_to_str(std::make_pair(i, y)));
    }
    else if (xdir == 1)
    {
        auto& row = map.at(y);
        int i = x;
        for(; i < static_cast<int>(row.size()) - 1; ++i)
        {
            auto& itm = row.at(i + 1);
            if (itm == '#')
            {
                x = i;
                return 0;
            }
            visited.insert(coord_to_str(std::make_pair(i,y)));
        }
        visited.insert(coord_to_str(std::make_pair(i, y)));
    }

    return 1;
}

[[nodiscard]] int process_col(map_t& map, visited_t& visited, Guard& guard)
{
    auto &[x,y] = guard.position;
    auto &[__,ydir] = guard.direction;

    if (ydir == -1)
    {
        int i = y;
        for(; i > 0; --i)
        {
            auto& itm = map.at(i - 1).at(x);
            if (itm == '#')
            {
                y = i;
                return 0;
            }
            visited.insert(coord_to_str(std::make_pair(x,i)));
        }
        visited.insert(coord_to_str(std::make_pair(x, i)));
    }
    else if (ydir == 1)
    {
        int i = y;
        for(; i < static_cast<int>(map.size()) - 1; ++i)
        {
            auto& itm = map.at(i + 1).at(x);
            if (itm == '#')
            {
                y = i;
                return 0;
            }
            visited.insert(coord_to_str(std::make_pair(x,i)));
        }
        visited.insert(coord_to_str(std::make_pair(x, i)));
    }

    return 1;
}

std::size_t runapp(map_t& map)
{
    Guard guard{find_guard_pos(map), std::make_pair(0,-1)};
    visited_t visited;

    if (std::get<0>(guard.position) == -1 || std::get<1>(guard.position) == -1)
    {
        return -1;
    }

    while (1)
    {
        auto &[xdir,ydir] = guard.direction;

        if (xdir == -1 || xdir == 1)
        {
            const auto res = process_row(map, visited, guard);
            if (res == 1) break;
            turn(guard);
        }
        else if (ydir == -1 || ydir == 1)
        {
            const auto res = process_col(map, visited, guard);
            if (res == 1) break;
            turn(guard);
        }
    }

    const auto str_to_coord = [](const std::string& mstr)
    {
        const int x = std::stoi(mstr.substr(0,mstr.find(',')));
        const int y = std::stoi(mstr.substr(mstr.find(',') + 1, mstr.size()));
        return std::make_pair(x,y);
    };

    for(const auto& coord : visited)
    {
        const auto [x,y] = str_to_coord(coord);
        map.at(y).at(x) = 'X';
    }

    return visited.size();
}

inline void print_map(map_t& map)
{
    for(const auto& row : map)
    {
        std::cout << row << "\n";
    }
}

int main()
{
    map_t map
    {
        "....#.....",
        ".........#",
        "..........",
        "..#.......",
        ".......#..",
        "..........",
        ".#..^.....",
        "........#.",
        "#.........",
        "......#...",
    };

    std::cout << runapp(map) << "\n";
    print_map(map);

    return 0;
}
