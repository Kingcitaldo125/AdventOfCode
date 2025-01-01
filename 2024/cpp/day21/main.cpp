#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>

using coord_t = std::pair<int,int>;
using map_t = std::unordered_map<char,coord_t>;

static map_t numberpad
{
    {'7', std::make_pair(-2,3)},
    {'8', std::make_pair(-1,3)},
    {'9', std::make_pair(0,3)},
    {'4', std::make_pair(-2,2)},
    {'5', std::make_pair(-1,2)},
    {'6', std::make_pair(0,2)},
    {'1', std::make_pair(-2,1)},
    {'2', std::make_pair(-1,1)},
    {'3', std::make_pair(0,1)},
    {' ', std::make_pair(-2,0)},
    {'0', std::make_pair(-1,0)},
    {'A', std::make_pair(0,0)},
};

static map_t directionalpad
{
    {'A', std::make_pair(0,0)},
    {'^', std::make_pair(-1,0)},
    {' ', std::make_pair(-2,0)},
    {'>', std::make_pair(0,-1)},
    {'v', std::make_pair(-1,-1)},
    {'<', std::make_pair(-2,-1)},
};

inline std::pair<int,int> mod_manhattan(const coord_t& first, const coord_t& second)
{
    const auto [x1,y1] = first;
    const auto [x2,y2] = second;
    return std::make_pair(x1-x2,y1-y2);
}

std::string mag_to_dir(const int mag, const bool x_dir)
{
    std::string output("");

    for(int i = 0; i < std::abs(mag); ++i)
    {
        if (x_dir)
        {
            output += mag < 0 ? '<' : '>';
        }
        else
        {
            output += mag < 0 ? 'v' : '^';
        }
    }

    return output;
}

std::string generate_sequence(const std::string& command)
{
    std::string seq("");
    auto [num_x,num_y] = numberpad['A'];
    auto [dir_x,dir_y] = directionalpad['A'];

    const auto generate_seq_splice = [](map_t& map, const char key, const coord_t& coord)
    {
        std::string outp("");

        const auto [dx, dy] = mod_manhattan(map[key], coord);
        const auto dx_str = mag_to_dir(dx, true);
        const auto dy_str = mag_to_dir(dy, false);

        std::cout << "dx_str: " << dx_str << "\n";
        std::cout << "dy_str: " << dy_str << "\n";

        if (std::abs(dx) > std::abs(dy))
        {
            if (dy_str.size() > 0) outp += dy_str;
            if (dx_str.size() > 0) outp += dx_str;
        }
        else
        {
            if (dx_str.size() > 0) outp += dx_str;
            if (dy_str.size() > 0) outp += dy_str;
        }
        outp += 'A';

        return std::make_tuple(outp, dx, dy);
    };

    for(auto it = command.begin(); it < command.end() - 1; ++it)
    {
        const auto [outp1,nx1,ny1] = generate_seq_splice(numberpad, *it, std::make_pair(num_x, num_y));
        num_x += nx1;
        num_y += ny1;

        std::cout << "outp1: " << outp1 << "\n";

        for(const auto& p : outp1)
        {
            std::cout << "p: " << p << "\n";

            auto [outp2,nx2,ny2] = generate_seq_splice(directionalpad, p, std::make_pair(dir_x, dir_y));

            std::cout << "dir_x: " << dir_x << "\n";
            std::cout << "dir_y: " << dir_y << "\n";

            dir_x += nx2;
            dir_y += ny2;

            std::cout << "outp2: " << outp2 << "\n";
            seq += std::move(outp2);
        }
        std::cout << "\n";
    }

    return seq;
}

int calculate_total(const std::vector<std::string>& commands)
{
    int total = 0;

    for(const auto& command : commands)
    {
        const auto seq = generate_sequence(command);

        std::cout << "command: " << command << "\n";
        std::cout << "seq: " << seq << "\n";

        const int seq_len = static_cast<int>(seq.size());
        const int numeric_portion = std::stoi(command.substr(0,command.size() - 1));

        std::cout << "seq_len: " << seq_len << "\n";
        std::cout << "numeric_portion: " << numeric_portion << "\n";
        std::cout << "\n";

        total += seq_len * numeric_portion;
    }

    return total;
}

int main()
{
    std::cout << calculate_total({"029A"}) << "\n";
    return 0;
}
