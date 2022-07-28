#pragma once
#include<iostream>

// Conclusion
// 1. In template struct declaration :
// -  do not put parameter after parameter-pack
// -  do not put fomrula, like N-1, or sizeof...(Ns)
// 2. In template struct body : 
// -  we can put parameter after parameter-pack
// -  we can put formula, like N-1, or sizeof<Plug>_...(Ns)
// 3. As the pack expands, the tail remains unchanged, see the illustrative comment below.


template<std::uint32_t ...Ns> struct idx_seq 
{
};

// Objective :
// idx_seq_generator<4>::type ---> idx_seq<0,1,2,3>
//
// Approach 1 works, it supposes to do :
// (1) idx_seq_generator<4>::type = idx_seq_generator<3,3> ::type
//                                = idx_seq_generator<2,2,3> ::type
//                                = idx_seq_generator<1,1,2,3> ::type
//                                = idx_seq_generator<0,0,1,2,3> ::type <--- recursion
//                                = idx_seq<0,1,2,3>                    <--- boundary condition (specialization)

template<std::uint32_t N, std::uint32_t ...Ns> 
struct idx_seq_generator
{
    using type = typename idx_seq_generator<N-1,N-1,Ns...>::type;
};

template<std::uint32_t ...Ns> 
struct idx_seq_generator<0,Ns...>
{
    using type = idx_seq<Ns...>;          
};

// Approach 2 fails, it supposes to do :
// (2) idx_seq_generator_failed<4>::type = idx_seq_generator_failed<4,0> ::type
//                                       = idx_seq_generator_failed<4,0,1> ::type
//                                       = idx_seq_generator_failed<4,0,1,2> ::type
//                                       = idx_seq_generator_failed<4,0,1,2,3> ::type
//                                       = idx_seq_generator_failed<4,0,1,2,3,4> ::type
//                                       = idx_seq<0,1,2,3>       
/* 

template<std::uint32_t N, std::uint32_t ...Ns> 
struct idx_seq_generator_failed
{
    using type = typename idx_seq_generator_failed<N,Ns...,sizeof...(Ns)>::type; // <--- compile OK
};

template<std::uint32_t N, std::uint32_t ...Ns> 
struct idx_seq_generator_failed<N,Ns...,N> // <--- compile error here
{
    using type = idx_seq<Ns...>; 
};  

*/
// Objective :
// idx_seq_generator<4>::type ---> idx_seq<0,1,2,3>
//
// Approach 1 works, it supposes to do :
// (1) rev_idx_seq_generator<4>::type = rev_idx_seq_generator<4,0> ::type
//                                    = rev_idx_seq_generator<4,1,0> ::type
//                                    = rev_idx_seq_generator<4,2,1,0> ::type
//                                    = rev_idx_seq_generator<4,3,2,1,0> ::type 
//                                    = rev_idx_seq_generator<4,4,3,2,1,0> ::type 
//                                    = rev_idx_seq<3,2,1,0>

template<std::uint32_t N, std::uint32_t ...Ns> 
struct rev_idx_seq_generator
{
    using type = typename rev_idx_seq_generator<N, sizeof...(Ns), Ns...>::type;
};

template<std::uint32_t N, std::uint32_t ...Ns>
struct rev_idx_seq_generator<N,N,Ns...>
{
    using type = idx_seq<Ns...>;    
}; 

// Approach 2 fails, it supposes to do :
// (2) rev_idx_seq_generator<4>::type = rev_idx_seq_generator<4,0> ::type
//                                    = rev_idx_seq_generator<4,1,0> ::type
//                                    = rev_idx_seq_generator<4,2,1,0> ::type
//                                    = rev_idx_seq_generator<4,3,2,1,0> ::type
//                                    = rev_idx_seq<3,2,1,0>  
/*

template<std::uint32_t N, std::uint32_t ...Ns> 
struct rev_idx_seq_generator
{
    using type = typename rev_idx_seq_generator<N, sizeof...(Ns), Ns...>::type;
};

template<std::uint32_t N, std::uint32_t ...Ns>
struct rev_idx_seq_generator<N,N-1,Ns...> // <--- in here : do not put formula, do not put oher para after para-pack
{
    using type = idx_seq<Ns...>;          // <--- in here : we can put formula, we can put oher para after para-pack
};  
*/

// ****************************************************************************************************************************** //
// What does this do? 
//
// template<std::uint32_t N, std::uint32_t ...Ns> 
// struct idx_seq_gen { using type = typename idx_seq_gen<N-A,N-B,Ns...>::type; };
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
struct alt_idx_seq_generator
{
    using type = typename alt_idx_seq_generator<N-2,N-2,Ns...>::type;
};

template<std::uint32_t... Ns>
struct alt_idx_seq_generator<0,Ns...> 
{
    using type = idx_seq<Ns...>; 
};

template<std::uint32_t... Ns>
struct alt_idx_seq_generator<1,Ns...> 
{
    using type = idx_seq<Ns...>; 
};


// ******************************************* //
// *** Push front & back to index sequence *** //
// ******************************************* //
template<std::uint32_t N, typename T> 
struct push_front_idx_seq  
{    
    // In general, T is not appendable. 
};  
   
template<std::uint32_t N, std::uint32_t...Ns> 
struct push_front_idx_seq<N, idx_seq<Ns...>> 
{
    using type = idx_seq<N,Ns...>;
};

template<std::uint32_t N, typename T> // Need to include N & T, must be more generic than specialization.
struct push_back_idx_seq                                       //
{                                                             //
    // In general, T is not appendable.                      //         
};                                                          //
                                                           //
template<std::uint32_t N, std::uint32_t...Ns>             //
struct push_back_idx_seq<N, idx_seq<Ns...>>              // <===
{
    using type = idx_seq<Ns...,N>;
};


// ***************************** //
// *** Filter index sequence *** //
// ***************************** //
template<typename T> 
struct filter_idx_seq // <--- This is interface.
{
    // In general, T is not filterable. 
};

template<>
struct filter_idx_seq<idx_seq<>> // <--- This is implementation for boundary case.
{
    using type = idx_seq<>; 
};

template<std::uint32_t N0>
struct filter_idx_seq<idx_seq<N0>> // <--- This is implementation for boundary case.
{
    using type = idx_seq<N0>;
};

template<std::uint32_t N0, std::uint32_t N1, std::uint32_t...Ns>
struct filter_idx_seq<idx_seq<N0,N1,Ns...>> // <--- This is implementation for recursion.
{
    using type = typename push_front_idx_seq<N0, typename filter_idx_seq<idx_seq<Ns...>>::type>::type;
};


// ****************************** //
// *** Reverse index sequence *** //
// ****************************** //
// Note we are reversing a type to get an output type, so that :
// 1. both are idx_seq
// 2. they have their indices reversed
//
// Thus, do not declare as : template<std::uint32_t...Ns> struct reverse_idx_seq {};

template<typename T> 
struct reverse_idx_seq
{
    // In general, T is not reversible. 
};

template<>
struct reverse_idx_seq<idx_seq<>>
{
    using type = idx_seq<>; 
};

template<std::uint32_t N, std::uint32_t...Ns>
struct reverse_idx_seq<idx_seq<N,Ns...>>
{
    using type = typename push_back_idx_seq<N, typename reverse_idx_seq<idx_seq<Ns...>>::type>::type;
};

