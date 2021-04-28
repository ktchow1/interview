// Three loops :
// 1. time domain
// 2. state domain (x = value of coins / weight of items / time of tasks)
// 3. value domain (y = number of coins / value of items / profit of tasks)

std::uint32_t coin_change(const std::set<std::uint32_t>& coins, std::uint32_t target)
{
}

struct item
{
    std::uint32_t weight; // x
    std::uint32_t value;  // y
};

std::uint32_t knapsack(const std::set<item>& items, std::uint32_t limit)
{
}

struct job 
{
    std::uint32_t load;     // x
    std::uint32_t profit    // y
    std::uint32_t deadline; // constraint
};

// Each job should be done once or never.
std::uint32_t job_schedule(const std::set<job>& jobs)
{
}
