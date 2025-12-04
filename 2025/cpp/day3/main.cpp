#include <algorithm>
#include <fstream>
#include <functional>
#include <numeric>
#include <iostream>
#include <vector>

std::vector<std::string> get_power_banks(const std::string& filename)
{
    std::ifstream iff(filename, std::ios::in);
    std::vector<std::string> banks;
    std::string holder;

    while(iff >> holder)
    {
        banks.push_back(std::move(holder));
    }

    return banks;
}

int find_largest_joltage(const std::string& mstr)
{
    std::string digits;
    auto begit = mstr.begin();
    auto endit = mstr.end();
    auto cit = begit;
    int last_el = -1;
    while(begit < endit)
    {
        cit = std::max_element(begit,endit);

        if (std::distance(cit,mstr.end() - 1) > 0)
            break;

        if (*cit == last_el)
            break;

        last_el = *cit;
        --endit;
    }

    digits += std::to_string(std::stoi(std::string(1,*cit)));
    digits += std::to_string(std::stoi(std::string(1,*std::max_element(cit + 1,mstr.end()))));
    return std::stoi(digits);
}

int find_output_joltage(const std::string& filename)
{
    const auto banks = get_power_banks(filename);
    std::vector<int> bank_joltages;
    for(const auto& cody_banks : banks)
    {
        bank_joltages.push_back(find_largest_joltage(cody_banks));
    }
    return static_cast<int>(std::accumulate(bank_joltages.begin(), bank_joltages.end(), 0, std::plus<int>()));
}

inline void find_individual_joltages()
{
    std::cout << find_largest_joltage("987654321111111") << "\n";
    std::cout << find_largest_joltage("811111111111119") << "\n";
    std::cout << find_largest_joltage("234234234234278") << "\n";
    std::cout << find_largest_joltage("818181911112111") << "\n";
}

int main(int argc, char** argv)
{
    const auto jolts = find_output_joltage("banks.txt");
    std::cout << jolts << "\n";
    return 0;
}
