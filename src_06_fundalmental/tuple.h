#pragma once
#include<iostream>
#include<tuple>
#include"idx_seq.h"

// In the following : 
// 1. template class is type-traits
// 2. template function is factory 
// 3. they might differ in implementation 
// 4. for template class, we usually have to (see tuple_size comment)
// -  implement a generic class as interface 
// -  implement a specialized class as implementation 
// -  the generic class defines dimension of template 
// -  the specialized class converts original dimension to another dimension (see remark A)
// 5. if the specialized class is implemented recursively, we need to provide boundary case(s)



// ****************** //
// *** Tuple size *** //
// ****************** //
template<typename TUP>
struct tuple_size 
{
    using type = std::integral_constant<std::uint32_t, 0>;
    static const std::uint32_t value = type::value;
};

template<typename...Ts>
struct tuple_size<std::tuple<Ts...>>
{
    using type = std::integral_constant<std::uint32_t, sizeof...(Ts)>;
    static const std::uint32_t value = type::value;
};



// ********************* //
// *** Tuple element *** //
// ********************* //
// *** Method 1 *** //
template<std::uint32_t N, typename TUP> // <--- interface
struct tuple_element 
{
};

template<typename T, typename...Ts> // <--- implementation : boundary condition 
struct tuple_element<0, std::tuple<T,Ts...>>
{
    using type = T;
};

template<std::uint32_t N, typename T, typename...Ts> // <--- implementation : recursion
struct tuple_element<N, std::tuple<T,Ts...>>
{
    using type = tuple_element<N-1, std::tuple<Ts...>>::type;
};

// *** Method 2 *** //
template<std::uint32_t N, typename TUP> 
struct tuple_element2
{
    using type = std::remove_cvref_t<decltype(std::get<N>(std::declval<TUP>()))>;
//  using type =                     decltype(std::get<N>(std::declval<TUP>())); // BUG : Does not work !!!
};



// ********************* //
// *** Shuffle tuple *** //
// ********************* //
// *** Method 1 *** //
template<typename TUP, std::uint32_t...Ns>
struct shuffle_tuple
{
    using type = std::tuple<typename std::tuple_element<Ns,TUP>::type...>; // BUG : Don't forget "::type"
};

// *** Method 2 *** //
template<typename TUP, typename IS> 
struct shuffle_tuple2
{
};

template<typename TUP, std::uint32_t...Ns> 
struct shuffle_tuple2<TUP, idx_seq<Ns...>> // Remark A : convert from dimension <TUP,IS> to <TUP,Ns...>
{
    using type = std::tuple<typename std::tuple_element<Ns,TUP>::type...>; 
};

// *** Factory *** //
template<typename TUP, std::size_t...Ns>
auto make_shuffle_tuple(const TUP& x, std::index_sequence<Ns...> dummy)
{
    return std::make_tuple(std::get<Ns>(x)...);
}



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

// template<typename...Ts, typename T>
// auto make_append_tuple(const std::tuple<Ts...>& tup, const T& x)
// {
//     return std::make_tuple(std::forward<Ts>(xs)..., x);
// }



// ********************* //
// *** Reverse tuple *** //
// ********************* //
// *** Method 1 (using index sequence) *** //
template<typename TUP>
struct reverse_tuple
{
    using type = typename shuffle_tuple2<TUP, typename inv_idx_seq_generator<std::tuple_size<TUP>::value>::type>::type;
};

// *** Method 2 (using append tuple) *** //
template<typename T>
struct reverse_tuple2
{
    // In general, T is not reversible. 
};

template<typename T>
struct reverse_tuple2<std::tuple<T>> 
{
    using type = std::tuple<T>; 
};

template<typename T, typename...Ts> 
struct reverse_tuple2<std::tuple<T,Ts...>>
{
    using type = typename append_tuple<typename reverse_tuple2<std::tuple<Ts...>>::type, T>::type;
};

// *** Factory *** //
template<typename TUP, std::size_t...Ns>
auto make_reverse_tuple_impl(const TUP& x, std::index_sequence<Ns...> dummy)
{
    return std::make_tuple(std::get<std::tuple_size<TUP>::value-1-Ns>(x)...); // BUG : Don't forget minus one, otherwise it goes out of tuple range
}

template<typename TUP>
auto make_reverse_tuple(const TUP& x)
{
    return make_reverse_tuple_impl(x, std::make_index_sequence<std::tuple_size<TUP>::value>{}); 
}



// ***************** //
// *** Tuple cat *** //
// ***************** //
template<typename TUP0, typename TUP1>
struct tuple_cat
{
};

template<typename TUP0, typename T>
struct tuple_cat<TUP0, std::tuple<T>>
{
    using type = typename append_tuple<TUP0,T>::type;
};

template<typename TUP0, typename T, typename...Ts>
struct tuple_cat<TUP0, std::tuple<T,Ts...>>
{
    using type = tuple_cat<typename append_tuple<TUP0,T>::type, std::tuple<Ts...>>::type;
}; 



// ******************* //
// *** Tuple apply *** //
// ******************* //

