#include <fstream>
#include <iostream>
#include <unordered_set>
#include <vector>

#define SPLITTER '^'

using diagram_t = std::vector<std::string>;

inline std::string coord_to_string(const int x, const int y)
{
    return std::to_string(x) + "," + std::to_string(y);
}

diagram_t read_diagram(const std::string& filename)
{
    std::ifstream iff(filename, std::ios::in);
    std::string holder;
    diagram_t diagram;
    while(iff >> holder)
    {
        diagram.push_back(std::move(holder));
    }
    return diagram;
}

inline std::pair<int,int> find_start(const diagram_t& diagram)
{
    for(std::size_t i = 0; i < diagram.size(); ++i)
    {
        for(std::size_t j = 0; j < diagram.front().size(); ++j)
        {
            if (diagram[i][j] == 'S')
                return std::make_pair(static_cast<int>(j),static_cast<int>(i));
        }
    }
    return std::make_pair(-1,-1);
}

int process_beam(std::unordered_set<std::string>& splitter_set, const diagram_t& diagram, const int row, const int col)
{
    if (col < 0 || col >= static_cast<int>(diagram.front().size()))
        return 0;
    if (row >= static_cast<int>(diagram.size()))
        return 0;

    if (diagram[row][col] == SPLITTER)
    {
        const auto cstr = coord_to_string(col,row);
        if (splitter_set.find(cstr) != splitter_set.end())
            return 0;

        //std::cout << "hit splitter at (" << col << "," << row << ")\n";
        splitter_set.insert(cstr);
        return 1 + process_beam(splitter_set, diagram, row + 1, col - 1) + process_beam(splitter_set, diagram, row + 1, col + 1);
    }
    return process_beam(splitter_set, diagram, row + 1, col);
}

int count_splits(const std::string& filename)
{
    std::unordered_set<std::string> splitter_set;
    const auto diagram = read_diagram(filename);
    const auto [sx,sy] = find_start(diagram);
    //std::cout << "start: (" << sx << "," << sy << ")\n";
    const int splits = process_beam(splitter_set, diagram, sy + 1, sx);
    //std::cout << "splitters: " << splitters << "\n";
    return splits;
}

int main(int argc, char** argv)
{
    std::cout << count_splits("diagram.txt") << '\n';
    return 0;
}
