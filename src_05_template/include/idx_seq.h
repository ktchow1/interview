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
// 4. Use std::size_t instead of std::uint32_t, as the latter is not compatiable with std::tuple_size
// 5. To get the type  of selected items in tuple, use variadic Ts... ans std::tuple<Ts...>
//    To get the value of selected items in tuple, use variatic Ns... and std::make_tuple
/*
[Summary]

    idx_seq            |                        |   my_tuple
    idx_seq_generator  |  tuple_size            |   my_tuple_size
inv_idx_seq_generator  |  tuple_element         |   my_tuple_element
off_idx_seq_generator  |                        |   get
                       |                        |   make_my_tuple
-----------------------+------------------------+--------------------
                       |  shuffle               |   
push_front/push_back   |  push_front/push_back  |
pop_front(failed)      |  pop_front(failed)     |
reverse                |  reverse               |
filter                 |  filter                |
dedupe                 |  tuple_cat             |
dot_product            |  tuple_apply           |

*/
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
//
// Making it more generic ...
//
// template<std::size_t N, std::size_t ...Ns> 
// struct idx_seq_generator { using type = typename idx_seq_generator<N-A,N-B,Ns...>::type; };
//
// idx_seq_generator<N>::type = idx_seq_generator<N- A, N-B>::type
//                            = idx_seq_generator<N-2A, N-B- A, N-B>::type
//                            = idx_seq_generator<N-3A, N-B-2A, N-B- A, N-B>::type
//                            = idx_seq_generator<N-4A, N-B-3A, N-B-2A, N-B- A, N-B>::type
//                            = idx_seq_generator<N-5A, N-B-4A, N-B-3A, N-B-2A, N-B-A, N-B>::type
//                            = idx_seq          <N-B-4A, N-B-3A, N-B-2A, N-B-A, N-B>  
//                                                ^--- by setting constraint on 1st parameter
// A defines delta
// B defines offset from N
//
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



// ***************************** //
// *** Offset index sequence *** //
// ***************************** //
//  create type idx_seq<N0,N0+1,...,N-1>, please ensure N0 <= N-1
//
template<std::size_t N0, std::size_t N, std::size_t ...Ns> 
struct off_idx_seq_generator
{
    using type = typename off_idx_seq_generator<N0,N-1,N-1,Ns...>::type;
};

template<std::size_t N0, std::size_t ...Ns> 
struct off_idx_seq_generator<N0,N0,Ns...>
{
    using type = idx_seq<Ns...>;          
};



// ******************************************* //
// *** Push front & back to index sequence *** //
// ******************************************* //
// 1. idx_seq<> does exist
// 2. we can push front/back to idx_seq<>
//
template<typename T, std::size_t...Ns> 
struct push_front_idx_seq  
{    
};  
  
template<std::size_t...TUP_Ns, std::size_t...Ns> 
struct push_front_idx_seq<idx_seq<TUP_Ns...>,Ns...>  
{
    using type = idx_seq<Ns...,TUP_Ns...>;
};

template<typename T, std::size_t...Ns> 
struct push_back_idx_seq 
{ 
};
 
template<std::size_t...TUP_Ns, std::size_t...Ns> 
struct push_back_idx_seq<idx_seq<TUP_Ns...>,Ns...>
{
    using type = idx_seq<TUP_Ns...,Ns...>;
};



// ********************************************* //
// *** Pop front & back from index sequence  *** //
// ********************************************* //
// The following method is ambiguous. [FAILED]
//
// Why pop is more difficult to implement?
// * this is recursive
// * while unlike other recursive template struct, 
//   there is no reduction in parameter-pack as recursion proceeds.
//
template<typename T, std::size_t N> 
struct pop_front_idx_seq_failed
{    
};

template<typename TUP> 
struct pop_front_idx_seq_failed<TUP,0>
{    
    using type = TUP;
};

template<std::size_t TUP_N, std::size_t...TUP_Ns, std::size_t N> 
struct pop_front_idx_seq_failed<idx_seq<TUP_N,TUP_Ns...>,N>  
{
    using type = typename pop_front_idx_seq_failed<idx_seq<TUP_Ns...>,N-1>::type;
};

// Todo : please try to mimic get<N>(tuple) implementation



// ****************************** //
// *** Reverse index sequence *** //
// ****************************** //
// Reversing index sequence is different from constructing an inverted index sequence.
//
template<typename T> 
struct reverse_idx_seq
{
};

template<>
struct reverse_idx_seq<idx_seq<>>
{
    using type = idx_seq<>; 
};

template<std::size_t N, std::size_t...Ns>
struct reverse_idx_seq<idx_seq<N,Ns...>>
{
    using type = typename push_back_idx_seq<typename reverse_idx_seq<idx_seq<Ns...>>::type, N>::type;
};



// ***************************** //
// *** Filter index sequence *** //
// ***************************** //
template<typename T> 
struct filter_idx_seq // <--- This is interface.
{
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
    using type = typename push_front_idx_seq<typename filter_idx_seq<idx_seq<Ns...>>::type, N0>::type;
};



// ***************************** //
// *** Dedupe index sequence *** //
// ***************************** //
template<typename T>
struct dedupe_idx_seq
{
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
//     using type = typename push_front_idx_seq<typename dedupe_idx_seq<idx_seq<Ns...>>::type, N>::type;
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
                 typename push_front_idx_seq<typename dedupe_idx_seq<idx_seq<Ns...>>::type, N>::type
                 >::type;
}; 

template<std::size_t N0, std::size_t N1, std::size_t...Ns>
struct dedupe_idx_seq<idx_seq<N0,N1,Ns...>>
{
    using type = typename push_front_idx_seq<typename dedupe_idx_seq<idx_seq<N1,Ns...>>::type, N0>::type;
};



// *************************** //
// *** Dot product and zip *** //
// *************************** //
template<typename T0, typename T1>
struct dot_product
{
};

template<std::size_t...Ns0, std::size_t...Ns1>
struct dot_product<idx_seq<Ns0...>, idx_seq<Ns1...>>
{
    using type = idx_seq<Ns0*Ns1...>;
};

template<typename...Is>
struct zip_product
{
};

template<typename IS>
struct zip_product<IS>
{
    using type = IS;
};

template<typename IS0, typename IS1, typename...ISs>
struct zip_product<IS0,IS1,ISs...>
{
    using type = typename zip_product<typename dot_product<IS0,IS1>::type, ISs...>::type;
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

