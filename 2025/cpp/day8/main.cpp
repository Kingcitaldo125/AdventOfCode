#include <cmath>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using graph_t = std::unordered_map<std::string,std::vector<std::string>>;
using visited_t = std::unordered_set<std::string>;

class JunctionBox
{
 public:
    JunctionBox(const int xx, const int yy, const int zz) : x(xx), y(yy), z(zz) {}
    friend std::ostream& operator<<(std::ostream& ostream, const JunctionBox& other);
    double distance_to(const JunctionBox& other)
    {
        const double xdiff = x - other.x;
        const double ydiff = y - other.y;
        const double zdiff = z - other.z;
        return sqrt(xdiff * xdiff + ydiff * ydiff + zdiff * zdiff);
    }
    std::string to_string() const
    {
        std::stringstream ss;
        ss << *this;
        return ss.str();
    }

 private:
    int x;
    int y;
    int z;
};

std::ostream& operator<<(std::ostream& ostream, const JunctionBox& other)
{
    ostream << other.x << "," << other.y << "," << other.z;
    return ostream;
}

JunctionBox get_jbox(const std::string& item)
{
    std::stringstream ss(item);
    std::string holder;
    std::vector<int> parts;
    while (std::getline(ss,holder,','))
    {
        parts.push_back(std::stoi(holder));
    }
    return {parts[0],parts[1],parts[2]};
}

std::vector<JunctionBox> read_box_input(const std::string& filename)
{
    std::vector<JunctionBox> boxes;
    std::ifstream iff(filename, std::ios::in);
    std::string holder;
    while(iff >> holder)
    {
        boxes.push_back(get_jbox(holder));
    }
    return boxes;
}

bool same_circuit_help(graph_t& graph, visited_t& visited, const std::string& cur, const std::string& to_find)
{
    visited.insert(cur);

    if (cur == to_find)
        return true;

    for(const auto& neighbor : graph[cur])
    {
        if (visited.find(neighbor) != visited.end())
            continue;

        const auto res = same_circuit_help(graph, visited, neighbor, to_find);
        if (res)
            return res;
    }

    return false;
}

bool same_circuit(graph_t& graph, const std::string& first, const std::string& second)
{
    bool same = false;
    {
        visited_t visited;
        same |= same_circuit_help(graph, visited, first, second);
    }
    {
        visited_t visited;
        same |= same_circuit_help(graph, visited, second, first);
    }
    return same;
}

graph_t make_graph(const std::string& filename)
{
    using box_triplet_t = std::vector<std::tuple<double,JunctionBox,JunctionBox>>;

    graph_t graph;
    auto boxes = read_box_input(filename);
    box_triplet_t box_triplets;

    for(auto it = boxes.begin(); it < boxes.end(); ++it)
    {
        for(auto jt = it + 1; jt < boxes.end(); ++jt)
        {
            box_triplets.push_back(std::make_tuple(it->distance_to(*jt),*it,*jt));
        }
        graph[it->to_string()] = {};
    }

    std::sort(box_triplets.begin(),box_triplets.end(),[](const auto& first, const auto& second)
    {
        const auto [dist1,_,__] = first;
        const auto [dist2,___,____] = second;
        return dist1 < dist2;
    });

    //visited_t box_set;
    for(auto it = box_triplets.begin(); it < box_triplets.end(); ++it)
    {
        const auto [dist,jbox1,jbox2] = *it;
        const auto j1 = jbox1.to_string();
        const auto j2 = jbox2.to_string();

        //std::cout << "j1 " << j1 << " j2 " << j2 << "\n";

        if (same_circuit(graph,j1,j2))
            continue;

        graph[j1].push_back(j2);
        graph[j2].push_back(j1);
    }

    return graph;
}

int count_circuit_sizes(const std::string& filename)
{
    std::vector<int> sizes{0,0,0};
    std::unordered_set<int> sizes_set;
    auto graph = make_graph(filename);
    for(const auto& node : graph)
    {
        /*
        std::cout << node.first << ": ";
        for(const auto& item : node.second)
        {
            std::cout << item << " ";
        }
        puts("");
        */

        visited_t visited;
        const int count = 1 + static_cast<int>(node.second.size());
        //std::cout << "count: " << count << "\n";
        //std::cout << "\n";

        const auto [_,inserted] = sizes_set.insert(count);
        if (!inserted)
            continue;

        sizes.push_back(count);
    }

    std::sort(sizes.begin(), sizes.end(), [](const int a, const int b)
    {
        return a >= b;
    });

    int prod = 1;
    for(int i = 0; i < 3; ++i)
    {
        //std::cout << sizes[i] << "\n";
        prod = prod * sizes[i];
    }

    return prod;
}

int main(int argc, char** argv)
{
    std::cout << count_circuit_sizes("boxes.txt") << "\n";
    return 0;
}
