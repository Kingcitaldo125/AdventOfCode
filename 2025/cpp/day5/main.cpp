//#include <algorithm>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <vector>

int count_fresh_ingredients(const std::string& filename)
{
    std::vector<std::string> ingredient_ranges;
    std::unordered_set<int> fresh_ingredients;
    int count = 0;

    {
        std::ifstream iff(filename, std::ios::in);
        std::string holder;
        int read_part = 0;
        while(std::getline(iff,holder))
        {
            std::cout << "holder '" << holder << "'\n";
            if (holder == "")
            {
                read_part = 1;
                continue;
            }
            if (read_part == 0)
            {
                ingredient_ranges.push_back(std::move(holder));
            }
            else if (read_part == 1)
            {
                fresh_ingredients.insert(std::stoi(holder));
            }
        }
    }

    const auto get_range_pair = [](const std::string& range)
    {
        const auto splt = range.find('-');
        const int i = std::stoi(range.substr(0,splt));
        const int j = std::stoi(range.substr(splt + 1, range.size()));
        return std::make_pair(i,j);
    };

    std::unordered_set<int> processed;
    for(const auto& range : ingredient_ranges)
    {
        const auto [i,j] = get_range_pair(range);
        for(int itm = i; itm <= j; ++itm)
        {
            if (fresh_ingredients.find(itm) == fresh_ingredients.end())
                continue;
            if (processed.find(itm) != processed.end())
                continue;
            processed.insert(itm);
            ++count;
        }
    }

    return count;
}

int main(int argc, char** argv)
{
    std::cout << count_fresh_ingredients("ingredients.txt") << "\n";
    return 0;
}
