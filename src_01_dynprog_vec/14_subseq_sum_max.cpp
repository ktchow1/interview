#include<iostream>
#include<vector>
#include<unordered_map>

std::uint32_t max_subseq_sum(const std::vector<std::uint32_t>& vec)
{
    if (vec.size() < 1) return 0;

    std::uint32_t msp = vec[0];    
    std::uint32_t ans = msp;    
    for(std::uint32_t n=1; n!=vec.size(); ++n)
    {
        msp = std::max(msp+vec[n], vec[n]);
        ans = std::max(msp, ans);
    }
    return ans;
}

std::uint32_t max_subseq_product(const std::vector<std::uint32_t>& vec)
{
    if (vec.size() < 1) return 0;

    std::uint32_t msp0 = vec[0];    
    std::uint32_t msp1 = vec[0];    
    std::uint32_t ans  = vec[0];    
    for(std::uint32_t n=1; n!=vec.size(); ++n)
    {
        auto tmp0 = msp0;
        auto tmp1 = msp1;
        msp0 = std::max(tmp0*vec[n], std::max(tmp1*vec[n], vec[n]));
        msp1 = std::min(tmp0*vec[n], std::min(tmp1*vec[n], vec[n]));
        ans  = std::max(msp0, ans);
    }
    return ans;
}

std::uint32_t max_non_contiguous_subseq_sum(const std::vector<std::uint32_t>& vec)
{
    if (vec.size() < 1) return 0;

    std::uint32_t msp0 = vec[0]; // include the last element  
    std::uint32_t msp1 = 1;      // enclude the last element  
    std::uint32_t ans  = std::max(msp0, msp1);    
    for(std::uint32_t n=1; n!=vec.size(); ++n)
    {
        auto tmp0 = msp0;
        auto tmp1 = msp1;
        msp0 = std::max(tmp0+vec[n], std::max(tmp1+vec[n], vec[n]));
        msp1 = std::max(tmp0, tmp1);
        ans  = std::max(msp0, std::max(msp1, ans));
    }
    return ans;
}

std::uint32_t max_puzzle_game(const std::vector<std::uint32_t>& vec)
{
    std::uint32_t max = *std::max_element(vec.begin(), vec.end());
    std::uint32_t min = *std::min_element(vec.begin(), vec.end());

    std::unordered_map<std::uint32_t, std::uint32_t> hist;    
    for(const auto& x:vec)
    {
        if (auto iter = hist.find(x); iter!=hist.end())
        {
            ++iter->second;
        }
        else
        {
            hist[x] = 1;
        }
    }

    std::uint32_t msp0 = min; // include the last element
    std::uint32_t msp1 = 0;   // exclude the last element
    std::uint32_t ans;
    for(std::uint32_t n=min+1; n<=max; ++n)
    {
        auto tmp0 = msp0;
        auto tmp1 = msp1;
        if (auto iter=hist.find(n); iter!=hist.end())
        {
            msp0 = std::max(tmp1+iter->first*iter->second, 
                                 iter->first*iter->second);
            msp1 = std::max(tmp0, tmp1); 
            ans  = std::max(msp0, msp1);
        }
        else
        {
        //  msp0 = unchanged
            msp1 = std::max(tmp0, tmp1); 
            ans  = std::max(msp0, msp1);
        }
    }
    return ans;
}
