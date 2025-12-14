#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

struct Tile
{
    Tile(const int xx, const int yy) : x(xx), y(yy) {}
    int x;
    int y;
};

std::vector<Tile> read_tiles(const std::string& filename)
{
    std::vector<Tile> tiles;
    std::ifstream iff(filename, std::ios::in);
    std::string holder;
    while(iff >> holder)
    {
        const auto comm = holder.find(',');
        const int x = std::stoi(holder.substr(0,comm));
        const int y = std::stoi(holder.substr(comm + 1, holder.size()));
        tiles.emplace_back(x,y);
    }

    return tiles;
}

int get_largest_rectangle(const std::string& filename)
{
    std::vector<int> areas{0};
    const auto tiles = read_tiles(filename);
    for(auto it = tiles.begin(); it < tiles.end(); ++it)
    {
        for(auto jt = it + 1; jt < tiles.end(); ++jt)
        {
            const int xdiff = 1 + std::abs(it->x - jt->x);
            const int ydiff = 1 + std::abs(it->y - jt->y);
            //std::cout << "xdiff{" << xdiff << "} ydiff{" << ydiff << "}\n";
            areas.push_back(xdiff * ydiff);
        }
    }

    return *std::max_element(areas.begin(), areas.end());
}

int main()
{
    std::cout << get_largest_rectangle("tiles.txt") << "\n";
    return 0;
}
