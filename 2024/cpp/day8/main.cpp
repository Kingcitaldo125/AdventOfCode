#include <iostream>
#include <unordered_set>
#include <vector>
#include <utility>

using map_t = std::vector<std::string>;
using visited_t = std::unordered_set<std::string>;
using coord_t = std::pair<int,int>;

inline std::string coord_to_str(const coord_t& coord)
{
    const auto [x,y] = coord;
    return std::to_string(x) + "," + std::to_string(y);
}

inline void print_map(const map_t& map, const visited_t& visited)
{
    int yctr = 0;
    int xctr = 0;

    for(const auto& row : map)
    {
        xctr = 0;
        for(const auto& r : row)
        {
            if (visited.find(coord_to_str(std::make_pair(xctr,yctr))) != visited.end())
            {
                if (isalnum(r))
                {
                    ++xctr;
                    continue;
                }

                std::cout << "#";
                ++xctr;
                continue;
            }

            std::cout << r;
            ++xctr;
        }

        std::cout << "\n";
        ++yctr;
    }
}

std::vector<coord_t> get_antenna_distances(const coord_t& antenna_a, const coord_t& antenna_b)
{
    const auto [x1,y1] = antenna_a;
    const auto [x2,y2] = antenna_b;

    const int xdiff1 = x2 - x1;
    const int ydiff1 = y2 - y1;

    const int xdiff2 = x1 - x2;
    const int ydiff2 = y1 - y2;

    coord_t first = std::make_pair(x2 + xdiff1, y2 + ydiff1);
    coord_t second = std::make_pair(x1 + xdiff2, y1 + ydiff2);

    return std::vector<coord_t>{first,second};
}

void calculate_antinodes(const std::vector<coord_t>& antennas, map_t& map, visited_t& visited)
{
    const auto in_bounds = [&map](const coord_t& coord)
    {
        const auto [x,y] = coord;
        const bool xbounds = x >= 0 && x < static_cast<int>(map.at(0).size());
        const bool ybounds = y >= 0 && y < static_cast<int>(map.size());
        return xbounds && ybounds;
    };

    for(auto it = antennas.begin(); it != antennas.end(); ++it)
    {
        for(auto jt = antennas.begin(); jt != antennas.end(); ++jt)
        {
            if (it == jt) continue;

            const auto [x1,y1] = *it;
            const auto [x2,y2] = *jt;

            if (map.at(y1).at(x1) != map.at(y2).at(x2)) continue;

            for (const auto& dist : get_antenna_distances(*it,*jt))
            {
                if (in_bounds(dist))
                {
                    //std::cout << "distance from '" << coord_to_str(*it) << "' to '" << coord_to_str(*jt) << "' = '" << coord_to_str(dist) << "'\n";
                    visited.insert(coord_to_str(dist));
                }
            }
        }
    }
}

std::vector<coord_t> calculate_antenna_positions(map_t& map)
{
    std::vector<coord_t> antennas;
    int yctr = 0;
    int xctr = 0;

    for(const auto& row : map)
    {
        xctr = 0;
        for(const auto& r : row)
        {
            if (!isalnum(r))
            {
                ++xctr;
                continue;
            }

            antennas.push_back(std::make_pair(xctr, yctr));
            ++xctr;
        }
        ++yctr;
    }

    return antennas;
}

inline int populate_antinodes(map_t& map, visited_t& visited)
{
    std::vector<coord_t> antennas = calculate_antenna_positions(map);

    calculate_antinodes(antennas, map, visited);

    return static_cast<int>(visited.size());
}

int main()
{
    // 11 columns
    map_t map
    {
        "............", // 0
        "........0...", // 1
        ".....0......", // 2
        ".......0....", // 3
        "....0.......", // 4
        "......A.....", // 5
        "............", // 6
        "............", // 7
        "........A...", // 8
        ".........A..", // 9
        "............", // 10
        "............", // 11
    };
    visited_t visited;

    std::cout << populate_antinodes(map, visited) << "\n";
    print_map(map, visited);

    return 0;
}
