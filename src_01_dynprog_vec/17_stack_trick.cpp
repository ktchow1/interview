#include<iostream>
#include<vector>
#include<stack>
#include<optional>

std::uint32_t length_of_minimum_sorting(const std::vector<std::uint32_t>& vec)
{
    // *** Forward *** //
    bool push = true;

    std::stack<std::uint32_t> s0;
    for(std::uint32_t n=0; n!=vec.size(); ++n)
    {
        while(!s0.empty() && vec[n] < s0.top()) 
        {
            push = false;
            s0.pop(); 
        }
        if (push) 
        {
            s0.push(vec[n]);
        }
    }

    // *** Backward *** //
    push = true;

    std::stack<std::uint32_t> s1;
    for(std::uint32_t n=0; n!=vec.size(); ++n)
    {
        while(!s1.empty() && vec[vec.size()-1-n] > s1.top())
        {
            push = false;
            s1.pop();
        }
        if (push)
        {
            s1.push(vec[vec.size()-1-n]);
        }
    }
    return vec.size()-s0.size()-s1.size();
}

/*
    Consider hist having one or more local maxima
    if hist[n] is on the rising edge of hist and
    if hist[m] is on the falling edge of hist and, where n<m
    if hist[n] == hist[m] <= hist[k] for all k in [n,m], then :
    then subproblem[n] should not be continued beyond m
    since they are already covered by subproblem[n-1] or subproblem[n-2] or ...

    i.e.    max_rect_in_hist2([1,3,4,2,5,3,1])
    == max( max_rect_in_hist2([0,3,4,0,0,0,0]),
            max_rect_in_hist2([1,2,2,2,5,3,1]) )
    == max( max_rect_in_hist2([0,3,4,0,0,0,0]),
            max_rect_in_hist2([0,0,0,0,5,3,0]),
            max_rect_in_hist2([1,2,2,2,3,3,1]) ) 
    == max( max_rect_in_hist2([0,3,4,0,0,0,0]),
            max_rect_in_hist2([0,0,0,0,5,3,0]),
            max_rect_in_hist2([0,2,2,2,3,3,0]),
            max_rect_in_hist2([1,1,1,1,1,1,1]) ) 

    Thus we use a stack to collect a set of ongoing subproblems in LIFO manner. Why LIFO?
    Since subproblem[n] must be push-in latter than subproblem[n-1]
    while subproblem[n] must be pop-out eariler than subproblem[n-1]
    calculation of rectangle area is done whenever a subproblem is popped only.

    For example, given a histogram like LHS, the processing order is in RHS :

      xx            11
     xxx  xx       222  44
     xxxx xxx      3333 555
     xxxxxxxx      66666666
    xxxxxxxxxxx   77777777777    which is basically a stack
*/
std::uint32_t biggest_rect_in_histogram(const std::vector<std::uint32_t>& vec)
{
    std::uint32_t ans=0;

    std::stack<std::pair<std::uint32_t, std::uint32_t>> s; // height and location
    for(std::uint32_t n=0; n!=vec.size(); ++n)             // n is the RHS boundary of RECT
    {
        // *** Part1 : Remove local maximum *** //
        std::optional<std::uint32_t> popped_index = std::nullopt;
        while(!s.empty() && vec[n] <= s.top().first) // BUG : Don't use <, use <=
        {
            {
                auto area = ((n-1) - (s.top().second-1)) * s.top().first;
                ans = std::max(ans, area);
            }
            popped_index = std::make_optional(s.top().second);
            s.pop();
        }
        if (popped_index)
        {
            s.push(std::make_pair(vec[n], *popped_index));
            s.push(std::make_pair(vec[n], n));
        }
        else
        {
            s.push(std::make_pair(vec[n], n));
        }
    }

    // *** Part2 : Consider the increasing part *** //
    while(!s.empty())
    {
        std::uint32_t n=vec.size()-1;
        {
            auto area = (n - (s.top().second-1)) * s.top().first;
            ans = std::max(ans, area);
        }
        s.pop();
    }
    return ans;
}

// ****************************** //
// *** Benchmarking algorithm *** //
// ****************************** //
std::uint32_t length_of_minimum_sorting_exhaustive(const std::vector<std::uint32_t>& vec)
{
    std::vector<std::uint32_t> vec0 = vec;
    std::sort(vec0.begin(), vec0.end());

    std::uint32_t N0;
    for(std::uint32_t n=0; n!=vec.size(); ++n)
    {
        if (vec[n]!=vec0[n]) { N0 = n; break; }
    }
    std::uint32_t N1;
    for(std::uint32_t n=0; n!=vec.size(); ++n)
    {
        if (vec[vec.size()-1-n]!=vec0[vec0.size()-1-n]) { N1 = n; break; }
    }
    return vec.size()-N0-N1;
}

// Exhaustive search for biggest rect[m,n] in 2D loop :
// n = index of RHS boundary of rect
// m = index of LHS boundary of rect, where m < n
//
std::uint32_t biggest_rect_in_histogram_exhaustive(const std::vector<std::uint32_t>& vec)
{
    std::uint32_t min_height;
    std::uint32_t max_area = 0;
    for(std::uint32_t n=0; n!=vec.size(); ++n)
    {
        min_height = vec[n];
        max_area   = std::max(max_area, vec[n]); // BUG : Don't write max_area = vec[n]
        for(std::int32_t m=static_cast<std::int32_t>(n)-1; m>=0; --m)
        {
            min_height = std::min(min_height, vec[m]);
            max_area   = std::max(max_area, (n-m+1)*min_height);
        }
    }
    return max_area;
}

// ************ //
// *** Test *** //
// ************ //
std::vector<std::uint32_t> gen_random_vector_for_sorting(std::uint32_t length)
{
    std::vector<std::uint32_t> ans;
    for(std::uint32_t n=0; n!=length; ++n)
    {
        ans.push_back(n);
    }

    std::uint32_t N0 = rand() % (std::uint32_t)(length*0.2) + (std::uint32_t)(length*0.05);
    std::uint32_t N1 = rand() % (std::uint32_t)(length*0.2) + (std::uint32_t)(length*0.05);
    for(std::uint32_t n=0; n!=length; ++n)
    {
        std::uint32_t n0 = N0+rand()%(length-N1-N0);
        std::uint32_t n1 = N0+rand()%(length-N1-N0);
        std::swap(ans[n0], ans[n1]);
    }
    return ans;
}

std::vector<std::uint32_t> gen_random_vector_for_histogram(std::uint32_t length)
{
    std::vector<std::uint32_t> ans;

    std::int32_t height = 12;
    for(std::uint32_t n=0; n!=length; ++n)
    {
        ans.push_back(height);
        height += static_cast<std::int32_t>(rand()%5)-2;
        if (height < 0) height = 0;
    }
    return ans;
}

void test_length_of_minimmum_sorting()
{
    for(std::uint32_t n=0; n!=100; ++n)
    {
        auto vec = gen_random_vector_for_sorting(80);
        auto ans0 = length_of_minimum_sorting_exhaustive(vec);
        auto ans1 = length_of_minimum_sorting(vec);

        std::cout << "\n" << ans0 << ", " << ans1 << ", vec = ";
        for(const auto& x:vec) std::cout << x << " ";
        if (ans0 != ans1) std::cout << " [ERROR]";
    }
}

void test_biggest_rect_in_histogram()
{
    for(std::uint32_t n=0; n!=300; ++n)
    {
        auto vec = gen_random_vector_for_histogram(100);
        auto ans0 = biggest_rect_in_histogram_exhaustive(vec);
        auto ans1 = biggest_rect_in_histogram(vec);

        std::cout << "\n" << ans0 << ", " << ans1 << ", vec = ";
        for(const auto& x:vec) std::cout << x << " ";
        if (ans0 != ans1) std::cout << " [ERROR]";
    }
}

void test_stack_trick()
{
//  test_length_of_minimmum_sorting();
    test_biggest_rect_in_histogram();
}
