#include <cmath>

#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <string>
#include <vector>

using coord_t = std::pair<int,int>;
using map_t = std::vector<std::string>;
using visited_t = std::unordered_set<std::string>;

constexpr int magnitude(const int p1, const int p2)
{
    return static_cast<int>(sqrt(static_cast<double>(p1*p1 + p2*p2)));
}

inline std::string coord_to_str(const coord_t& coord)
{
    const auto [x,y] = coord;
    return std::to_string(x) + "," + std::to_string(y);
};

std::vector<coord_t> get_neighbors(const map_t& map, const coord_t& coord)
{
    std::vector<coord_t> coords;
    const int rows = static_cast<int>(map.size());
    const int cols = static_cast<int>(map.at(0).size());
    const auto [x,y] = coord;

    for(int j = y - 1; j < y + 2; ++j)
    {
        for(int i = x - 1; i < x + 2; ++i)
        {
            if (j < 0 || j >= rows)
                continue;
            if (i < 0 || i >= cols)
                continue;
            if (i == x && j == y)
                continue;

            coords.push_back(std::make_pair(i,j));
        }
    }

    return coords;
}

inline bool pairs_equal(const coord_t& c1, const coord_t& c2)
{
    const auto [c1x,c1y] = c1;
    const auto [c2x,c2y] = c2;
    return c1x == c2x && c1y == c2y;
}

inline coord_t get_neighbor_direction(const coord_t& location, const coord_t& n)
{
    const auto [x,y] = location;
    const auto [nx,ny] = n;

    const int diffx = nx-x;
    const int diffy = ny-y;

    const int mag = magnitude(diffx, diffy);

    return std::make_pair(diffx/mag,diffy/mag);
}

void search(const map_t& map, visited_t& visited, const std::string& xmas_str, int& count, coord_t coord, coord_t direction, const std::size_t cur_char_idx)
{
    const auto coord_has_neighbor = [&visited](const coord_t& c)
    {
        return visited.find(coord_to_str(c)) != visited.end();
    };

    const auto cstr = coord_to_str(coord);
    visited.insert(cstr);

    //std::cout << "current char: " << xmas_str.at(cur_char_idx) << "\n";
    //std::cout << "at idx: " << cstr << "\n";

    if (cur_char_idx >= xmas_str.size() - 1)
    {
        //std::cout << "found S at '" << cstr << "'\n";
        ++count;
        return;
    }

    for(auto&& n : get_neighbors(map, coord))
    {
        if (coord_has_neighbor(n)) continue;

        const char next_char = xmas_str.at(cur_char_idx + 1);
        //std::cout << "next char: " << next_char << "\n";
        const auto [nx,ny] = n;

        if (map.at(ny).at(nx) != next_char) continue;
        if (!pairs_equal(direction, get_neighbor_direction(coord, n))) continue;

        search(map, visited, xmas_str, count, n, direction, cur_char_idx + 1);
    }
    //std::cout << "\n";
}

int xmas_count(const map_t& map)
{
    const std::string xmas_str("XMAS");
    const int rows = static_cast<int>(map.size());
    const int cols = static_cast<int>(map.at(0).size());
    int tcount = 0;

    for (int y = 0; y < rows; ++y)
    {
        for (int x = 0; x < cols; ++x)
        {
            const char& c = map.at(y).at(x);
            if (c != 'X') continue;
            const coord_t coord = std::make_pair(x,y);

            visited_t visited;
            int count = 0;

            for(auto&& n : get_neighbors(map, coord))
            {
                const auto [nx,ny] = n;
                if (map.at(ny).at(nx) != 'M') continue;
                const auto direction = get_neighbor_direction(coord, n);
                //std::cout << "start direction: " << coord_to_str(direction) << "\n";

                search(map, visited, xmas_str, count, n, direction, 1);
            }

            tcount += count;
        }
    }

    return tcount;
}

int main()
{
    map_t map
    {
        "....XXMAS.",
        ".SAMXMS...",
        "...S..A...",
        "..A.A.MS.X",
        "XMASAMX.MM",
        "X.....XA.A",
        "S.S.S.S.SS",
        ".A.A.A.A.A",
        "..M.M.M.MM",
        ".X.X.XMASX",
    };

    std::cout << xmas_count(map) << "\n";

    return 0;
}
