#include<iostream>
#include<iomanip>
#include<vector>
#include<iterator>
#include<algorithm>

namespace interview {
// If there is no missing number in vec, return vec.size()+1.
//
// *********************** //
// Method 1 : With sorting
// time  = O(NlogN) 
// space = O(1)     
// *********************** //
std::int32_t min_missing_pos_num_sort(std::vector<std::int32_t>& vec)
{
    std::sort(vec.begin(), vec.end());

    std::int32_t ans = 1;
    for(std::uint32_t n=0; n!=vec.size(); ++n)
    {
        if (vec[n] <= 0)     continue;
        if (vec[n] >  ans)   return ans;
        if (vec[n] == ans)   ++ans;
    //  if (vec[n] == ans-1) duplicated number, we should proceed
    }
    return vec.size()+1; 
}

// **************************** //
// Method 2 : With visited flag
// time  = O(N) 
// space = O(N)     
// **************************** //
std::int32_t min_missing_pos_num_flag(const std::vector<std::int32_t>& vec)
{
    std::vector<bool> flags(vec.size(), false);

    for(const auto& x:vec)
    {
        if (x <= 0)         continue;
        if (x > vec.size()) continue;
        flags[x-1] = true;
    }
    for(std::uint32_t n=0; n!=flags.size(); ++n)
    {
        if (!flags[n]) return n+1;
    }
    return vec.size()+1; 
}

// ****************************************** //
// Method 3 : Reuse input vec as visited flag
// time   = O(N) 
// space  = O(1)     
// assume = all numbers within +/-OFFSET
// ****************************************** //
std::int32_t min_missing_pos_num_bounded(std::vector<std::int32_t>& vec)
{

    return vec.size()+1; 
}

// **************** //
// *** Testing **** //
// **************** //
auto random_int_vector_with_missing(std::uint32_t size)
{
    std::vector<std::int32_t> output;
    std::int32_t answer = 1 + rand() % (size+1);  

    for(std::uint32_t n=0; n!=size; ++n)
    {
        output.push_back(n+1);
    }
    output[answer-1] = size+1; // min missing number
    if (answer < size)
    {
        for(std::uint32_t n=answer; n!=size; ++n)
        {
            std::uint32_t temp = rand()%6;
            if      (temp == 0) output[n]  = -(rand() % size);  // negative numbers
            else if (temp == 1) output[n] +=   rand() % size;   // numbers bigger than answer
            else if (temp == 2) output[n]  =   rand() % answer; // numbers smaller than answer, which are duplicated 
        }
    }

    // *** Shuffle *** //
    for(std::uint32_t n=0; n!=size; ++n)
    {
        std::uint32_t n0 = rand() % size;
        std::uint32_t n1 = rand() % size;
        std::swap(output[n0], output[n1]);
    }
    return std::make_pair(output, answer);
}

void test_archax_min_missing_pos_num()
{
    std::cout << "\n\nArchax : min missing positive number";
    std::uint32_t num_error = 0;
    for(std::uint32_t n=0; n!=20; ++n)
    {
        std::uint32_t num_of_odd = 1 + rand()%10;
        auto [vec,ans] = random_int_vector_with_missing(20);

        std::vector<std::int32_t> vec0(vec);
        std::vector<std::int32_t> vec1(vec);
        std::vector<std::int32_t> vec2(vec);
        std::vector<std::int32_t> vec3(vec);

        std::int32_t ans0 = min_missing_pos_num_sort(vec0); 
        std::int32_t ans1 = min_missing_pos_num_flag(vec1); 
        std::int32_t ans2 = min_missing_pos_num_bounded(vec2); 
//      std::int32_t ans3 = min_missing_pos_num_optimal(vec3); 

        if (ans != ans0 ||
            ans != ans1 || 
            ans != ans2)
//          ans != ans3) 
            ++num_error;

        std::cout << "\ntest " << n 
                  << ", ans = " << ans 
                  << ", ans0 = " << ans0 
                  << ", ans1 = " << ans1 
                  << ", ans2 = " << ans2 
//                << ", ans3 = " << ans3 
                  << ", error = " << num_error << "/" << n+1;


        std::cout << " input ";
        for(const auto& x:vec) std::cout << x << ",";
    }

}
}
