#include<iostream>
#include<vector>

std::int32_t max_profit(const std::vector<std::int32_t>& vec)
{
    if (vec.size()<2) return 0;
    std::int32_t msp = vec[1]-vec[0];
    std::int32_t ans = msp;

    for(std::uint32_t n=2; n!=vec.size(); ++n)
    {
        msp = std::max(msp-vec[n-1]+vec[n], vec[n]-vec[n-1]);
        ans = std::max(ans, msp);
    }
    return ans;
}

// Compare dynamic programming with exhaustive search in test
void test_2ptr_diff_max()
{
}

