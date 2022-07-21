#include<iostream>
#include<vector>
#include<map>
#include<unordered_map>

std::uint32_t count_target_subseq_sum(const std::vector<std::uint32_t>& vec, std::uint32_t target)
{
    std::unordered_map<std::uint32_t, std::uint32_t> hist;

    std::uint32_t cum=0;
    std::uint32_t ans=0;
    for(std::uint32_t n=0; n!=vec.size(); ++n)
    {
        cum += vec[n];
        if (auto iter=hist.find(target-cum); iter!=hist.end())
        {
            ans += iter->second;
        }
        if (auto iter=hist.find(cum); iter!=hist.end())
        {
            ++iter->second;
        }
        else 
        {
            hist[cum] = 1;
        }
    }
    return ans;
}

std::uint32_t count_target_divisible_subseq_sum(const std::vector<std::uint32_t>& vec, std::uint32_t target)
{
    std::vector<std::uint32_t> hist{target, 0}; // This pattern is slightly different, since vector is used as hist.

    std::uint32_t cum=0;
    std::uint32_t ans=0;
    for(std::uint32_t n=0; n!=vec.size(); ++n)
    {
        cum += vec[n];
        ans += hist[cum % target];
        ++hist[cum % target];
    }
    return ans;
}


// Find m such that : 
//   
//      cum - cum[m] < target, where cum = vec[0]+vec[1]+...+vec[n]
// i.e. cum[m] > cum - target 
//
std::uint32_t count_less_than_target_subseq_sum(const std::vector<std::uint32_t>& vec, std::uint32_t target)
{
    std::map<std::uint32_t, std::uint32_t> index; // index[sum] = n, means vec[0]+...vec[n] = sum

    std::uint32_t cum=0;
    std::uint32_t ans=0;
    for(std::uint32_t n=0; n!=vec.size(); ++n)
    {
        cum += vec[n];
        if (cum < target)
        {
            ++ans;
        }
        else if (auto iter=index.upper_bound(cum-target); iter!=index.end())
        {
            ans += n-iter->second;
        }
        index[cum] = n; // theoretically, the same entry will not be overwritten
    }
    return ans;
}

std::uint32_t count_less_than_target_subseq_sum_exhaustive(const std::vector<std::uint32_t>& vec, std::uint32_t target)
{
    std::uint32_t ans=0;
    for(std::uint32_t n=0; n!=vec.size(); ++n)
    {
        std::uint32_t cum=0;
        for(std::uint32_t m=n+1; m!=vec.size(); ++m)
        {
            cum += vec[m];
            if (cum < target) ++ans;
        }
    }
    return ans;
}

// Find m such that : 
//   
//      cum / cum[m] < target, where cum = vec[0]*vec[1]*...*vec[n]
// i.e. cum[m] > cum / target 
//
// Consider target = 23 :
// vec[10] = x     cum[10] =  1000    hist[ 1000] = 10
// vec[11] = 5     cum[11] =  5000    hist[ 5000] = 11
// vec[12] = 3     cum[12] = 15000    hist[15000] = 12
// vec[13] = 4     cum     = 60000    cum/target  = 60000/23 = 2608.7
//
std::uint32_t count_less_than_target_subseq_product(const std::vector<std::uint32_t>& vec, std::uint32_t target)
{
    std::map<std::uint32_t, std::uint32_t> index;

    std::uint32_t cum=1;
    std::uint32_t ans=0;
    for(std::uint32_t n=0; n!=vec.size(); ++n)
    {
        cum *= vec[n];
        if (cum < target)
        {
            ++ans;
        }        
        else if (auto iter=index.upper_bound(cum/target); iter!=index.end())
        {
            ans += n-iter->second;
        }
        index[cum] = n; 
    }
    return ans;
}

std::uint32_t longest_target_subseq_sum(const std::vector<std::uint32_t>& vec, std::uint32_t target)
{
    std::unordered_map<std::uint32_t, std::uint32_t> index; // index[sum] = min(n), where vec[0]+...vec[n] = sum

    std::uint32_t cum=0;
    std::uint32_t ans=0;
    for(std::uint32_t n=0; n!=vec.size(); ++n)
    {
        cum += vec[n];
        if (auto iter=index.find(target-cum); iter!=index.end())
        {
            ans = std::max(ans, n-iter->second);
        }
        if (auto iter=index.find(cum); iter==index.end())
        {
            index[cum] = n;
        }
    }
    return ans;
}

void test_count_less_than_target_subseq_sum()
{
    for(std::uint32_t n=0; n!=1; ++n)
    {
        std::vector<std::uint32_t> vec;
        for(std::uint32_t m=0; m!=10; ++m) vec.push_back(rand()%10);
        std::cout << "\ncount less than target subseq sum : " << count_less_than_target_subseq_sum(vec, 30) <<
                                                         ", " << count_less_than_target_subseq_sum_exhaustive(vec, 30);
    }
}

void test_subseq_sum_count()
{
    test_count_less_than_target_subseq_sum();
}
