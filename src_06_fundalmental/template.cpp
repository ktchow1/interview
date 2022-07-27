#include<iostream>
#include<vector>
#include<string>
#include<tuple>


// Template class 
// 
// When used as type-type mapping, template struct is like a multivariate function.

template<typename T0, typename T1, typename T2> // <--- line 1
struct fct
{
    using output_type = std::tuple<T0,T1,T2>;
};

// We cam define specialization like below, make sure specialization is a subset of generic template, 
// that is, the feasible-set defined by line 1 is a superset of the feasible-set defined by line 3,
// however, the feasible-set defined by line 2 means something different, it refers to other types 
// that the specialization depends on, hence #arg in line 2 may be more than #arg in line 1.
//

template<typename U0, typename U1, typename U2, typename U3>     // <--- line 2
struct fct<std::pair<U0,U1>, std::pair<U1,U2>, std::pair<U2,U3>> // <--- line 3
{
    using output_type = std::tuple<U0,U1,U2,U3>;
}; 

void test()
{
    using Y0 = fct<std::uint32_t, std::string, std::vector<std::uint32_t>>::output_type;
    static_assert(std::is_same_v<Y0, std::tuple<std::uint32_t, 
                                                std::string, 
                                                std::vector<std::uint32_t>>>, "type mapping incorrect");

    using Y1 = fct<std::pair<char,          std::uint32_t>,
                   std::pair<std::uint32_t, std::string>, 
                   std::pair<std::string,   std::vector<std::uint32_t>>>::output_type;
    static_assert(std::is_same_v<Y1, std::tuple<char, 
                                                std::uint32_t, 
                                                std::string, 
                                                std::vector<std::uint32_t>>>, "type mapping incorrect");

    using Y2 = fct<std::pair<char,          std::uint32_t>,
                   std::pair<std::string,   std::string>, 
                   std::pair<std::string,   std::vector<std::uint32_t>>>::output_type;
    static_assert(std::is_same_v<Y2, std::tuple<std::pair<char,        std::uint32_t>,
                                                std::pair<std::string, std::string>, 
                                                std::pair<std::string, std::vector<std::uint32_t>>>>, "type mapping incorrect");
}

