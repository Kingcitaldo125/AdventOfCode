#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <unordered_set>
#include <vector>

using map_t = std::vector<std::vector<std::string>>;
using visited_t = std::unordered_set<std::string>;
using costs_t = std::unordered_set<int>;
using target_t = std::pair<int,int>;

struct Reindeer
{
    int x;
    int y;
    char dirchar;
};

struct Neighbor
{
    int x;
    int y;
    int cost;
    char dirchar;
};

inline std::string coord_to_string(const int x, const int y)
{
    return std::to_string(x) + "," + std::to_string(y);
}

inline void print_map(map_t& map)
{
    for(const auto& row : map)
    {
        for(const auto& r : row)
        {
            std::cout << r;
        }
        std::cout << '\n';
    }
}

bool read_input(map_t& map, Reindeer& reindeer, target_t& target, const std::string& filepath)
{
    std::ifstream iff(filepath, std::ios::in);
    if (!iff.is_open())
    {
        std::cout << "Cannot open '" << filepath << "' for reading.\n";
        return false;
    }

    std::string holder("");
    int xctr = 0;
    int yctr = 0;
    while(std::getline(iff, holder))
    {
        std::vector<std::string> row;
        xctr = 0;

        for(auto it = holder.begin(); it != holder.end(); ++it)
        {
            if (*it == 'S')
            {
                reindeer.x = xctr;
                reindeer.y = yctr;
            }
            else if (*it == 'E')
            {
                target = std::make_pair(xctr, yctr);
            }

            row.push_back(std::string(1,*it));
            ++xctr;
        }

        map.push_back(std::move(row));
        ++yctr;
    }

    reindeer.dirchar = '>';
    return true;
}

std::vector<Neighbor> get_neighbors(map_t& map, const Reindeer& reindeer)
{
    std::vector<Neighbor> neighbors;
    const int nrows = static_cast<int>(map.size());
    const int ncols = static_cast<int>(map.at(0).size());
    const int x = reindeer.x;
    const int y = reindeer.y;
    const char dirchar = reindeer.dirchar;

    const auto nsort = [](const Neighbor& neigh1, const Neighbor& neigh2)
    {
        return neigh1.cost < neigh2.cost;
    };

    for(int i = x - 1; i < x + 2; ++i)
    {
        if (i < 0 || i >= ncols || i == x) continue;
        if (map.at(y).at(i) == "#") continue;

        int lcost = 1;

        if (i < x)
        {
            lcost = dirchar != '<' ? 1001 : 1;
        }
        else if (i > x)
        {
            lcost = dirchar != '>' ? 1001 : 1;
        }

        if (dirchar == '<' && i > x || dirchar == '>' && i < x)
        {
            lcost = 2001;
        }

        neighbors.push_back(Neighbor{i,y,lcost,i < x ? '<' : '>'});
    }

    for(int i = y - 1; i < y + 2; ++i)
    {
        if (i < 0 || i >= nrows || i == y) continue;
        if (map.at(i).at(x) == "#") continue;

        int lcost = 1;

        if (i < y)
        {
            lcost = dirchar != '^' ? 1001 : 1;
        }
        else if (i > y)
        {
            lcost = dirchar != 'v' ? 1001 : 1;
        }

        if (dirchar == '^' && i > y || dirchar == 'v' && i < y)
        {
            lcost = 2001;
        }

        neighbors.push_back(Neighbor{x,i,lcost,i < y ? '^' : 'v'});
    }

    std::sort(neighbors.begin(), neighbors.end(), nsort);

    return neighbors;
}

int path_cost(map_t& map, visited_t& visited, costs_t& costs, Reindeer reindeer, const target_t& target, int cost)
{
    const auto reindeer_str = coord_to_string(reindeer.x, reindeer.y);
    //std::cout << "Reindeer at: " << reindeer_str << " facing " << reindeer.dirchar << "\n";
    //std::cout << "Current cost: " << cost << "\n";

    const auto [tx,ty] = target;

    if (reindeer.x == tx && reindeer.y == ty)
    {
        //std::cout << "found target at: " << reindeer_str << "\n";
        //std::cout << "path cost: " << cost << "\n";
        costs.insert(cost);
        return 1;
    }

    visited.insert(reindeer_str);

    if (cost != 0)
        map.at(reindeer.y).at(reindeer.x) = reindeer.dirchar;

    int lfound = 0;
    for(auto& neighbor : get_neighbors(map, reindeer))
    {
        const auto nstr = coord_to_string(neighbor.x, neighbor.y);
        if (visited.find(nstr) != visited.end()) continue;

        lfound |= path_cost(
            map,
            visited,
            costs,
            Reindeer{neighbor.x, neighbor.y, neighbor.dirchar},
            target,
            cost + neighbor.cost
        );
    }

    if (lfound == 1)
    {
        visited.erase(reindeer_str);
    }
    else
    {
        map.at(reindeer.y).at(reindeer.x) = '.';
    }

    return lfound;
}

int run(map_t& map, Reindeer reindeer, const target_t& target)
{
    visited_t visited;
    costs_t costs;
    path_cost(map, visited, costs, std::move(reindeer), target, 0);

    if (costs.size() == 0) return 0;

    int min_cost = 999999999;
    for(const auto& cost : costs)
    {
        min_cost = std::min(min_cost, cost);
    }

    return min_cost;
}

int main()
{
    map_t map;
    Reindeer reindeer;
    target_t target;

    if(!read_input(map, reindeer, target, "./input.txt"))
    {
        return 1;
    }

    const auto mcost = run(map, reindeer, target);

    print_map(map);

    std::cout << "min cost: " << mcost << "\n";

    return 0;
}
