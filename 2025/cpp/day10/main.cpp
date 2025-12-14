#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

struct Machine
{
    //Machine() : presses(0) {}
    friend std::ostream& operator<<(std::ostream& stream, const Machine& machine);
    std::vector<bool> states;
    std::vector<bool> scheme;
    std::vector<std::vector<int>> button_presses;
    std::vector<int> joltages;
    //std::size_t presses;
};

std::ostream& operator<<(std::ostream& stream, const Machine& machine)
{
    stream << "[";
    for(const auto& itm : machine.scheme)
    {
        stream << (itm ? '#' : '.');
    }
    stream << "] ";

    stream << "([";
    for(const auto& itm : machine.states)
    {
        stream << (itm ? '#' : '.');
    }
    stream << "]) ";

    for(const auto& itm : machine.button_presses)
    {
        stream << "(";
        for(auto it = itm.begin(); it < itm.end() - 1; ++it)
        {
            stream << *it << ",";
        }
        stream << itm.back() << ") ";
    }

    stream << "{";
    for(auto it = machine.joltages.begin(); it < machine.joltages.end() - 1; ++it)
    {
        stream << *it << ",";
    }
    stream << machine.joltages.back() << "}";

    return stream;
}

inline void process_button_press(Machine& machine, const std::string& press)
{
    std::stringstream ss(press);
    std::string holder;
    std::vector<int> presses;
    while(std::getline(ss,holder,','))
    {
        presses.push_back(std::stoi(holder));
    }
    machine.button_presses.push_back(std::move(presses));
}

inline void process_joltages(Machine& machine, const std::string& jolts)
{
    std::stringstream ss(jolts);
    std::string holder;
    std::vector<int> xjolts;
    while(std::getline(ss,holder,','))
    {
        xjolts.push_back(std::stoi(holder));
    }
    machine.joltages = std::move(xjolts);
}

std::vector<Machine> read_machines(const std::string& filename)
{
    std::vector<Machine> machines;
    std::ifstream iff(filename, std::ios::in);
    std::string holder;
    while(std::getline(iff, holder))
    {
        Machine machine;
        std::stringstream ss(holder);
        std::string holder2;
        while(std::getline(ss, holder2, ' '))
        {
            //std::cout << "holder2: " << holder2 << "\n";
            const char first_chr = holder2.front();
            if (first_chr == '[')
            {
                const auto close = holder2.find(']');
                const auto hsize = holder2.substr(1,close-1).size();
                //std::cout << "hsize: " << hsize << "\n";
                int idx = 0;
                machine.states = std::vector<bool>(hsize);
                machine.scheme = std::vector<bool>(hsize);
                for(const auto& itm : holder2.substr(1,close-1))
                {
                    //std::cout << "itm: " << itm << "\n";
                    machine.scheme[idx] = itm == '#';
                    ++idx;
                }
            }
            else if (first_chr == '(')
            {
                const auto close = holder2.find(')');
                process_button_press(machine, holder2.substr(1,close-1));
            }
            else if (first_chr == '{')
            {
                const auto close = holder2.find('}');
                process_joltages(machine, holder2.substr(1,close-1));
            }
        }

        machines.push_back(std::move(machine));
    }

    return machines;
}

inline bool same_scheme(const std::vector<bool>& scheme_a, const std::vector<bool>& scheme_b)
{
    if (scheme_a.size() != scheme_b.size())
        return false;

    for(std::size_t i = 0; i < scheme_a.size(); ++i)
    {
        if (scheme_a[i] != scheme_b[i])
            return false;
    }
    return true;
}

std::size_t determine_presses_help(Machine machine, std::size_t presses, std::size_t& tcombos)
{
    if (same_scheme(machine.states, machine.scheme))
    {
        //std::cout << "same machine " << machine << " presses (" << presses << ")\n";
        tcombos = std::min(presses, tcombos);
        return presses;
    }

    if (presses > tcombos)
        return tcombos;

    std::vector<std::size_t> press_mags{tcombos};
    for(const auto& press_config : machine.button_presses)
    {
        Machine machine2 = machine;
        //std::cout << "1machine " << machine << " presses (" << presses << ") (";
        for(const auto& itm : press_config)
        {
            machine2.states[itm] = !machine.states[itm];
            //std::cout << itm << ",";
        }
        //std::cout << ")\n2machine " << machine << " presses (" << presses << ")\n\n";
        press_mags.push_back(determine_presses_help(std::move(machine2), presses + 1, tcombos));
    }
    return *std::min_element(press_mags.begin(), press_mags.end());
}

inline std::size_t determine_presses(Machine machine)
{
    std::size_t tcombos = machine.states.size() * machine.button_presses.size();
    return determine_presses_help(machine, 0, tcombos);
}

int count_presses(const std::string& filename)
{
    std::vector<std::size_t> presses;
    const auto machines = read_machines(filename);
    for(const auto& machine : machines)
    {
        //std::cout << machine << "\n";
        presses.push_back(determine_presses(machine));
    }

    int count = 0;
    for(const auto& press : presses)
    {
        ++count;
    }
    return count;
}

int main()
{
    std::cout << count_presses("machines.txt") << "\n";
    return 0;
}
