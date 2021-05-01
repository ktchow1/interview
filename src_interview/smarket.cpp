#include<iostream>
#include<vector>
#include<map>
#include<math.h>

// ************************************************* //
// *** Less than target subseq product in O(N^3) *** //
// ************************************************* //
std::optional<std::uint32_t> lt_target_ssp0(const std::vector<std::uint64_t>& vec, std::uint64_t target)
{
    std::uint32_t count = 0;
    for(std::uint32_t n=0; n!=vec.size(); ++n) // begin
    {
        for(std::uint32_t m=n+1; m!=vec.size()+1; ++m) // end
        {
            std::uint64_t product = 1;
            for(std::uint32_t k=n; k!=m; ++k) 
            {
                auto product_old = product;
                product *= vec[k];                
                if (product < product_old) 
                {
                    // Overflow detected
                    return std::nullopt;
                }
            }
            if (product <= target) ++count;
        }
    }
    return std::make_optional(count);
}

// *********************************************** //
// *** Less than target subseq product in O(N) *** //
// *********************************************** //
// These two ceil-methods are verified to be the same.
//
std::uint64_t quotient_ceil_method0(std::uint64_t product, std::uint64_t target)
{
    return (std::uint64_t) ceil((double)product / target);  // BUG3
}

std::uint64_t quotient_ceil_method1(std::uint64_t product, std::uint64_t target)
{
    std::uint64_t ans = product / target; 
    if (product % target != 0) ++ans;  
    return ans;
}

std::uint32_t lt_target_ssp1(const std::vector<std::uint64_t>& vec, std::uint64_t target)
{
    std::map<std::uint64_t, std::uint64_t> hist;
    std::uint64_t product = 1;
    std::uint32_t count = 0;
    for(std::uint32_t n=0; n!=vec.size(); ++n)
    {
        // update product
        product *= vec[n];

        // update count 
        if (product <= target)
        {
            count += n+1; // BUG1
        }
        else
        {
            std::uint64_t temp = quotient_ceil_method1(product, target); 
            auto iter0 = hist.lower_bound(temp); 
            if (iter0 != hist.end())
            {
                count += n-iter0->second; // BUG2
            }
        }

        // update histogram
        auto iter1 = hist.find(product);
        if (iter1 == hist.end())
        {
            hist[product] = n;
        }
    }
    return count;
}

// ****************************** //
// *** Point puzzle in O(2^N) *** //
// ****************************** //
std::uint32_t point_puzzle0(std::vector<std::uint32_t>& vec)
{
    return 0;
}

// **************************** //
// *** Point puzzle in O(N) *** //
// **************************** //
std::uint32_t point_puzzle1(std::vector<std::uint32_t>& vec)
{
    return 0;
}

// *********************** //
// *** Test programmes *** //
// *********************** //
void test_smarket2_LT_subseq_product()
{
    std::uint32_t count_success = 0;
    std::uint32_t count_done = 0;
    std::uint64_t T = 1000;    // number of trials
    std::uint64_t N = 15;      // number of elements, overflow happens when N > 15

    for(std::uint64_t t=0; t!=T; ++t)
    {
        std::vector<std::uint64_t> vec;
        std::uint64_t target = 1;

        for(std::uint64_t n=0; n!=N; ++n)
        {
            vec.push_back(rand()%20+1); // Numbers are all positive.
            target *= 2;
        }

        auto ans0 = lt_target_ssp0(vec, target);
        auto ans1 = lt_target_ssp1(vec, target);

        if (ans0)
        {
            ++count_done;
            if (*ans0 == ans1) ++count_success;
            std::cout << "\nTrial " << t << " : " 
                                    << *ans0 << ", "  
                                    <<  ans1 << " success = " 
                                    << count_success << "/" 
                                    << count_done;
        }
    }
}

void test_smarket3_point_puzzle()
{
}





