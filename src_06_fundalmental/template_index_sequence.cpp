#include<iostream>
#include<tuple>
#include<vector>

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
// struct idx_seq_gen { using type = typename idx_seq_generator<N-A,N-B,Ns...>::type; };
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
struct push_back_idx_seq                                               //
{                                                             //
    // In general, T is not appendable.                      //         
};                                                          //
                                                           //
template<std::uint32_t N, std::uint32_t...Ns>             //
struct push_back_idx_seq<N, idx_seq<Ns...>>               // <===
{
    using type = idx_seq<Ns...,N>;
};


// ***************************** //
// *** Filter index sequence *** //
// ***************************** //
template<typename T> 
struct filter_idx_seq
{
    // In general, T is not filterable. 
};

template<>
struct filter_idx_seq<idx_seq<>>
{
    using type = idx_seq<>; 
};

template<std::uint32_t N0>
struct filter_idx_seq<idx_seq<N0>>
{
    using type = idx_seq<N0>;
};

template<std::uint32_t N0, std::uint32_t N1, std::uint32_t...Ns>
struct filter_idx_seq<idx_seq<N0,N1,Ns...>>
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


// ************************************* //
// *** Shuffle tuple (get type only) *** //
// ************************************* //

// *** Method 1 *** //
template<typename T, std::uint32_t...Ns>
struct shuffle_tuple
{
    using type = std::tuple<typename std::tuple_element<Ns,T>::type...>; // BUG : Don't forget "::type"
};

// *** Method 2 *** //
template<typename T, typename IS> // T = tuple, IS = idx_seq
struct shuffle_tuple2
{
    // Unlike method 1, we need a default case here,
    // to convert <T,IS> to <T,Ns...>
};

template<typename T, std::uint32_t...Ns> 
struct shuffle_tuple2<T, idx_seq<Ns...>>
{
    using type = std::tuple<typename std::tuple_element<Ns,T>::type...>; 
};


// ******************** //
// *** Append tuple *** //
// ******************** //
template<typename TUP, typename T> 
struct append_tuple
{
};

template<typename...Ts, typename T>
struct append_tuple<std::tuple<Ts...>,T>
{
    using type = std::tuple<Ts...,T>;
};


// ********************* //
// *** Reverse tuple *** //
// ********************* //
// *** Method 1 (using index sequence) *** //
template<typename T>
struct reverse_tuple
{
    using type = typename shuffle_tuple2<T, typename rev_idx_seq_generator<std::tuple_size<T>::value>::type>::type;
};

// *** Method 2 (using append tuple) *** //
template<typename T>
struct reverse_tuple2
{
    // In general, T is not reversible. 
};

template<typename T>
struct reverse_tuple2<std::tuple<T>> // This T is tuple's internal type. Above T is tuple itself.
{
    using type = std::tuple<T>; 
};

template<typename T, typename...Ts> 
struct reverse_tuple2<std::tuple<T,Ts...>>
{
    using type = typename append_tuple<typename reverse_tuple2<std::tuple<Ts...>>::type, T>::type;
};


// ************ //
// *** Test *** //
// ************ //
void test_template_index_sequence()
{
    static_assert(std::is_same_v<    idx_seq_generator <5>::type, idx_seq<0,1,2,3,4>>, "failed to generate idx_seq");
    static_assert(std::is_same_v<rev_idx_seq_generator <5>::type, idx_seq<4,3,2,1,0>>, "failed to generate rev idx_seq");
    static_assert(std::is_same_v<alt_idx_seq_generator<10>::type, idx_seq<0,2,4,6,8>>, "failed to generate alt idx_seq");
    static_assert(std::is_same_v<alt_idx_seq_generator<11>::type, idx_seq<1,3,5,7,9>>, "failed to generate alt idx_seq");


    using T0 = push_front_idx_seq<11, idx_seq<0,1,2,3,5,7,9>>::type;
    static_assert(std::is_same_v<T0, idx_seq<11,0,1,2,3,5,7,9>>, "failed to push_front_idx_seq");

    using T1 = push_front_idx_seq<99, idx_seq<>>::type;
    static_assert(std::is_same_v<T1, idx_seq<99>>, "failed to push_front_idx_seq");

    using T2 = push_back_idx_seq<11, idx_seq<0,1,2,3,5,7,9>>::type;
    static_assert(std::is_same_v<T2, idx_seq<0,1,2,3,5,7,9,11>>, "failed to push_back_idx_seq");

    using T3 = push_back_idx_seq<99, idx_seq<>>::type;
    static_assert(std::is_same_v<T3, idx_seq<99>>, "failed to push_back_idx_seq");

    using T4 = filter_idx_seq<idx_seq<10,20,30,40,50,60,70,80,90>>::type;
    static_assert(std::is_same_v<T4, idx_seq<10,30,50,70,90>>, "failed to filter_idx_seq");

    using T5 = filter_idx_seq<idx_seq<10>>::type;
    static_assert(std::is_same_v<T5, idx_seq<10>>, "failed to filter_idx_seq");

    using T6 = reverse_idx_seq<idx_seq<1,2,3,4,5,6>>::type;
    static_assert(std::is_same_v<T6, idx_seq<6,5,4,3,2,1>>, "failed to reverse_idx_seq");

    using T7 = reverse_idx_seq<idx_seq<80,70,60,50,40,30,20,10>>::type;
    static_assert(std::is_same_v<T7, idx_seq<10,20,30,40,50,60,70,80>>, "failed to reverse_idx_seq");
}

void test_template_tuple()
{
    using T0 = std::tuple<char, std::uint32_t, std::string, std::pair<double, double>>;
    using X0 = shuffle_tuple<T0,3,2,1>::type;
    using Y0 = std::tuple<std::pair<double, double>, std::string, std::uint32_t>;
    static_assert(std::is_same_v<X0,Y0>, "failed to shuffle_tuple");

    using T1 = std::tuple<char, std::uint32_t, std::string, std::pair<double, double>>;
    using X1 = shuffle_tuple2<T1, idx_seq<3,2,1>>::type;
    using Y1 = std::tuple<std::pair<double, double>, std::string, std::uint32_t>;
    static_assert(std::is_same_v<X1,Y1>, "failed to shuffle_tuple");

    static_assert(std::is_same_v<std::tuple_size<T0>::type, std::integral_constant<std::size_t,4>>, "failed to tuple_size");
    static_assert(std::is_same_v<std::tuple_size<X0>::type, std::integral_constant<std::size_t,3>>, "failed to tuple_size");
    static_assert(std::is_same_v<std::tuple_size<Y0>::type, std::integral_constant<std::size_t,3>>, "failed to tuple_size");
    static_assert(std::tuple_size<T0>::value == 4, "failed to tuple_size");
    static_assert(std::tuple_size<X0>::value == 3, "failed to tuple_size");
    static_assert(std::tuple_size<Y0>::value == 3, "failed to tuple_size");

    using T2 = append_tuple<T0, double>::type;
    using T3 = append_tuple<T2, double>::type;
    using Y2 = std::tuple<char, std::uint32_t, std::string, std::pair<double, double>, double>;
    using Y3 = std::tuple<char, std::uint32_t, std::string, std::pair<double, double>, double, double>;
    static_assert(std::is_same_v<T2,Y2>, "failed to append_tuple");
    static_assert(std::is_same_v<T3,Y3>, "failed to append_tuple");

    using T4 = std::tuple<char, std::uint32_t, std::string, std::pair<double, double>, std::vector<double>, double>;
    using T5 = reverse_tuple<T4>::type;
    using T6 = reverse_tuple<T5>::type;
    static_assert(std::is_same_v<T5,std::tuple<double, std::vector<double>, std::pair<double, double>, std::string, std::uint32_t, char>>, "failed to reverse_tuple");
    static_assert(std::is_same_v<T6,std::tuple<char, std::uint32_t, std::string, std::pair<double, double>, std::vector<double>, double>>, "failed to reverse_tuple");
    static_assert(std::is_same_v<T4,T6>, "failed to reverse_tuple");

    using T7 = reverse_tuple<T4>::type;
    using T8 = reverse_tuple<T7>::type;
    static_assert(std::is_same_v<T7,std::tuple<double, std::vector<double>, std::pair<double, double>, std::string, std::uint32_t, char>>, "failed to reverse_tuple");
    static_assert(std::is_same_v<T8,std::tuple<char, std::uint32_t, std::string, std::pair<double, double>, std::vector<double>, double>>, "failed to reverse_tuple");
    static_assert(std::is_same_v<T4,T8>, "failed to reverse_tuple");
}
