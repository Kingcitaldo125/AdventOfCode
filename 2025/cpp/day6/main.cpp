#include <fstream>
#include <iostream>
#include <functional>
#include <numeric>
#include <sstream>
#include <vector>

struct homework_t
{
    std::vector<std::vector<int>> factors;
    std::vector<char> operands;
    int rows;
    int columns;
};

std::vector<std::string> split_string(const std::string& str)
{
    std::vector<std::string> splits;
    std::stringstream ss(str);
    std::string holder;
    while(ss >> holder)
    {
        splits.push_back(std::move(holder));
    }

    return splits;
}

homework_t get_problems(const std::string& filename)
{
    homework_t homework;
    std::string holder;
    {
        std::ifstream iff(filename, std::ios::in);
        std::getline(iff, holder);
        homework.columns = static_cast<int>(split_string(holder).size());

        if (homework.columns > 0)
        {
            homework.rows = 1;
            while(std::getline(iff,holder))
            {
                ++homework.rows;
            }
        }
    }

    /*
    std::cout << "rows: " << homework.rows << '\n';
    std::cout << "columns: " << homework.columns << '\n';
    */

    std::ifstream iff(filename, std::ios::in);
    holder = "";
    int icount = 1;
    while(std::getline(iff,holder))
    {
        const auto xrow = split_string(holder);
        //std::cout << "icount: " << icount << std::endl;
        if (icount < homework.rows)
        {
            std::vector<int> row;
            row.reserve(homework.columns);
            for(const auto& c : xrow)
            {
                //std::cout << "col: " << c << std::endl;
                row.push_back(std::stoi(c));
            }
            homework.factors.push_back(std::move(row));
        }
        else if(icount == homework.rows)
        {
            std::vector<char> row;
            row.reserve(homework.columns);
            for(const auto& c : xrow)
            {
                row.push_back(c.front());
            }
            homework.operands = std::move(row);
        }
        ++icount;
    }

    return homework;
}

int compute_problems(const std::string& filename)
{
    const auto homework = get_problems(filename);
    const auto& factors = homework.factors;
    const auto& operands = homework.operands;
    const int rows = homework.rows;
    const int columns = homework.columns;
    int totals = 0;

    for(std::size_t i = 0; i < columns; ++i)
    {
        std::vector<int> xfacts;
        const char op = operands[i];
        for(std::size_t j = 0; j < rows - 1; ++j)
        {
            xfacts.push_back(factors[j][i]);
        }

        if (op == '+')
        {
            const auto xsums = std::accumulate(xfacts.begin(),xfacts.end(),0,std::plus<int>());
            totals += static_cast<int>(xsums);
        }
        else if (op == '*')
        {
            const auto xprods = std::accumulate(xfacts.begin(),xfacts.end(),1,std::multiplies<int>());
            totals += static_cast<int>(xprods);
        }
    }

    return totals;
}

inline void gp_test()
{
    const auto homework = get_problems("problems.txt");

    puts("factors:");
    for(const auto& row : homework.factors)
    {
        for(const auto& r : row)
            std::cout << r << " ";
        std::cout << "\n";
    }

    puts("operands:");
    for(const auto& o : homework.operands)
    {
        std::cout << o << "\n";
    }
}

int main(int argc, char** argv)
{
    std::cout << compute_problems("problems.txt") << "\n";
    return 0;
}
