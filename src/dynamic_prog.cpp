#include<iostream>
#include<set>
#include<map>
#include<unordered_set>
#include<unordered_map>
#include<optional>

// Three loops :
// 1. time domain
// 2. state domain (x = value of coins / weight of items / time of tasks)
// 3. value domain (y = number of coins / value of items / profit of tasks)

std::optional<std::uint32_t> coin_change(const std::set<std::uint32_t>& coins, std::uint32_t target)
{
    std::unordered_set<std::uint32_t> state0;
    std::uint32_t iteration = 1;

    while(true)        
    {
        std::unordered_set<std::uint32_t> state1;
        for(const auto& x:state0)
        {
            for(const auto& dx:coins)
            {
                auto temp = x+dx;
                if (temp <= target) state1.insert(temp);
            }
        }

        // *** Exit condition *** //
        if (state1.find(target) != state1.end()) 
        {
            return std::make_optional(iteration);
        }
        if (state1.empty()) 
        {
            return std::nullopt;
        }
    
        // *** Move to next iteration *** //
        state0 = std::move(state1);
        ++iteration;
    }
    return std::nullopt;
}

// ********************************************************************* //
// For knapsack, we decouple :
// 1. graph for objective function (like the DAG in Dijkstra)
// 2. state for region growing     (like the priority queue in Dijkstra)
// They are merged into the same unordered_set in coin-change problem.
// ********************************************************************* //
struct item
{
    std::uint32_t weight; // x
    std::uint32_t value;  // y
};

std::optional<std::uint32_t> knapsack(const std::set<item>& items, std::uint32_t limit)
{
    std::map<std::uint32_t, std::uint32_t> objective; // total-weight vs total-value
    std::unordered_map<std::uint32_t, std::uint32_t> state0;
    
    while(true)
    {
        std::unordered_map<std::uint32_t, std::uint32_t> state1;
        for(const auto& x:state0)
        {
            for(const auto& dx:items)
            {
                auto total_weight = x.first + dx.weight;
                auto total_value  = x.second + dx.value;
                if (total_weight <= limit) 
                {
                    // *** Update objective *** //
                    auto iter = objective.find(total_weight);
                    if (iter != objective.end())
                    {
                        if (iter->second < total_value)
                            iter->second = total_value;
                    }
                    else iter->second = total_value;

                    // *** Update region-growing queue *** //
                    state1.insert(std::make_pair(total_weight, total_value));
                }
            }
        }

        // *** Exit condition *** //
        if (state1.empty())
        {
            auto iter = objective.upper_bound(limit);
            if (iter != objective.end())
            {
                return std::make_optional(iter->second);
            }
        }

        // *** Move to next iteration *** //
        state0 = std::move(state1);
    }
}

struct job 
{
    std::uint32_t load;     // x
    std::uint32_t profit;   // y
    std::uint32_t deadline; // constraint
};

// Each job should be done once or never.
std::uint32_t job_schedule(const std::set<job>& jobs)
{
    return 0;
}
