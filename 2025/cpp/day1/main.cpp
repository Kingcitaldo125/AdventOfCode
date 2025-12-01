#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// read the commands from the 'config_file' text file.
std::vector<std::string> read_configuration(const std::string& config_file)
{
    std::vector<std::string> cfg;
    std::ifstream iff(config_file.c_str(), std::ios::in);
    std::string holder;
    while(iff >> holder)
    {
        cfg.push_back(std::move(holder));
    }
    return cfg;
}

int get_password(const std::vector<std::string>& cfg)
{
    constexpr int positions = 100;
    int direction = 50;
    int pcount = 0;
    // read the commands and move the needle accordingly
    for(const auto& c : cfg)
    {
        // extract the parts of the command
        const char xdir = c.front();
        const int mag = std::stoi(c.substr(1,c.size()));
        if (xdir == 'L')
        {
            direction -= mag;
            // cap the range to [0-100)
            direction = direction < 0 ? (direction + positions) : (direction);
        }
        else if (xdir == 'R')
        {
            direction += mag;
            // cap the range to [0-100)
            direction = direction > 99 ? (direction - positions) : (direction);
        }

        if (!direction)
            ++pcount;
    }

    return pcount;
}

int main(int argc, char** argv)
{
    const auto cfg = read_configuration("commands.txt");
    const int passwd = get_password(cfg);
    std::cout << passwd << std::endl;
    return 0;
}
