#include <algorithm>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>

using map_t = std::vector<std::vector<std::string>>;

static std::unordered_map<char,int> direction_map
{
    {'<', -1},
    {'>', 1},
    {'^', -1},
    {'v', 1},
};

struct Robot
{
    int x;
    int y;
    char dirchar;
    std::vector<char> directions;
};

inline void print_map(map_t& map)
{
    for(auto& row : map)
    {
        for(auto& cell : row)
        {
            //std::cout << '\'' << cell << '\'';
            std::cout << cell;
        }
        std::cout << "\n";
    }
}

void read_input(map_t& map, Robot& robot, const std::string& filepath)
{
    std::ifstream iff(filepath, std::ios::in);
    if (!iff.is_open())
    {
        std::cout << "Cannot open '" << filepath << "' for reading.\n";
        return;
    }

    std::string holder("");
    int xctr = 0;
    int yctr = 0;
    while(std::getline(iff, holder))
    {
        if (holder.find("#") != std::string::npos)
        {
            std::vector<std::string> row;
            xctr = 0;
            for(const auto& h : holder)
            {
                if (h == '@')
                {
                    robot.x = xctr;
                    robot.y = yctr;
                }

                row.push_back(std::string(1, h));
                ++xctr;
            }
            map.push_back(std::move(row));
            ++yctr;
        }
        else
        {
            bool direction_line = false;
            for(const auto& dir_seq : direction_map)
            {
                const auto [chr, _] = dir_seq;
                if (holder.find(chr) != std::string::npos)
                {
                    direction_line = true;
                    break;
                }
            }

            if (!direction_line) continue;

            //std::cout << "holder: " << holder << "\n";
            for(const char& chr : holder)
            {
                robot.directions.push_back(chr);
                //std::cout << "pushed: " << chr << "\n";
            }
        }
    }
}

int handle_row(map_t& map, const char dir, const int row_idx, const int idx)
{
    if (idx < 1 || idx >= map.at(0).size() - 1) return 1;

    auto& cur = map.at(row_idx).at(idx);
    auto& next = map.at(row_idx).at(idx + direction_map[dir]);

    /*
    std::cout << "row cur: " << cur << "\n";
    std::cout << "row next: " << next << "\n";
    std::cout << "\n";
    */

    if (next == "#") return 1;

    int retv = 0;
    if (cur == "@")
    {
        if (next == "O")
        {
            retv = handle_row(map, dir, row_idx, idx + direction_map[dir]);
        }
    }
    else if (cur == "O")
    {
        if (next == "O")
        {
            retv = handle_row(map, dir, row_idx, idx + direction_map[dir]);
        }
        else if (next == ".")
        {
            std::swap(cur, next);
            return 0;
        }
    }

    // swap the robot and the next char
    if (retv == 0)
        std::swap(cur, next);

    return retv;
}

int handle_column(map_t& map, const char dir, const int col_idx, const int idx)
{
    if (idx < 1 || idx >= map.size() - 1) return 1;

    auto& cur = map.at(idx).at(col_idx);
    auto& next = map.at(idx + direction_map[dir]).at(col_idx);

    /*
    std::cout << "row cur: " << cur << "\n";
    std::cout << "row next: " << next << "\n";
    std::cout << "\n";
    */

    if (next == "#") return 1;

    int retv = 0;
    if (cur == "@")
    {
        if (next == "O")
        {
            retv = handle_column(map, dir, col_idx, idx + direction_map[dir]);
        }
    }
    else if (cur == "O")
    {
        if (next == "O")
        {
            retv = handle_column(map, dir, col_idx, idx + direction_map[dir]);
        }
        else if (next == ".")
        {
            std::swap(cur, next);
            return 0;
        }
    }

    // swap the robot and the next char
    if (retv == 0)
        std::swap(cur, next);

    return retv;
}

void handle_robot(map_t& map, Robot& robot)
{
    //std::cout << "robot at " << robot.x << ":" << robot.y << "\n";

    if (robot.dirchar == '<' || robot.dirchar == '>')
    {
        handle_row(map, robot.dirchar, robot.y, robot.x);
    }
    else if (robot.dirchar == '^' || robot.dirchar == 'v')
    {
        handle_column(map, robot.dirchar, robot.x, robot.y);
    }

    for(int y = 0; y < static_cast<int>(map.size()); ++y)
    {
        bool found = false;
        for(int x = 0; x < static_cast<int>(map.at(0).size()); ++x)
        {
            if (map.at(y).at(x) == "@")
            {
                found = true;
                robot.x = x;
                robot.y = y;
                break;
            }
        }
        if (found) break;
    }
    /*
    */
}

constexpr int calculate_distance(const int box_x, const int box_y)
{
    constexpr int multiplier = 100;
    return multiplier * box_y + box_x;
}

int run(map_t& map, Robot& robot)
{
    if (robot.directions.size() == 0)
    {
        std::cout << "No robot directions to process" << std::endl;
        return 0;
    }

    print_map(map);
    std::cout << std::endl;

    for(const auto& dir : robot.directions)
    {
        //std::cout << "robot pos: " << robot.x << "," << robot.y << "\n";
        // Update the current robot direction
        robot.dirchar = dir;
        std::cout << robot.dirchar << "\n";

        handle_robot(map, robot);
        print_map(map);
        std::cout << std::endl;
    }

    std::vector<std::pair<int,int>> box_coords;
    for(int y = 0; y < static_cast<int>(map.size()); ++y)
    {
        for(int x = 0; x < static_cast<int>(map.at(0).size()); ++x)
        {
            if (map.at(y).at(x) != "O") continue;
            box_coords.push_back(std::make_pair(x,y));
        }
    }

    int box_coord_sum = 0;
    for(const auto& box : box_coords)
    {
        const auto [bx,by] = box;
        box_coord_sum += calculate_distance(bx,by);
    }

    return box_coord_sum;
}

int main()
{
    map_t map;
    Robot robot;

    read_input(map, robot, "./input.txt");

    std::cout << run(map, robot) << "\n";

    return 0;
}
