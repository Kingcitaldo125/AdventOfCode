#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

#define ROLL '@'

using grid_t = std::vector<std::string>;

inline void print_grid(const grid_t& grid)
{
    for(const auto& row : grid)
    {
        std::cout << row << "\n";
    }
}

grid_t generate_grid(const std::string& filename)
{
    grid_t grid;
    std::ifstream iff(filename, std::ios::in);
    std::string holder;

    while(iff >> holder)
    {
        grid.push_back(std::move(holder));
    }

    return grid;
}

int get_adjacency_count(grid_t& grid, const int x, const int y)
{
    int adj_count = 0;

    for(int i = x - 1; i < x + 2; ++i)
    {
        for(int j = y - 1; j < y + 2; ++j)
        {
            if (i < 0 || i >= static_cast<int>(grid.front().size()))
                continue;
            if (j < 0 || j >= static_cast<int>(grid.size()))
                continue;
            if (i == x && j == y)
                continue;

            if (grid[j][i] == ROLL)
                ++adj_count;
        }
    }

    return adj_count;
}

int count_accessible_rolls(const std::string& filename)
{
    auto grid = generate_grid(filename);
    int count = 0;
    for(int y = 0; y < static_cast<int>(grid.size()); ++y)
    {
        for(int x = 0; x < static_cast<int>(grid.front().size()); ++x)
        {
            if (grid[y][x] != ROLL)
                continue;

            const auto adj_cnt = get_adjacency_count(grid,x,y);
            if (adj_cnt >= 4) // too many adjacent rolls
                continue;

            ++count;
        }
    }

    return count;
}

int main(int argc, char** argv)
{
    std::cout << count_accessible_rolls("rolls.txt") << "\n";
    return 0;
}
