#include<iostream>
#include<vector>
#include<02_dynprog.h>

std::uint64_t nCr(std::uint16_t n, std::uint16_t r)
{
    if (n==1) return 1;
    if (r==0) return 1; // LHS of Pascal triangle
    if (r==n) return 1; // RHS of Pascal triangle
    return nCr(n-1,r) + nCr(n-1,r-1);
}

std::uint64_t bell_number(std::uint16_t n)
{
    if (n==0) return 1;
    if (n==1) return 1;

    std::uint64_t ans = 0;
    for(std::uint16_t r=0; r<=n-1; ++r)
    {
        ans += nCr(n-1,r) * bell_number(n-1-r); // pick n to form a cluster with the latest item
    }
    return ans;
}

// *************************** //
// *** Boolean parenthesis *** //
// *************************** //
enum class logic : std::uint8_t 
{
    OR,
    XOR,
    AND
};

struct bool_logic
{
    bool value;
    logic op;
};

auto merge_two_subproblems(const logic& op, const auto& sub0, const auto& sub1)
{
    std::uint32_t num_true;
    std::uint32_t num_false;
    if (op == logic::OR)
    {
        num_true  = sub0.first * sub1.first + sub0.first * sub1.second + sub0.second * sub1.first; 
        num_false = sub0.second * sub1.second;
    }
    else if (op == logic::XOR)
    {
        num_true  = sub0.first * sub1.second + sub0.second * sub1.first; 
        num_false = sub0.first * sub1.first + sub0.second * sub1.second;
    }
    else
    {
        num_true  = sub0.first * sub1.first;
        num_false = sub0.first * sub1.second + sub0.second * sub1.first + sub0.second * sub1.second;
    }
    return std::make_pair(num_true, num_false);
}

// Expression is represented (ignores vec.back().op)
//
// = vec[0].value vec[0].op vec[1].value vec[1].op ... vec[n-2].value vec[n-2].op vec[n-1].value
//
std::pair<std::uint32_t, std::uint32_t> 
num_bool_parenthesis_recursive(const std::vector<bool_logic>::const_iterator& begin,
                               const std::vector<bool_logic>::const_iterator& end)
{
    if (begin+1==end)
    {
        if (begin->value) return {1,0};
        else              return {0,1};
    } 

    std::uint32_t num_true  = 0;
    std::uint32_t num_false = 0;
    for(auto iter=begin; iter!=end-1; ++iter)
    {
        auto sub0 = num_bool_parenthesis_recursive(begin, iter+1);
        auto sub1 = num_bool_parenthesis_recursive(iter+1, end);
        auto sub  = merge_two_subproblems(iter->op, sub0, sub1);
        num_true  += sub.first;
        num_false += sub.second;
    }
    return {num_true, num_false};
} 

std::pair<std::uint32_t, std::uint32_t> 
num_bool_parenthesis_iterative(const std::vector<bool_logic>::const_iterator& begin,
                               const std::vector<bool_logic>::const_iterator& end)
{
    std::uint32_t N = std::distance(begin, end);
    matrix<std::pair<std::uint32_t, std::uint32_t>> mat(N,N); // #true and #false

    std::uint32_t k=0; 
    for(auto iter=begin; iter!=end; ++iter, ++k) 
    {
        mat(k,k) = (iter->value? std::pair<std::uint32_t, std::uint32_t>{1,0}: 
                                 std::pair<std::uint32_t, std::uint32_t>{0,1});
    }

    for(std::uint32_t subdiag=1; subdiag!=N; ++subdiag)
    {
        for(std::uint32_t n=0; n!=N-subdiag; ++n)
        {
            std::uint32_t m=n+subdiag;
            mat(n,m).first  = 0;
            mat(n,m).second = 0;
            for(std::uint32_t k=0; k!=m; ++k)
            {
                auto sub = merge_two_subproblems((begin+k)->op, mat(n,k), mat(k+1,m));
                mat(n,m).first  += sub.first;
                mat(n,m).second += sub.second;
            }
        }
    }
    return mat(0,N-1);
}

// ***************** //
// *** Coin game *** //
// ***************** //
//
// f(n.m) = std::max(vec[n]+std::min(f(n+2,m),f(n+1,m-1),
//                   vec[m]+std::min(f(n+1,m-1),f(n,m-2))
//
// ************ //
// *** Test *** //
// ************ //
std::vector<bool_logic> gen_bool_logic_vector(std::uint32_t N)
{
    // vec.back().op is redundant
    std::vector<bool_logic> vec;

    for(std::uint32_t n=0; n!=N; ++n)
    {
        auto tmp = rand()%6;
        if      (tmp==0)   vec.push_back({true, logic::OR});
        else if (tmp==1)   vec.push_back({true, logic::XOR});
        else if (tmp==2)   vec.push_back({true, logic::AND});
        else if (tmp==3)   vec.push_back({false, logic::OR});
        else if (tmp==4)   vec.push_back({false, logic::XOR});
        else               vec.push_back({false, logic::AND});
    } 
    return vec;
}

void test_nCr()
{
    std::cout << "\nPascal triangle";
    for(std::uint16_t n=1; n!=10; ++n)
    {
        std::cout << "\n";
        for(std::uint16_t r=0; r<=n; ++r)
        {
            std::cout << nCr(n,r) << " ";
        }
    }
}

void test_bell_number()
{
    for(std::uint16_t n=0; n!=10; ++n)
    {
        std::cout << "\nbell number " << n << " : " << bell_number(n);
    }
}

void test_num_bool_parenthesis()
{
    for(std::uint32_t n=0; n!=100; ++n)
    {
        auto vec = gen_bool_logic_vector(rand()%15+2); 
        auto ans0 = num_bool_parenthesis_recursive(vec.begin(), vec.end());
        auto ans1 = num_bool_parenthesis_iterative(vec.begin(), vec.end());

        // iterative approach should be faster than recursive approach
        std::cout << "\ninput = [";
        for(const auto& x:vec) 
        {
            if (&x!=&vec.back())
            {
                std::cout << (x.value? "T":"F") << " " 
                          << (x.op==logic::OR? "or" :
                             (x.op==logic::XOR? "xor" : "and")) << " ";
            }
            else
            {
                std::cout << (x.value? "T":"F") << "]";
            }
        }
        std::cout << "\nans0  = " << ans0.first << " + " << ans0.second 
                  << "\nans1  = " << ans1.first << " + " << ans1.second; 
        if (ans0.first  != ans1.first ||
            ans0.second != ans1.second) std::cout << " [error]";
    }
}

void test_multi_link_subproblem()
{
//  test_nCr();
//  test_bell_number();
    test_num_bool_parenthesis();
}
