#include <algorithm>
#include <iostream>
#include <string>

std::string convert_diskmap(const std::string& diskmap)
{
    std::string conversion("");
    int ctr = 0;
    bool flip = true;

    for(const auto& itm : diskmap)
    {
        const int times = std::stoi(std::string(1,itm));
        const auto ctr_str = std::to_string(ctr);
        for(int i = 0; i < times; ++i)
        {
            if (flip)
            {
                conversion += ctr_str;
                continue;
            }

            conversion += ".";
        }

        if (flip) ++ctr;
        flip = !flip;
    }

    return conversion;
}

std::string defrag_blocks(std::string diskmap)
{
    while(1)
    {
        auto first_dot = std::find_if(diskmap.begin(), diskmap.end(), [](const char c){ return c == '.'; });
        auto last_alnum = std::find_if(diskmap.rbegin(), diskmap.rend(), [](const char c){ return isdigit(c); });

        const auto first_dot_dist = std::distance(diskmap.begin(), first_dot);
        const auto last_alnum_dist = std::distance(last_alnum, diskmap.rend());

        if (first_dot_dist >= last_alnum_dist) break;

        std::iter_swap(first_dot, last_alnum);
    }

    return diskmap;
}

int calculate_checksum(const std::string& mstr)
{
    int prod_sum = 0;

    for(int i = 0; i < static_cast<int>(mstr.size()); ++i)
    {
        const auto& itm = mstr.at(i);
        if (itm == '.') break;
        prod_sum += i * std::stoi(std::string(1,itm));
    }

    return prod_sum;
}

int main()
{
    const std::string conversion = convert_diskmap("2333133121414131402");
    const std::string defrag = defrag_blocks(conversion);

    const int csum = calculate_checksum(defrag);
    std::cout << csum << '\n';

    return 0;
}
