#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

struct Range
{
    Range(const int64_t s, const int64_t e) : start(s), end(e) {}
    int64_t start;
    int64_t end;
};

// Check to see if a given subrange has sequences of digits repeated twice
bool valid_subrange(const int64_t r)
{
    if (r < 10)
        return true;

    const auto rstr = std::to_string(r);
    if (rstr.size() % 2 != 0)
        return true;

    // Split the number down the middle and compare both parts
    const auto split_point = rstr.size() / 2;
    const auto ss1 = rstr.substr(0,split_point);
    const auto ss2 = rstr.substr(split_point, rstr.size());
    const bool split_same = ss1 == ss2;
    return !split_same;
}

// Read the config file containing the challenge data
inline std::string read_configuration(const std::string& config_file)
{
    std::ifstream iff(config_file.c_str(), std::ios::in);
    std::string holder;
    while(iff >> holder) {}
    return holder;
}

// Convert a range string (123-456) to a range object
inline Range get_range(const std::string& rstr)
{
    const auto hyph = rstr.find('-');
    const int64_t i = std::stoi(rstr.substr(0,hyph));
    const int64_t j = std::stoi(rstr.substr(hyph + 1,rstr.size()));
    return {i,j};
}

// Split the list of ranges and assemble the ranges into a std::vector of 'Range'
inline std::vector<Range> process_range_cfg(const std::string& cfg)
{
    std::vector<Range> ranges;
    std::stringstream ss(cfg);
    std::string holder;
    while(std::getline(ss,holder,','))
    {
        ranges.push_back(get_range(holder));
    }
    return ranges;
}

// Process the ranges and check for invalid ranges
int64_t check_invalid_ids(const std::string& config_file)
{
    int64_t total_ids = 0;
    const auto ranges = process_range_cfg(read_configuration(config_file));
    for(const auto& range : ranges)
    {
        for(int64_t i = range.start; i <= range.end; ++i)
        {
            if (valid_subrange(i))
                continue;
            total_ids += i;
        }
    }
    return total_ids;
}

int main(int argc, char** argv)
{
    const auto res = check_invalid_ids("ranges.txt");
    std::cout << res << std::endl;
    return 0;
}
