#include <algorithm>
#include <iostream>
#include <fstream>
#include <unordered_set>
#include <vector>

#define RETVAL 999999999
//#define SHOW_INFO

struct location_vec
{
    int32_t x;
    int32_t y;
};

struct Button
{
    std::string id;
    location_vec movement_vel;
};

struct Claw
{
    location_vec position;
};

struct ClawMachine
{
    std::string id;
    Claw claw;
    Button a_button;
    Button b_button;
    location_vec prize_location;
};

using visited_t = std::unordered_set<std::string>;

inline std::string location_to_string(const location_vec& loc)
{
    return std::to_string(loc.x) + ',' + std::to_string(loc.y);
}

void read_input(std::vector<ClawMachine>& machines, const std::string& filepath)
{
    std::ifstream iff(filepath);
    if (!iff.is_open()) return;

    std::vector<Button> local_buttons;
    std::string holder("");
    int machines_processed = 0;
    int row_ctr = 1;
    while(std::getline(iff, holder))
    {
        //std::cout << "holder: " << holder << "\n";

        if (holder.find("Button") != std::string::npos)
        {
            std::string button_type = "";

            if (holder.find("A") != std::string::npos)
            {
                button_type = "A";
            }
            else if (holder.find("B") != std::string::npos)
            {
                button_type = "B";
            }
            else
            {
                std::cout << "Cannot determine button type on '" << row_ctr << "'th row\n";
                continue;
            }

            const auto xvloc = holder.find('X');
            const auto yvloc = holder.find('Y');

            //std::cout << "xvloc: " << xvloc << "\n";
            //std::cout << "yvloc: " << yvloc << "\n";

            int xvel = std::stol(holder.substr(xvloc + 2, holder.find(',')));
            int yvel = std::stol(holder.substr(yvloc + 2, holder.size()));

            //std::cout << "xvel: " << xvel << "\n";
            //std::cout << "yvel: " << yvel << "\n";

            local_buttons.push_back({std::move(button_type), {xvel, yvel}});
        }
        else if (holder.find("Prize") != std::string::npos)
        {
            ClawMachine machine;
            machine.id = std::to_string(machines_processed + 1);
            machine.claw = Claw{{0, 0}};

            const auto xvloc = holder.find('X');
            const auto yvloc = holder.find('Y');

            int xvel = std::stol(holder.substr(xvloc + 2, holder.find(',')));
            int yvel = std::stol(holder.substr(yvloc + 2, holder.size()));

            //std::cout << "prize xvel: " << xvel << "\n";
            //std::cout << "prize yvel: " << yvel << "\n";

            machine.prize_location.x = xvel;
            machine.prize_location.y = yvel;

            for(auto& butt : local_buttons)
            {
                if (butt.id == "A")
                {
                    machine.a_button = std::move(butt);
                    continue;
                }
                else if (butt.id == "B")
                {
                    machine.b_button = std::move(butt);
                    continue;
                }
            }
            machines.push_back(std::move(machine));
            ++machines_processed;
        }

        ++row_ctr;
    }
}

int token_cost_help(ClawMachine& machine, visited_t& visited, Claw claw, int x_cost, int y_cost)
{
    const auto locstr = location_to_string(claw.position);

    if (visited.find(locstr) != visited.end()) return RETVAL;

    visited.insert(locstr);

    if (claw.position.x > machine.prize_location.x || claw.position.y > machine.prize_location.y)
    {
        #ifdef SHOW_INFO
        std::cout << "Claw Pos: " << claw.position.x << "," << claw.position.y << "\n";
        std::cout << "\n";
        #endif // SHOW_INFO

        return RETVAL;
    }

    if (claw.position.x == machine.prize_location.x && claw.position.y == machine.prize_location.y)
    {
        #ifdef SHOW_INFO
        std::cout << "Claw for machine '" << machine.id << "' hit end point\n";
        std::cout << "x_cost: " << x_cost << "\n";
        std::cout << "y_cost: " << y_cost << "\n";
        #endif // SHOW_INFO

        return x_cost * 3 + y_cost;
    }

    auto& a_button = machine.a_button;
    auto& b_button = machine.b_button;

    auto new_xloc = claw.position.x + a_button.movement_vel.x;
    auto new_yloc = claw.position.y + a_button.movement_vel.y;

    const auto lt_cost_a = token_cost_help(machine, visited, Claw{{new_xloc, new_yloc}}, x_cost + 1, y_cost);

    new_xloc = claw.position.x + b_button.movement_vel.x;
    new_yloc = claw.position.y + b_button.movement_vel.y;

    const auto lt_cost_b = token_cost_help(machine, visited, Claw{{new_xloc, new_yloc}}, x_cost, y_cost + 1);

    #ifdef SHOW_INFO
    if (lt_cost_a != RETVAL || lt_cost_b != RETVAL)
    {
        std::cout << "lt_cost_a: " << lt_cost_a << "\n";
        std::cout << "lt_cost_b: " << lt_cost_b << "\n";
        std::cout << "\n";
    }
    #endif // SHOW_INFO

    return std::min(lt_cost_a, lt_cost_b);
}

int calculate_token_cost(std::vector<ClawMachine>& machines)
{
    int t_cost = 0;

    for(auto& machine: machines)
    {
        visited_t visited;
        const auto machine_cost = token_cost_help(machine, visited, machine.claw, 0, 0);
        if (machine_cost == RETVAL) continue;

        std::cout << "Machine '" << machine.id << "' costs '" << machine_cost << "'\n";
        t_cost += machine_cost;
    }

    return t_cost;
}

int main()
{
    std::vector<ClawMachine> machines;
    read_input(machines, "input.txt");
    const int token_cost = calculate_token_cost(machines);

    std::cout << "token_cost: " << token_cost << "\n";

    return 0;
}
