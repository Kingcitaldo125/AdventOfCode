#include <iostream>
#include <fstream>
#include <vector>

//#define TILE_WIDTH 11
//#define TILE_HEIGHT 7

#define TILE_WIDTH 101
#define TILE_HEIGHT 103
#define SIM_SECONDS 100

using map_t = std::vector<std::vector<std::string>>;

struct Robot
{
    int x;
    int y;
    int velx;
    int vely;
};

inline void make_map(map_t& map)
{
    for(int j = 0; j < TILE_HEIGHT; ++j)
    {
        std::vector<std::string> row;
        for(int i = 0; i < TILE_WIDTH; ++i)
        {
            row.push_back(".");
        }
        map.push_back(std::move(row));
    }
}

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

void read_input(std::vector<Robot>& robots, const std::string& filepath)
{
    std::ifstream iff(filepath, std::ios::in);
    if (!iff.is_open())
    {
        std::cout << "Cannot open '" << filepath << "' for reading.\n";
        return;
    }
    // p=0,4 v=3,-3
    std::string holder("");
    while(std::getline(iff,holder))
    {
        //std::cout << "holder: " << holder << "\n";

        auto first_equals = holder.find("=");
        auto first_comma = holder.find(",");

        const auto x = std::stoi(holder.substr(first_equals + 1, first_comma));
        const auto y = std::stoi(holder.substr(first_comma + 1, holder.find(" ")));

        //std::cout << "x: " << x << "\n";
        //std::cout << "y: " << y << "\n";

        const auto vel = holder.find("v");
        auto second_equals = holder.find("=", vel);
        auto second_comma = holder.find(",", vel);

        const auto velx = std::stoi(holder.substr(second_equals + 1, second_comma));
        const auto vely = std::stoi(holder.substr(second_comma + 1, holder.size()));

        //std::cout << "velx: " << velx << "\n";
        //std::cout << "vely: " << vely << "\n";

        robots.push_back(Robot{x,y,velx,vely});
    }
}

void handle_robot(map_t& map, Robot& robot)
{
    if (robot.x + robot.velx < 0)
    {
        robot.x = TILE_WIDTH + robot.x + robot.velx;
    }
    else if (robot.x + robot.velx >= TILE_WIDTH)
    {
        robot.x = robot.x + robot.velx - TILE_WIDTH;
    }
    else
    {
        robot.x = robot.x + robot.velx;
    }

    if (robot.y + robot.vely < 0)
    {
        robot.y = TILE_HEIGHT + robot.y + robot.vely;
    }
    else if (robot.y + robot.vely >= TILE_HEIGHT)
    {
        robot.y = robot.y + robot.vely - TILE_HEIGHT;
    }
    else
    {
        robot.y = robot.y + robot.vely;
    }
}

int calculate_quadrant(map_t& map, const int quad_id)
{
    if (quad_id < 1 || quad_id > 4)
    {
        std::cout << "Quadrant id must be >0 and <5\n";
        return 0;
    }

    constexpr int map_rows = TILE_HEIGHT;
    constexpr int map_cols = TILE_WIDTH;
    constexpr int half_rows = map_rows / 2;
    constexpr int half_cols = map_cols / 2;

    int startx = 0;
    int starty = 0;
    int endx = 0;
    int endy = 0;

    if (quad_id == 1)
    {
        endx = half_cols;
        endy = half_rows;
    }
    else if (quad_id == 2)
    {
        startx = half_cols + 1;
        endx = map_cols;
        endy = half_rows;
    }
    else if (quad_id == 3)
    {
        starty = half_rows + 1;
        endx = half_cols;
        endy = map_rows;
    }
    else if (quad_id == 4)
    {
        startx = half_cols + 1;
        starty = half_rows + 1;
        endx = map_cols;
        endy = map_rows;
    }

    int quad_score = 0;

    for(int y = starty; y < endy; ++y)
    {
        for(int x = startx; x < endx; ++x)
        {
            const std::string& qid = map.at(y).at(x);
            if (qid == ".") continue;
            const int digit = std::stoi(qid);
            quad_score += digit;
        }
    }

    return quad_score;
}

int run(map_t& map, std::vector<Robot> robots)
{
    int i = 0;
    while(i < SIM_SECONDS)
    {
        for(auto& rbt : robots)
        {
            handle_robot(map, rbt);
        }
        ++i;
    }

    for(auto& rbt : robots)
    {
        auto& cell = map.at(rbt.y).at(rbt.x);
        if (cell != ".")
        {
            const int inc_cell = std::stoi(cell) + 1;
            cell = static_cast<char>(std::to_string(inc_cell).at(0));
            continue;
        }
        cell = "1";
    }

    //print_map(map);

    int mulres = 1;
    for(int qidx = 1; qidx < 5; ++qidx)
    {
        const int qscore = calculate_quadrant(map, qidx);
        //std::cout << "Quadrant " << qidx << " score: " << qscore << "\n";
        mulres *= qscore;
    }

    return mulres;
}

int main()
{
    map_t map;
    std::vector<Robot> robots;

    make_map(map);
    read_input(robots, "./input.txt");

    std::cout << run(map, robots) << "\n";

    return 0;
}
