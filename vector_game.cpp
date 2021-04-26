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
    std::map<T, ITER> idx; idx[0] = begin; // index is the "end" of subseq starting with element 0 having specific sum
    std::uint32_t ans = 0;

    T sum = 0;
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
auto Q12_longest_palindrome(std::string::iterator begin,
                            std::string::iterator end)
{
    std::uint32_t sub0 = 1; //  odd palindrome : ABCBA
    std::uint32_t sub1 = 0; // even palindrome : ABCCBA
    std::uint32_t ans  = 1;

    for(auto iter=begin+1; iter!=end; ++iter) // start with begin+1, avoid bound-checking in line A&B
    {
        // *** Update odd *** //
        if (*iter == *(iter-sub0-1)) // line A
        {
            sub0 = sub0 + 2;
        }
        else
        {
            sub0 = 1;
        }

        // *** Update even *** //
        if (*iter == *(iter-sub1-1)) // line B
        {
            sub1 = sub1 + 2;
        }
        else
        {
            sub1 = 0;
        }

        ans = std::max(ans, std::max(sub0, sub1));
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
        output.insert(x);
        x.insert(0,str[0]);
        output.insert(x);
    }
    output.insert(std::string(1. str[0]));
}

auto Q14_nearest_target_pair_sum(const std::vector<std::uint32_t>& vx, // suppose to be sorted
                                 const std::vector<std::uint32_t>& vy, // suppose to be sorted
                                 std::uint32_t target)
{
    std::uint32_t x = s0.size()-1; 
    std::uint32_t y = 0;          
    std::uint32_t win_x = x;
    std::uint32_t win_y = y;
    std::uint32_t min_diff = abs(vx[x] + vy[y] - target);
    while(true)
    {
        auto sum = vx[x] + vy[x];

        // step 1 : minimization
        if (auto tmp = abs(sum - targe); tmp < min_diff)
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

void Q16_tree_layer_average(const node<T>* root, std::vector<std::pair<T, std::uint32_t>>& result)
{
    if (!root) return;

    std::queue<std::pair<std::uint32_t, const node<T>*>> q; q.push(std::make_pair(0UL, root));
    while(!q.empty())
    {
        auto [layer, this_node] = q.top(); q.pop();
        while(layer >= result.size()) result.push_back(std::make_pair(0,0));
        result[layer].first += this_node->value;
        result[layer].second += 1;
        
        if (this_node->lhs) q.push(std::make_pair(layer+1, this_node->lhs));
        if (this_node->rhs) q.push(std::make_pair(layer+1, this_node->rhs));
    }
}

template<typename ITER>
auto Q17_order_statistics(ITER begin, ITER end, std::uint32_t K)
{
    auto front = begin;
    auto last = end-1;

    while(front!=last)
    {
        // step 1 : quick-sort-iteration
    
    
        // step 2 : bisection
    }
    return *front;
}

template<typename ITER>
std::uint32_t Q18_num_of_bribes(ITER begin, ITER end)
{

}

template<typename ITER>
std::uint32_t Q19_num_of_sorted_element(ITER begin, ITER end)
{
}

template<typename ITER>
auto Q20_biggest_rectangle(ITER begin, ITER end)
{
}

template<typename ITER>
auto Q21_muddy_puddle(ITER begin, ITER end)
{
}
