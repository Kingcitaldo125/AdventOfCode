#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

using graph_t = std::unordered_map<std::string,std::vector<std::string>>;

std::vector<std::string> split_string(const std::string& str)
{
    std::vector<std::string> splits;
    std::stringstream ss(str);
    std::string holder;
    while(std::getline(ss,holder,' '))
    {
        splits.push_back(holder);
    }
    return splits;
}

graph_t read_graph(const std::string& filename)
{
    graph_t graph;
    std::ifstream iff(filename, std::ios::in);
    std::string holder;
    const std::string field_separator(": ");
    while(std::getline(iff,holder))
    {
        const auto splitter = holder.find(field_separator);
        const std::string key = holder.substr(0,splitter);
        const std::string values = holder.substr(splitter + field_separator.size(), holder.size());
        const auto splits = split_string(values);
        for(const auto& sp : splits)
        {
            graph[key].push_back(sp);
        }
    }

    return graph;
}

void find_path(graph_t& graph, const std::string& cur, const std::string& target, int& path_count)
{
    if (cur == target)
        ++path_count;

    for(const auto& n : graph[cur])
    {
        find_path(graph,n,target,path_count);
    }
}

inline int count_paths(const std::string& filename)
{
    auto graph = read_graph(filename);
    const std::string target("out");
    int path_count = 0;
    find_path(graph,"you",target,path_count);
    return path_count;
}

int main()
{
    std::cout << count_paths("devices.txt") << "\n";
    return 0;
}
