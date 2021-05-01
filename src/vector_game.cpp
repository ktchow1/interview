#include<iostream>
#include<vector>
#include<queue>
#include<stack>
#include<set>
#include<map>
#include<unordered_set>
#include<unordered_map>

// ******************************** //
// *** This file is not tested. *** //
// ******************************** //
// Vector problem A
//  1. max dist-sum 
//
// Vector problem B
//  2. max   profit                      : sub  / ans
//  3. count target profit               : hist / ans
//  4. max   subseq sum                  : sub  / ans
//  5. max   subseq product              : sub  / ans
//  6. max   non-contiguous subseq sum   : sub  / ans
//  7. count target subseq sum           : sum  / hist / ans
//  8. count target-divisible subseq sum : sum  / hist / ans
//  9. count target-less-than subseq sum : sum  / hist / ans
// 10. count longest target subseq sum   : sum  / hist / ans
//
// String problem & 2D problem
// 11. longest non-duplicated substr
// 12. longest palindrome substr
// 13. set of subset
// 14. nearest target pair-sum
// 15. longest non-contiguous common subseq
//
// Tree, sort-variant, stack-trick
// 16. tree layer average
// 17. order statistic   -  quick sort variant
// 18. number of bribes  - bubble sort variant
// 19. minimum unsorted part
// 20. biggest rectangle
// 21. biggest muddy puddle

template<typename ITER>
auto Q02_max_profit(ITER begin, ITER end)
{
    auto sub = *(begin+1) - *begin;
    auto ans = sub;

    for(auto iter=begin+2; iter!=end; ++iter)    
    {
        sub = std::max(sub + *iter - *(iter-1), *iter - *(iter-1));
        ans = std::max(ans, sub);
    } 
    return ans;
}

template<typename ITER>
auto Q03_count_target_profit(ITER begin, ITER end, const typename std::iterator_traits<ITER>::value_type& target)
{
    using T = typename std::iterator_traits<ITER>::value_type;
    std::unordered_map<T, int> hist; hist[*begin] = 1;
    std::uint32_t ans = 0;

    for(auto iter=begin+1; iter!=end; ++iter)    
    {
        // *** Update ans *** //
        if (auto tmp = hist.find(*iter - target); tmp!=hist.end())
        {
            ans += tmp->second;
        }

        // *** Update hist *** //
        if (auto tmp = hist.find(*iter); tmp!=hist.end())
        {
            ++tmp->second;
        } 
        else
        {
            hist[*iter] = 1;
        }
    } 
    return ans;
}

template<typename ITER>
auto Q04_max_subseq_sum(ITER begin, ITER end)
{
    auto sub = *begin;
    auto ans = sub;
    for(auto iter=begin+1; iter!=end; ++iter)    
    {
        sub = std::max(sub + *iter, *iter);
        ans = std::max(ans, sub);
    } 
    return ans;
}

template<typename ITER>
auto Q05_max_subseq_product(ITER begin, ITER end)
{
    auto sub0 = *begin;
    auto sub1 = *begin;
    auto ans  = sub0;
    for(auto iter=begin+1; iter!=end; ++iter)    
    {
        sub0 = std::max(std::max(sub0 * *iter, sub1 * *iter), *iter);
        sub1 = std::min(std::min(sub0 * *iter, sub1 * *iter), *iter);
        ans  = std::max(ans, sub0);
    } 
    return ans;
}

template<typename ITER>
auto Q06_max_non_contig_subseq_sum(ITER begin, ITER end)
{
    auto sub0 = *begin; // subproblem with constraint : must include element[n]
    auto sub1 = 0;      // subproblem with constraint : must exclude element[n]
    auto ans  = std::max(sub0, sub1);
    for(auto iter=begin+1; iter!=end; ++iter)    
    {
        sub0 = std::max(std::max(sub0 + *iter, sub1 + *iter), *iter);
        sub1 = std::max(std::max(sub0, sub1), 0);
        ans  = std::max(ans, std::max(sub0, sub1));
    } 
    return ans;
}


template<typename ITER>
auto Q07_count_target_subseq_sum(ITER begin, ITER end, const typename std::iterator_traits<ITER>::value_type& target)
{
    using T = typename std::iterator_traits<ITER>::value_type;
    std::unordered_map<T, int> hist; hist[0] = 1; // Don't forget this.
    std::uint32_t ans = 0;

    T sum = 0;
    for(auto iter=begin; iter!=end; ++iter)    
    {
        // *** Update sum *** //
        sum += *iter;

        // *** Update ans *** //
        if (auto tmp = hist.find(sum - target); tmp!=hist.end())
        {
            ans += tmp->second;
        }

        // *** Update hist *** //
        if (auto tmp = hist.find(sum); tmp!=hist.end())
        {
            ++tmp->second;
        } 
        else
        {
            hist[sum] = 1;
        }
    } 
    return ans;
}

template<typename ITER>
auto Q08_count_target_divisible_subseq_sum(ITER begin, ITER end, const typename std::iterator_traits<ITER>::value_type& target)
{
    using T = typename std::iterator_traits<ITER>::value_type;
    std::unordered_map<T, int> hist; hist[0] = 1; // Don't forget this.
    std::uint32_t ans = 0;

    T sum = 0;
    for(auto iter=begin; iter!=end; ++iter)    
    {
        // *** Update sum *** //
        sum += *iter;
        sum = sum % target;

        // *** Update ans  *** //
        // *** Update hist *** //
        if (auto tmp = hist.find(sum); tmp!=hist.end())
        {
            ans += tmp->second;
            ++tmp->second;
        } 
        else
        {
            hist[sum] = 1;
        }
    } 
    return ans;
}

// ***************************************************** //
// For this question and the next, 
// replace std::unordered_map of count (i.e. histogram) 
//    with std::unordered_map of index
// ***************************************************** //
template<typename ITER>
auto Q09_longest_target_subseq_sum(ITER begin, ITER end, const typename std::iterator_traits<ITER>::value_type& target)
{
    using T = typename std::iterator_traits<ITER>::value_type;
    std::unordered_map<T, ITER> idx; idx[0] = begin; // index is the "end" of first subseq having specific sum
    std::uint32_t ans = 0;

    T sum = 0;
    for(auto iter=begin; iter!=end; ++iter)    
    {
        // *** Update sum *** //
        sum += *iter;

        // *** Update ans *** //
        if (auto tmp = idx.find(sum - target); tmp!=idx.end())
        {
            ans = std::max(std::distance(tmp->second, iter+1), ans);
        }

        // *** Update idx *** //
        if (auto tmp = idx.find(sum); tmp==idx.end())
        {
            idx[sum] = iter+1;
        }
    } 
    return ans;
}

// ************************************************** //
// 1. For positive number only
// 2. For lower bound search of key instead of 
//        exact match search of key, hence 
//        use std::map instead of std::unordered_map.
// ************************************************** //
auto Q10_count_lt_target_subseq_sum_of_pos_numbers(std::vector<uint32_t>::iterator begin, 
                                                   std::vector<uint32_t>::iterator end,
                                                   std::uint32_t target)
{
    using ITER = std::vector<std::uint32_t>::iterator;
    std::map<std::uint32_t, ITER> idx; idx[0] = begin; // index is the "end" of subseq starting with element 0 having specific sum
    std::uint32_t ans = 0;

    std::uint32_t sum = 0;
    for(auto iter=begin; iter!=end; ++iter)    
    {
        // *** Update sum *** //
        sum += *iter;

        // *** Update ans *** //
        if (auto tmp = idx.lower_bound(sum - target); tmp!=idx.end())
        {
            ans += std::distance(tmp->second, iter+1);
        }

        // *** Update idx *** //
        if (auto tmp = idx.find(sum); tmp==idx.end())
        {
            idx[sum] = iter+1;
        }
    } 
    return ans;
}

auto Q11_longest_non_duplicated_string(std::string::iterator begin,
                                       std::string::iterator end)
{
    std::unordered_map<char, std::string::iterator> idx;
    std::uint32_t sub = 0;
    std::uint32_t ans = 0;

    for(auto iter=begin; iter!=end; ++iter)
    {
        // *** Update sub & ans *** //
        if (auto tmp = idx.find(*iter); tmp!=idx.end())
        {
            sub = std::distance(tmp->second, iter+1); 
        }
        else
        {
            sub = sub+1;
        }
        ans = std::max(ans, sub);

        // *** Update idx *** //
        if (auto tmp = idx.find(*iter); tmp==idx.end()) // This is duplicated for readability.
        {
            idx[*iter] = iter+1;
        }
    }
    return ans;
}

// This method cannot solve cycle cases, like ABCBABCBA.
auto Q12_longest_palindrome_failed(std::string::iterator begin, std::string::iterator end)
{
    std::uint32_t sub0 = 0; // even palindrome : ABCCBA
    std::uint32_t sub1 = 1; //  odd palindrome : ABCBA
    std::uint32_t ans  = 1;

    // *** Update even *** //
    if (std::distance(begin, end) >= 2)
    {
        for(auto iter=begin+1; iter!=end; ++iter) 
        {
            if (*iter == *(iter-sub0-1)) 
            {
                sub0 = sub0 + 2;
            }
            else
            {
                sub0 = 0;
            }
            ans = std::max(ans, sub0);
        }
    }

    // *** Update odd *** //
    if (std::distance(begin, end) >= 3)
    {
        for(auto iter=begin+2; iter!=end; ++iter) 
        {
            if (*iter == *(iter-sub1-1))
            {
                sub1 = sub1 + 2;
            }
            else
            {
                sub1 = 1;
            }
            ans = std::max(ans, sub1);
        }
    }
    return ans;
}

// This method can solve cycle cases, like ABCBABCBA.
auto Q12_longest_palindrome(std::string::iterator begin, std::string::iterator end)
{
    std::uint32_t sub0 = 0; // even palindrome : ABCCBA (space between 2Cs is the centre)
    std::uint32_t sub1 = 1; //  odd palindrome : ABCBA  (char C is the centre)
    std::uint32_t ans  = 1;

    // *** Update even *** //
    if (std::distance(begin, end) >= 2)
    {
        for(auto iter=begin+1; iter!=end; ++iter) 
        {
            if (sub0 > 0 && *iter == *(iter-sub0-1)) 
            {
                sub0 = sub0 + 2;
            }
            else
            {
                sub0 = 0;
                if (*iter == *(iter-sub0-1))
                {
                    sub0 = sub0 + 2; // for handling cyclic case
                }
                else
                {
                    sub0 = 0;
                }
            }
            ans = std::max(ans, sub0);
        }
    }

    // *** Update odd *** //
    if (std::distance(begin, end) >= 3)
    {
        for(auto iter=begin+2; iter!=end; ++iter) 
        {
            if (sub1 > 1 && *iter == *(iter-sub1-1))
            {
                sub1 = sub1 + 2;
            }
            else 
            {
                sub1 = 1;
                if (*iter == *(iter-sub1-1))
                {
                    sub1 = sub1 + 2; // for handling cyclic case
                }
                else
                {
                    sub1 = 1;
                }
            }
            ans = std::max(ans, sub1);
        }
    }
    return ans;
}

auto Q13_set_of_subset(const std::string& str, std::set<std::string>& output)
{
    if (str.size()==0) return;
    
    std::set<std::string> temp;
    Q13_set_of_subset(str.substr(1), temp);
    for(const auto& x:temp)
    {
        std::string y(1, str[0]);
        y.append(x);

        output.insert(x);
        output.insert(y);
    }
    output.insert(std::string(1, str[0]));
}

auto Q14_nearest_target_pair_sum(const std::vector<std::uint32_t>& vx, // suppose to be sorted
                                 const std::vector<std::uint32_t>& vy, // suppose to be sorted
                                 std::uint32_t target)
{
    std::uint32_t x = vx.size()-1; 
    std::uint32_t y = 0;          
    std::uint32_t win_x = x;
    std::uint32_t win_y = y;
    std::uint32_t min_diff = abs(vx[x] + vy[y] - target);
    while(true)
    {
        auto sum = vx[x] + vy[x];

        // step 1 : minimization
        if (auto tmp = abs(sum - target); tmp < min_diff)
        {
            min_diff = tmp;
            win_x = x;
            win_y = y; 
        }

        // step 2: netvigation
        if (vx[x] + vy[y] < target) --x;
        else ++y;
    }
    return std::make_pair(vx[win_x], vy[win_y]);
}

template<typename T> struct node
{
    T value;
    node<T>* lhs;
    node<T>* rhs;
};

void Q16_tree_layer_average(const node<std::uint32_t>* root, std::vector<std::pair<std::uint32_t, std::uint32_t>>& result)
{
    if (!root) return;

    std::queue<std::pair<std::uint32_t, const node<std::uint32_t>*>> q; q.push(std::make_pair(0UL, root));
    while(!q.empty())
    {
        auto [layer, this_node] = q.front(); q.pop();
        while(layer >= result.size()) result.push_back(std::make_pair(0,0));
        result[layer].first += this_node->value;
        result[layer].second += 1;
        
        if (this_node->lhs) q.push(std::make_pair(layer+1, this_node->lhs));
        if (this_node->rhs) q.push(std::make_pair(layer+1, this_node->rhs));
    }
}

template<typename ITER>
auto Q17_order_statistics(ITER begin, ITER end, std::uint32_t k)
{
    auto last = end-1;
    while(begin!=last)
    {
        // step 1 : quick-sort-iteration
        auto i = begin;
        auto j = last;
        while(i!=j)
        {
            if (*i <= *j) --j;
            else
            {
                auto temp = *i;
                *i = *(i+1);
                *(i+1) = *j;
                *j = temp;
                ++i;
            }
        }
    
        // step 2 : bisection
        if (std::distance(begin, i)==k)
        {
            return *i;
        }
        else if (std::distance(begin,i) < k)
        {   
            begin = i+1;
        }
        else 
        {
            last = i-1;
        }
    }
    return *begin;
}

// ******************* //
// *** Slow method *** //
// ******************* //
// n      is old position 
// vec[n] is new position
// ********************** //
std::uint32_t Q18_num_of_bribes_slow(const std::vector<std::uint32_t>& vec)
{
    std::uint32_t num_bribes = 0;
    for(std::uint32_t n=0; n!=vec.size(); ++n)
    {
        for(std::uint32_t m=n+1; m!=vec.size(); ++m) // m (originally before n) falls behind n
        {
            if (vec[m] < vec[n]) ++num_bribes; // original order
        }
    }
    return num_bribes;
}

// **************************************************** //
// *** Fast method (reduce the range of inner loop) *** //
// **************************************************** //
std::uint32_t Q18_num_of_bribes_fast(const std::vector<std::uint32_t>& vec)
{
    std::uint32_t num_bribes = 0;
    for(std::uint32_t n=0; n!=vec.size(); ++n)
    {
        for(std::uint32_t m=vec[n]-2; m!=n; ++m) // m (originally after n) overtakes n 
        {
            if (vec[m] > vec[n]) ++num_bribes; // original order
        }
    }
    return num_bribes;
}

// ************************ //
// *** Store y in stack *** //
// ************************ //
std::uint32_t Q19_num_of_sorted_element(const std::vector<std::uint32_t>& vec)
{
    // *** LHS *** //
    std::stack<std::uint32_t> s0;
    std::uint32_t lhs_min = vec.size(); 
    for(std::uint32_t n=0; n!=vec.size(); ++n)
    {
        // correct order 
        if (s0.empty() || vec[n] >= s0.top())
        {
            s0.push(vec[n]);
        }
        // incorrect order
        else
        {
            while(!s0.empty() && vec[n] < s0.top()) s0.pop();
            if (lhs_min > s0.size()) 
                lhs_min = s0.size();
        }
    }

    // *** RHS *** //
    std::stack<std::uint32_t> s1;
    std::uint32_t rhs_min = vec.size(); 
    for(std::uint32_t n=0; n!=vec.size(); ++n)
    {
        // correct order 
        if (s1.empty() || vec[vec.size()-n-1] <= s1.top())
        {
            s1.push(vec[vec.size()-n-1]);
        }
        // incorrect order
        else
        {
            while(!s1.empty() && vec[vec.size()-n-1] > s1.top()) s1.pop();
            if (rhs_min > s1.size()) 
                rhs_min = s1.size();
        }
    }

    return lhs_min + rhs_min;
}

// ******************************* //
// *** Store both x&y in stack *** //
// ******************************* //
// Same pattern in Q19 & Q20 :
// for()
// {
//     if () {           }
//     else  { while ... }
// }
// ******************************* //
std::uint32_t Q20_biggest_rectangle(const std::vector<std::uint32_t>& vec)
{
    std::stack<std::pair<std::uint32_t, std::uint32_t>> s;
    std::uint32_t sub;
    std::uint32_t ans = 0;

    for(std::uint32_t n=0; n!=vec.size(); ++n)
    {
        if (s.empty() || vec[n] > s.top().second) // no equality here ...
        {
            s.push(std::make_pair(n,vec[n]));
            sub = vec[n];
        }
        else
        {
            while(!s.empty() && vec[n] < s.top().second) s.pop(); // no equality here too ... 
            if (s.empty())
            {
                sub = vec[n];
            }
            else
            {
                // The following are equivalent.
                sub =         s.top().second          * (n-s.top().first+1); 
            //  sub = std:min(s.top().second, vec[n]) * (n-s.top().first+1);
            }
        }
        ans = std::max(ans, sub);
    }
    return ans;
}

std::uint32_t Q21_muddy_puddle(const std::vector<std::uint32_t>& vec)
{
    std::vector<std::uint32_t> lhs_profile;
    std::vector<std::uint32_t> rhs_profile;

    // *** LHS profile *** //
    std::uint32_t max = std::numeric_limits<std::uint32_t>::min();
    for(std::uint32_t n=0; n!=vec.size(); ++n)
    {
        if (max < vec[n])
            max = vec[n];
        lhs_profile.push_back(max);
    }

    // *** RHS profile *** //
    max = std::numeric_limits<std::uint32_t>::min();
    for(std::uint32_t n=0; n!=vec.size(); ++n)
    {
        if (max < vec[vec.size()-n-1])
            max = vec[vec.size()-n-1];
        rhs_profile.push_back(max);
    }

    // *** Total volume *** //
    std::uint32_t vol = 0;
    for(std::uint32_t n=0; n!=vec.size(); ++n)
    {
        auto temp = std::min(lhs_profile[n], rhs_profile[n]);
        if (temp > vec[n]) vol += temp - vec[n];
    }
    return vol;
}


