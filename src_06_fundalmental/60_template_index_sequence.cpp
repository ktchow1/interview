#include<iostream>
#include<tuple>

// Conclusion
// 1. In template struct declaration :
// -  do not put parameter after parameter-pack
// -  do not put fomrula, like N-1, or sizeof...(Ns)
// 2. In template struct body : 
// -  we can put parameter after parameter-pack
// -  we can put formula, like N-1, or sizeof<Plug>_...(Ns)
// 3. As the pack expands, the tail remains unchanged, see the illustrative comment below.


template<std::uint32_t ...Ns> struct index_sequence 
{
};

// Objective :
// index_sequence_generator<4>::type ---> index_sequence<0,1,2,3>
//
// Approach 1 works, it supposes to do :
// (1) index_sequence_generator<4>::type = index_sequence_generator<3,3> ::type
//                                       = index_sequence_generator<2,2,3> ::type
//                                       = index_sequence_generator<1,1,2,3> ::type
//                                       = index_sequence_generator<0,0,1,2,3> ::type <--- recursion
//                                       = index_sequence<0,1,2,3>                    <--- boundary condition (specialization)

template<std::uint32_t N, std::uint32_t ...Ns> 
struct index_sequence_generator
{
    using type = typename index_sequence_generator<N-1,N-1,Ns...>::type;
};

template<std::uint32_t ...Ns> 
struct index_sequence_generator<0,Ns...>
{
    using type = index_sequence<Ns...>;          
};

// Approach 2 fails, it supposes to do :
// (2) index_sequence_generator_failed<4>::type = index_sequence_generator_failed<4,0> ::type
//                                              = index_sequence_generator_failed<4,0,1> ::type
//                                              = index_sequence_generator_failed<4,0,1,2> ::type
//                                              = index_sequence_generator_failed<4,0,1,2,3> ::type
//                                              = index_sequence_generator_failed<4,0,1,2,3,4> ::type
//                                              = index_sequence<0,1,2,3>       
/* 

template<std::uint32_t N, std::uint32_t ...Ns> 
struct index_sequence_generator_failed
{
    using type = typename index_sequence_generator_failed<N,Ns...,sizeof...(Ns)>::type; // <--- compile OK
};

template<std::uint32_t N, std::uint32_t ...Ns> 
struct index_sequence_generator_failed<N,Ns...,N> // <--- compile error here
{
    using type = index_sequence<Ns...>; 
};  

*/
// Objective :
// index_sequence_generator<4>::type ---> index_sequence<0,1,2,3>
//
// Approach 1 works, it supposes to do :
// (1) inv_index_sequence_generator<4>::type = inv_index_sequence_generator<4,0> ::type
//                                           = inv_index_sequence_generator<4,1,0> ::type
//                                           = inv_index_sequence_generator<4,2,1,0> ::type
//                                           = inv_index_sequence_generator<4,3,2,1,0> ::type 
//                                           = inv_index_sequence_generator<4,4,3,2,1,0> ::type 
//                                           = inv_index_sequence<3,2,1,0>

template<std::uint32_t N, std::uint32_t ...Ns> 
struct inv_index_sequence_generator
{
    using type = typename inv_index_sequence_generator<N, sizeof...(Ns), Ns...>::type;
};

template<std::uint32_t N, std::uint32_t ...Ns>
struct inv_index_sequence_generator<N,N,Ns...>
{
    using type = index_sequence<Ns...>;    
}; 

// Approach 2 fails, it supposes to do :
// (2) inv_index_sequence_generator<4>::type = inv_index_sequence_generator<4,0> ::type
//                                           = inv_index_sequence_generator<4,1,0> ::type
//                                           = inv_index_sequence_generator<4,2,1,0> ::type
//                                           = inv_index_sequence_generator<4,3,2,1,0> ::type
//                                           = inv_index_sequence<3,2,1,0>  
/*

template<std::uint32_t N, std::uint32_t ...Ns> 
struct inv_index_sequence_generator
{
    using type = typename inv_index_sequence_generator<N, sizeof...(Ns), Ns...>::type;
};

template<std::uint32_t N, std::uint32_t ...Ns>
struct inv_index_sequence_generator<N,N-1,Ns...> // <--- in here : do not put formula, do not put oher para after para-pack
{
    using type = index_sequence<Ns...>;          // <--- in here : we can put formula, we can put oher para after para-pack
};  

*/
// ************ //
// *** Test *** //
// ************ //
void test_index_sequence()
{
    static_assert(std::is_same_v<    index_sequence_generator<5>::type, index_sequence<0,1,2,3,4>>, "failed to generate correct index_sequence");
//  static_assert(std::is_same_v<    index_sequence_generator<5>::type, index_sequence<4,3,2,1,0>>, "failed to generate correct index_sequence");
    static_assert(std::is_same_v<inv_index_sequence_generator<5>::type, index_sequence<4,3,2,1,0>>, "failed to generate correct index_sequence");
//  static_assert(std::is_same_v<inv_index_sequence_generator<5>::type, index_sequence<0,1,2,3,4>>, "failed to generate correct index_sequence");

}

void test_template_index_sequence()
{
    test_index_sequence();
}
