#pragma once
#include<iostream>

// Conclusion
// 1. In template struct declaration :
// -  do not put parameter after parameter-pack
// -  do not put fomrula, like N-1, or sizeof...(Ns)
// 2. In template struct body : 
// -  we can put parameter after parameter-pack
// -  we can put formula, like N-1, or sizeof...(Ns)
// 3. As the pack expands, the tail remains unchanged, see the illustrative comment below.


template<std::size_t ...Ns> struct idx_seq 
{
};



// ********************************* //
// *** Increasing index sequence *** // 
// ********************************* //
// [Approach 1] works
// idx_seq_generator<4>::type = idx_seq_generator<3,3> ::type
//                            = idx_seq_generator<2,2,3> ::type
//                            = idx_seq_generator<1,1,2,3> ::type
//                            = idx_seq_generator<0,0,1,2,3> ::type <--- recursion
//                            = idx_seq<0,1,2,3>                    <--- boundary condition (specialization)

template<std::size_t N, std::size_t ...Ns> 
struct idx_seq_generator
{
    using type = typename idx_seq_generator<N-1,N-1,Ns...>::type;
};

template<std::size_t ...Ns> 
struct idx_seq_generator<0,Ns...>
{
    using type = idx_seq<Ns...>;          
};

// [Approach 2] fails
// idx_seq_generator_failed<4>::type = idx_seq_generator_failed<4,0> ::type
//                                   = idx_seq_generator_failed<4,0,1> ::type
//                                   = idx_seq_generator_failed<4,0,1,2> ::type
//                                   = idx_seq_generator_failed<4,0,1,2,3> ::type
//                                   = idx_seq_generator_failed<4,0,1,2,3,4> ::type
//                                   = idx_seq<0,1,2,3>       
 
template<std::size_t N, std::size_t ...Ns> 
struct idx_seq_generator_failed
{
    using type = typename idx_seq_generator_failed<N,Ns...,sizeof...(Ns)>::type; // <--- compile OK
};  
/*
template<std::size_t N, std::size_t ...Ns> 
struct idx_seq_generator_failed<N,Ns...,N> // <--- compile error here
{
    using type = idx_seq<Ns...>; 
};  */



// ******************************* //
// *** Inverted index sequence *** //
// ******************************* //
// [Approach 1] works
// inv_idx_seq_generator<4>::type = inv_idx_seq_generator<4,0> ::type
//                                = inv_idx_seq_generator<4,1,0> ::type
//                                = inv_idx_seq_generator<4,2,1,0> ::type
//                                = inv_idx_seq_generator<4,3,2,1,0> ::type 
//                                = inv_idx_seq_generator<4,4,3,2,1,0> ::type 
//                                = rev_idx_seq<3,2,1,0>

template<std::size_t N, std::size_t ...Ns> 
struct inv_idx_seq_generator
{
    using type = typename inv_idx_seq_generator<N, sizeof...(Ns), Ns...>::type;
};

template<std::size_t N, std::size_t ...Ns>
struct inv_idx_seq_generator<N,N,Ns...>
{
    using type = idx_seq<Ns...>;    
}; 

// [Approach 2] fails
// inv_idx_seq_generator_failed<4>::type = inv_idx_seq_generator_failed<4,0> ::type
//                                       = inv_idx_seq_generator_failed<4,1,0> ::type
//                                       = inv_idx_seq_generator_failed<4,2,1,0> ::type
//                                       = inv_idx_seq_generator_failed<4,3,2,1,0> ::type
//                                       = rev_idx_seq<3,2,1,0>  
/*
template<std::size_t N, std::size_t ...Ns> 
struct inv_idx_seq_generator_failed
{
    using type = typename inv_idx_seq_generator_failed<N, sizeof...(Ns), Ns...>::type;
};

template<std::size_t N, std::size_t ...Ns>
struct inv_idx_seq_generator_failed<N,N-1,Ns...> // <--- in here : do not put formula, do not put oher para after para-pack
{
    using type = idx_seq<Ns...>;                 // <--- in here : we can put formula, we can put oher para after para-pack
};  
*/

// ************************************************************************************* //
// Making it more generic ...
//
// template<std::size_t N, std::size_t ...Ns> 
// struct idx_seq_gen { using type = typename idx_seq_gen<N-A,N-B,Ns...>::type; };
//
// idx_seq_gen<N>::type = idx_seq_gen<N- A, N-B>::type
//                      = idx_seq_gen<N-2A, N-B- A, N-B>::type
//                      = idx_seq_gen<N-3A, N-B-2A, N-B- A, N-B>::type
//                      = idx_seq_gen<N-4A, N-B-3A, N-B-2A, N-B- A, N-B>::type
//                      = idx_seq_gen<N-5A, N-B-4A, N-B-3A, N-B-2A, N-B-A, N-B>::type
//                      = idx_seq          <N-B-4A, N-B-3A, N-B-2A, N-B-A, N-B>  
//                                          ^--- by setting constraint on 1st parameter
// A defines delta
// B defines offset from N
// ************************************************************************************* //



// ******************************** //
// *** Alternate index sequence *** //
// ******************************** //
template<std::size_t N, std::size_t... Ns>
struct alt_idx_seq_generator
{
    using type = typename alt_idx_seq_generator<N-2,N-2,Ns...>::type;
};

template<std::size_t... Ns>
struct alt_idx_seq_generator<0,Ns...> 
{
    using type = idx_seq<Ns...>; 
};

template<std::size_t... Ns>
struct alt_idx_seq_generator<1,Ns...> 
{
    using type = idx_seq<Ns...>; 
};



// ******************************************* //
// *** Push front & back to index sequence *** //
// ******************************************* //
// 1. idx_seq<> does exist
// 2. we can push front/back to idx_seq<>
//
template<std::size_t N, typename T> 
struct push_front_idx_seq  
{    
    // In general, T is not appendable. 
};  
   
template<std::size_t N, std::size_t...Ns> 
struct push_front_idx_seq<N, idx_seq<Ns...>>  
{
    using type = idx_seq<N,Ns...>;
};

template<std::size_t N, typename T> // Need to include N & T, must be more generic than specialization.
struct push_back_idx_seq                                       //
{                                                             //
    // In general, T is not appendable.                      //         
};                                                          //
                                                           //
template<std::size_t N, std::size_t...Ns>                 //
struct push_back_idx_seq<N, idx_seq<Ns...>>              // <===
{
    using type = idx_seq<Ns...,N>;
};



// ****************************** //
// *** Reverse index sequence *** //
// ****************************** //
// Depends on push_back.
// Reversing index sequence is different from constructing an inverted index sequence.
//
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

template<std::size_t N, std::size_t...Ns>
struct reverse_idx_seq<idx_seq<N,Ns...>>
{
    using type = typename push_back_idx_seq<N, typename reverse_idx_seq<idx_seq<Ns...>>::type>::type;
};



// ***************************** //
// *** Filter index sequence *** //
// ***************************** //
// Depends on push_front.
//
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

template<std::size_t N0>
struct filter_idx_seq<idx_seq<N0>> // <--- This is implementation for boundary case.
{
    using type = idx_seq<N0>;
};

template<std::size_t N0, std::size_t N1, std::size_t...Ns>
struct filter_idx_seq<idx_seq<N0,N1,Ns...>> // <--- This is implementation for recursion.
{
    using type = typename push_front_idx_seq<N0, typename filter_idx_seq<idx_seq<Ns...>>::type>::type;
};



// ***************************** //
// *** Dedupe index sequence *** //
// ***************************** //
template<typename T>
struct dedupe_idx_seq
{
    // In general, T is not dedupable. 
};

template<>
struct dedupe_idx_seq<idx_seq<>>
{
    using type = idx_seq<>;
};

template<std::size_t N>
struct dedupe_idx_seq<idx_seq<N>>
{
    using type = idx_seq<N>;
};
  
// template<std::size_t N, std::size_t...Ns>
// struct dedupe_idx_seq<idx_seq<N,N,Ns...>>
// {
//     using type = typename push_front_idx_seq<N, typename dedupe_idx_seq<idx_seq<Ns...>>::type>::type;
// }; 
//
// This implementation fails, it converts :
// idx_seq<1,1,1,1,1>            -->  idx_seq<1,1,1>
// idx_seq<1,1,1,1,1,2,3,3,3,4>  -->  idx_seq<1,1,1,2,3,3,4>
//
// When N == 1st item of Ns..., then it cannot dedupe that item, which is incorrect.

template<std::size_t N, std::size_t...Ns>
struct dedupe_idx_seq<idx_seq<N,N,Ns...>>
{
    using type = typename dedupe_idx_seq<
                 typename push_front_idx_seq<N, typename dedupe_idx_seq<idx_seq<Ns...>>::type>::type
                 >::type;
}; 

template<std::size_t N0, std::size_t N1, std::size_t...Ns>
struct dedupe_idx_seq<idx_seq<N0,N1,Ns...>>
{
    using type = typename push_front_idx_seq<N0, typename dedupe_idx_seq<idx_seq<N1,Ns...>>::type>::type;
};



// ********************************************************************************* //
// For index sequence, we can get reversed type :
//
// reverse_idx_seq<idx_seq<1,2,3,4,5>>::type == idx_seq<5,4,3,2,1>
//
// For tuple, since it has members, we can get reversed type AND reversed object :
// 
// reverse_tuple<std::tuple<A,B,C,D,E>>::type == std::tuple<E,D,C,B,A>    <--- reverse the type 
// reverse_tuple_object(t0) == t1                                         <--- reverse the type and the value 
//
// where t0 is an object of std::tuple<A,B,C,D,E>
//       t1 is an object of std::tuple<E,D,C,B,A>
//
// Remark : reverse of tuple object is easier than reverse of tuple type, because :
// 1. there is an input object (see tuple.h), we can operate with
// 2. there is a factory std::make_tuple<>() which can resolve T... 
// ********************************************************************************* //
