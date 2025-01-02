#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <vector>

using coord_t = std::pair<int,int>;
using map_t = std::vector<std::vector<char>>;
using visited_t = std::unordered_set<std::string>;

constexpr int n_fence_posts(const int rows, const int cols)
{
    return (4 * rows + 4 * cols)/ 2;
}

inline std::string coord_to_str(const coord_t& c)
{
    const auto [x,y] = c;
    return std::to_string(x) + "," + std::to_string(y);
};

void search(const map_t& map, visited_t& visited, const coord_t& start_coord, coord_t coord, int& rows, int& cols)
{
    const auto get_neighbors = [&map](const coord_t& c)
    {
        std::vector<coord_t> coords;
        const auto [x,y] = c;
        for(int i = x - 1; i < x + 2; ++i)
        {
            if (i < 0 || i >= static_cast<int>(map.at(0).size()) || i == x)
                continue;
            if (map.at(y).at(x) != map.at(y).at(i))
                continue;
            coords.push_back(std::make_pair(i,y));
        }
        for(int i = y - 1; i < y + 2; ++i)
        {
            if (i < 0 || i >= static_cast<int>(map.size()) || i == y)
                continue;
            if (map.at(y).at(x) != map.at(i).at(x))
                continue;
            coords.push_back(std::make_pair(x,i));
        }
        return coords;
    };

    const auto coord_has_neighbor = [&visited](const coord_t& c)
    {
        return visited.find(coord_to_str(c)) != visited.end();
    };

    visited.insert(coord_to_str(coord));
    const auto [cx,cy] = start_coord;

    for(auto&& n : get_neighbors(coord))
    {
        if (coord_has_neighbor(n)) continue;

        const auto [nx,ny] = n;

        //std::cout << "cx: " << cx << " cy: " << cy << "\n";
        //std::cout << "nx: " << nx << " ny: " << ny << "\n";

        rows = std::max(rows, std::abs(ny-cy));
        cols = std::max(cols, std::abs(nx-cx));

        search(map, visited, start_coord, n, rows, cols);
    }
}

int calculate_asdf(const map_t& map)
{
    visited_t g_visited;
    int res = 0;
    char last_char = '_';

    for (int y = 0; y < static_cast<int>(map.size()); ++y)
    {
        for (int x = 0; x < static_cast<int>(map.at(0).size()); ++x)
        {
            const coord_t coord = std::make_pair(x,y);

            if (g_visited.find(coord_to_str(coord)) != g_visited.end())
            {
                continue;
            }

            const char& c = map.at(y).at(x);
            if (c == last_char) continue;
            //std::cout << "char: " << c << "\n";

            visited_t visited;
            int rows = 0;
            int cols = 0;

            search(map, visited, coord, coord, rows, cols);
            ++rows; // account for off-by-1
            ++cols; // account for off-by-1

            const int vlen = static_cast<int>(visited.size());

            for(const auto& v : visited)
            {
                g_visited.insert(v);
            }

            /*
            std::cout << "vlen: " << vlen << "\n";
            std::cout << "rows: " << rows << "\n";
            std::cout << "cols: " << cols << "\n";
            std::cout << "\n";
            */

            res += n_fence_posts(rows,cols) * vlen;
            last_char = c;
        }
    }

    return res;
}

int main()
{
    map_t map
    {
        {'A','A','A','A'},
        {'B','B','C','D'},
        {'B','B','C','C'},
        {'E','E','E','C'},
    }; // example 1

    std::cout << calculate_asdf(map) << "\n";

    return 0;
}
