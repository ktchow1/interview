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

// ****************************************************************************************************************************** //
// What does this do? 
//
// template<std::uint32_t N, std::uint32_t ...Ns> 
// struct idx_seq_gen { using type = typename index_sequence_generator<N-A,N-B,Ns...>::type; };
//
// idx_seq_gen<N>::type = idx_seq_gen<N- A, N-B>::type
//                      = idx_seq_gen<N-2A, N-B- A, N-B>::type
//                      = idx_seq_gen<N-3A, N-B-2A, N-B- A, N-B>::type
//                      = idx_seq_gen<N-4A, N-B-3A, N-B-2A, N-B- A, N-B>::type
//                      = idx_seq_gen<N-5A, N-B-4A, N-B-3A, N-B-2A, N-B-A, N-B>::type
//                      = idx_seq          <N-B-4A, N-B-3A, N-B-2A, N-B-A, N-B>  by setting constraint on first template parameter
//
// A defines delta
// B defines offset from N
// ****************************************************************************************************************************** //
// Lets construct an odd / even index sequence.

template<std::uint32_t N, std::uint32_t... Ns>
struct alt_index_sequence_generator
{
    using type = typename alt_index_sequence_generator<N-2,N-2,Ns...>::type;
};

template<std::uint32_t... Ns>
struct alt_index_sequence_generator<0,Ns...> 
{
    using type = index_sequence<Ns...>; 
};

template<std::uint32_t... Ns>
struct alt_index_sequence_generator<1,Ns...> 
{
    using type = index_sequence<Ns...>; 
};


// ************************* //
// *** Push front & back *** //
// ************************* //
template<std::uint32_t N, typename T> 
struct push_front  
{    
    // In general, T is not appendable. 
};  
   
template<std::uint32_t N, std::uint32_t...Ns> 
struct push_front<N, index_sequence<Ns...>> 
{
    using type = index_sequence<N,Ns...>;
};

template<std::uint32_t N, typename T> // Need to include N & T, must be more generic than specialization.
struct push_back                                               //
{                                                             //
    // In general, T is not appendable.                      //         
};                                                          //
                                                           //
template<std::uint32_t N, std::uint32_t...Ns>             //
struct push_back<N, index_sequence<Ns...>>         // <===
{
    using type = index_sequence<Ns...,N>;
};


// *************** //
// *** Reverse *** //
// *************** //
template<typename T> 
struct reverse
{
    // In general, T is not reversible. 
};

template<>
struct reverse<index_sequence<>>
{
    using type = index_sequence<>; 
};

template<std::uint32_t N, std::uint32_t...Ns>
struct reverse<index_sequence<N,Ns...>>
{
    using type = typename push_back<N, typename reverse<index_sequence<Ns...>>::type>::type;
};


// ************** //
// *** Filter *** //
// ************** //
template<typename T> 
struct filter
{
    // In general, T is not filterable. 
};

template<>
struct filter<index_sequence<>>
{
    using type = index_sequence<>; 
};

template<std::uint32_t N0>
struct filter<index_sequence<N0>>
{
    using type = index_sequence<N0>;
};

template<std::uint32_t N0, std::uint32_t N1, std::uint32_t...Ns>
struct filter<index_sequence<N0,N1,Ns...>>
{
    using type = typename push_front<N0, typename filter<index_sequence<Ns...>>::type>::type;
};


// About tuple ...

// ************ //
// *** Test *** //
// ************ //
void test_index_sequence()
{
    static_assert(std::is_same_v<    index_sequence_generator <5>::type, index_sequence<0,1,2,3,4>>, "failed to generate index_sequence");
    static_assert(std::is_same_v<inv_index_sequence_generator <5>::type, index_sequence<4,3,2,1,0>>, "failed to generate inv index_sequence");
    static_assert(std::is_same_v<alt_index_sequence_generator<10>::type, index_sequence<0,2,4,6,8>>, "failed to generate alt index_sequence");
    static_assert(std::is_same_v<alt_index_sequence_generator<11>::type, index_sequence<1,3,5,7,9>>, "failed to generate alt index_sequence");


    using T0 = push_front<11, index_sequence<0,1,2,3,5,7,9>>::type;
    static_assert(std::is_same_v<T0, index_sequence<11,0,1,2,3,5,7,9>>, "failed to push_front");

    using T1 = push_front<99, index_sequence<>>::type;
    static_assert(std::is_same_v<T1, index_sequence<99>>, "failed to push_front");

    using T2 = push_back<11, index_sequence<0,1,2,3,5,7,9>>::type;
    static_assert(std::is_same_v<T2, index_sequence<0,1,2,3,5,7,9,11>>, "failed to push_back");

    using T3 = push_back<99, index_sequence<>>::type;
    static_assert(std::is_same_v<T3, index_sequence<99>>, "failed to push_back");

    using T4 = reverse<index_sequence<1,2,3,4,5,6>>::type;
    static_assert(std::is_same_v<T4, index_sequence<6,5,4,3,2,1>>, "failed to reverse");

    using T5 = reverse<index_sequence<80,70,60,50,40,30,20,10>>::type;
    static_assert(std::is_same_v<T5, index_sequence<10,20,30,40,50,60,70,80>>, "failed to reverse");

    using T6 = filter<index_sequence<10,20,30,40,50,60,70,80,90>>::type;
    static_assert(std::is_same_v<T6, index_sequence<10,30,50,70,90>>, "failed to filter");
}

void test_template_index_sequence()
{
    test_index_sequence();
}
