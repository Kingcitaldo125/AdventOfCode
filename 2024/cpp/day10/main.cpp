#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <string>
#include <vector>

using coord_t = std::pair<int,int>;
using map_t = std::vector<std::string>;
using visited_t = std::unordered_set<std::string>;

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

    for(int i = x - 1; i < x + 2; ++i)
    {
        if (i < 0 || i >= cols || i == x)
            continue;

        coords.push_back(std::make_pair(i,y));
    }

    for(int i = y - 1; i < y + 2; ++i)
    {
        if (i < 0 || i >= rows || i == y)
            continue;

        coords.push_back(std::make_pair(x,i));
    }

    return coords;
}

void search(const map_t& map, visited_t& visited, coord_t coord, int& xcount, const int count)
{
    const auto coord_has_neighbor = [&visited](const coord_t& c)
    {
        return visited.find(coord_to_str(c)) != visited.end();
    };

    const auto cstr = coord_to_str(coord);
    visited.insert(cstr);
    const auto [cx,cy] = coord;

    if (map.at(cy).at(cx) == '9')
    {
        ++xcount;
        return;
    }

    for(auto&& n : get_neighbors(map, coord))
    {
        if (coord_has_neighbor(n)) continue;

        const auto [nx,ny] = n;
        const int int1 = std::stoi(std::string(1, map.at(ny).at(nx)));
        const int int2 = std::stoi(std::string(1, map.at(cy).at(cx)));

        // if the neighbor is not 1 higher than the current elevation
        if (int1 != int2 + 1) continue;

        search(map, visited, n, xcount, count + 1);
    }
}

int path_sum(const map_t& map)
{
    const int rows = static_cast<int>(map.size());
    const int cols = static_cast<int>(map.at(0).size());
    int psum = 0;

    for (int y = 0; y < rows; ++y)
    {
        for (int x = 0; x < cols; ++x)
        {
            const char& c = map.at(y).at(x);
            if (c != '0') continue;

            visited_t visited;
            int xsum = 0;
            search(map, visited, std::make_pair(x,y), xsum, 0);
            psum += xsum;
        }
    }

    return psum;
}

int main()
{
    map_t map
    {
        "89010123",
        "78121874",
        "87430965",
        "96549874",
        "45678903",
        "32019012",
        "01329801",
        "10456732",
    };

    std::cout << path_sum(map) << "\n";

    return 0;
}
