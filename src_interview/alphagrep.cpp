#include<iostream>
#include<vector>



// ****************** //
// *** Question 2 *** //
// ****************** //
// Given vector of unsorted integers, count num of contiguous subsequence, so that it has exactly k odd integers : 
// where k is given and programmable
// 
// * integers are unsorted
// * integers may duplicated
// * identical subsequences (with different composite indices) are counted as two distinct subsequence, 
//   i.e. given 1,2',2",3 then subsequence [1,2',3] != [1,2",3], this relaxation makes thing easier
//
// Ideas :
// * no dynamic programming, subproblem does not help
// * no exhaustive search, it is O(N^3) 
// * do heuristic search
//
namespace interview {
std::uint32_t count_k_odd_subseq(const std::vector<std::uint32_t>& vec, std::uint32_t k)    
{
    std::vector<std::uint32_t> odd_index; // odd_index[m] = n means (m+1)th odd number is vec[n]

    std::uint32_t count=0;
    for(std::uint32_t n=0; n!=vec.size(); ++n)
    {
        if (vec[n]%2==1) 
        {
            odd_index.push_back(n);
        }

        // Add number of valid subseq ending with vec[n]
        if (odd_index.size() == k)
        {
            count += odd_index[0]+1; // subseq [0,n], [1,n] ... [odd_index[0],n]
        }
        else if (odd_index.size() > k)
        {
            auto i0 = odd_index[odd_index.size()-k-1]+1;
            auto i1 = odd_index[odd_index.size()-k];
            count += i1-i0+1; // subseq [i0,n] [i0+1,n] ... [i1,n]
        }
    }
    return count;
}

std::uint32_t count_k_odd_subseq_exhaustive(const std::vector<std::uint32_t>& vec, std::uint32_t num_of_odd)    
{
    std::uint32_t count=0;
    for(std::uint32_t n=0; n!=vec.size(); ++n)
    {
        for(std::uint32_t m=n; m!=vec.size(); ++m) 
        {
            // Consider subseq = vec[n:m]
            std::uint32_t temp=0;
            for(std::uint32_t k=n; k!=m+1; ++k)
            {
                if (vec[k]%2==1) ++temp;
            }
            if (temp==num_of_odd) ++count;
        }
    }
    return count;
}
}

// ****************** //
// *** Question 3 *** //
// ****************** //
// Given vector of unsorted integers, count num of non-contiguous subsequence with size of 3, so that it is strictly decreasing : 
// i.e. find v[n0] > v[n1] > v[n2]
//      where  n0  <   n1  <   n2
// 
// * integers are unsorted
// * integers may duplicated
// * identical subsequences are counted as one
//
// Ideas :
// * no dynamic programming, subproblem does not help
// * no exhaustive search, it is O(N^3) 
// * do heuristic search
// * it is limited to subsequence with size of 3 (cannot be generalized to subsequences with any size)
// * make use of the info "3", i.e. finding n1 :
//   then look for bigger v[n0] on LHS
//   then look for bigger v[n2] on RHS
//
namespace interview {
std::uint32_t count_decreasing_triplet(const std::vector<std::uint32_t>& vec)
{
    std::uint32_t count=0;

    return count;
}
}

// ****************** //
// *** Question 4 *** //
// ****************** //
// Given string of 0&1, count num of non-contiguous substring, so that it is a step-function with edge in middle :
// * 01
// * 10
// * 0011
// * 1100
// * 000111
// * 111000
// * ... etc, identical substrings are counted as one
//
// Ideas :
// * no dynamic programming, subproblem does not help
// * no exhaustive search, it is more complicated
// * do heuristic search
// * if 000111 exists, then 0011 exists
// * if 111000 exists, then 1100 exists
// * do scan twice, once for counting longest 000...111...
//                  once for counting longest 111...000...
// * given a string, for element n
//   if num of 0 on LHS (including n) = x, then
//      num of 1 on LHS (including n) = n+1-x
//      num of 0 on RHS (excluding n) = total num of 0s - x
//      nun of 1 on RHS (excluding n) = total num of 1s - (n+1-x) 
//
namespace interview {
std::uint32_t count_step_funcion(std::string& str)
{
    // ************************************************* //
    // step 1 : find total num of 0s
    // step 2 : find num of 0s on LHS (including str[n])
    // ************************************************* //
    std::uint32_t total_num_of_0s = 0;
    std::uint32_t total_num_of_1s = 0;
    std::vector<std::uint32_t> vec_num_of_0s_LHS;
    for(const auto& c:str)
    {
        if (c=='0') ++total_num_of_0s;
        vec_num_of_0s_LHS.push_back(total_num_of_0s);
    }
    total_num_of_1s = str.size() - total_num_of_0s;

    // ********************************************** //
    // step 3 : find num of 0 in longest 000...111...
    //          find num of 0 in longest 111...000...
    // ********************************************** //
    std::uint32_t longest_01 = 0; 
    std::uint32_t longest_10 = 0; 
    for(std::uint32_t n=0; n!=str.size(); ++n)
    {
        std::uint32_t num_of_0s_LHS =     vec_num_of_0s_LHS[n];
        std::uint32_t num_of_1s_LHS = n+1-vec_num_of_0s_LHS[n];
        std::uint32_t num_of_0s_RHS = total_num_of_0s - num_of_0s_LHS;
        std::uint32_t num_of_1s_RHS = total_num_of_1s - num_of_1s_LHS;

        std::uint32_t temp_01 = std::min(num_of_0s_LHS, num_of_1s_RHS);
        std::uint32_t temp_10 = std::min(num_of_1s_LHS, num_of_0s_RHS);
        longest_01 = std::max(longest_01, temp_01);
        longest_10 = std::max(longest_10, temp_10);
    }
    return longest_01 + longest_10;    
}
}


// *************** //
// *** Testing *** //
// *************** //

namespace interview {
std::vector<std::uint32_t> random_int_vector(std::uint32_t size)
{
    std::vector<std::uint32_t> output;
    for(std::uint32_t n=0; n!=size; ++n)
    {
        output.push_back(rand()%100);
    }
    return output;
}

void test_alphagrep_count_k_odd_subseq()    
{
    std::cout << "\n\nAlphagrep : count k odd subseq";
    std::uint32_t num_error = 0;
    for(std::uint32_t n=0; n!=500; ++n)
    {
        std::uint32_t num_of_odd = 1 + rand()%10;
        auto vec = random_int_vector(40+rand()%40);
        std::uint32_t count0 = count_k_odd_subseq(vec, num_of_odd); 
        std::uint32_t count1 = count_k_odd_subseq_exhaustive(vec, num_of_odd); 
        if (count0 != count1) ++num_error;

        std::cout << "\ntest " << n 
                  << ", count0 = " << count0 
                  << ", count1 = " << count1 
                  << ", error = " << num_error << "/" << n+1;
    }
}

std::string random_binary_string(std::uint32_t size)
{
    std::string output;
    for(std::uint32_t n=0; n!=size; ++n)
    {
        if (rand()%2==0) output.push_back('0');
        else             output.push_back('1');
    }
    return output;
}

void test_alphagrep_count_step_function()
{
    std::cout << "\n\nAlphagrep : count step function";
    for(std::uint32_t n=0; n!=10; ++n)
    {
        auto str = random_binary_string(10+rand()%10);
        std::uint32_t count = count_step_funcion(str); 

        std::cout << "\ntest " << n 
                  << ", str = " << str 
                  << ", count = " << count;
    }
}
}
