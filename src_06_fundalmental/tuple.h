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
    using type = std::integral_constant<std::size_t, 0>;
    static const std::size_t value = type::value;
};

template<typename...Ts>
struct tuple_size<std::tuple<Ts...>>
{
    using type = std::integral_constant<std::size_t, sizeof...(Ts)>;
    static const std::size_t value = type::value;
};



// ********************* //
// *** Tuple element *** //
// ********************* //
// *** Method 1 *** //
template<std::size_t N, typename TUP> // <--- interface
struct tuple_element 
{
};

template<typename T, typename...Ts> // <--- implementation : boundary condition 
struct tuple_element<0, std::tuple<T,Ts...>>
{
    using type = T;
};

template<std::size_t N, typename T, typename...Ts> // <--- implementation : recursion
struct tuple_element<N, std::tuple<T,Ts...>>
{
    using type = tuple_element<N-1, std::tuple<Ts...>>::type;
};

// *** Method 2 *** //
template<std::size_t N, typename TUP> 
struct tuple_element2
{
    using type = std::remove_cvref_t<decltype(std::get<N>(std::declval<TUP>()))>;
//  using type =                     decltype(std::get<N>(std::declval<TUP>())); // BUG : Does not work !!!
};



// ********************* //
// *** Shuffle tuple *** //
// ********************* //
// Slight difference between method 1&2
// 1. shuffle_tuple         <TUP,1,3,5,7>
// 2. shuffle_tuple2<idx_seq<TUP,1,3,5,7>>
  
// *** Method 1 *** //
template<typename TUP, std::size_t...Ns>
struct shuffle_tuple
{
    using type = std::tuple<typename std::tuple_element<Ns,TUP>::type...>; 
};

// *** Method 2 *** //
template<typename TUP, typename T> 
struct shuffle_tuple2
{
};

template<typename TUP, std::size_t...Ns> 
struct shuffle_tuple2<TUP, idx_seq<Ns...>> // Remark A : convert from dimension <TUP,T> to <TUP,Ns...>
{
    using type = std::tuple<typename std::tuple_element<Ns,TUP>::type...>; 
};

// *** Factory *** //
template<typename TUP, std::size_t...Ns>
auto make_shuffle_tuple(const TUP& x, idx_seq<Ns...> dummy)
{
    return std::make_tuple(std::get<Ns>(x)...);
}



// ********************************** //
// *** Push front & back to tuple *** //
// ********************************** //
template<typename TUP, typename...Ts> 
struct push_back_tuple
{
};

template<typename...TUP_Ts, typename...Ts>
struct push_back_tuple<std::tuple<TUP_Ts...>,Ts...>
{
    using type = std::tuple<TUP_Ts...,Ts...>;
};

// *** Factory *** //
// The following technique is useful in append / reverse / cat :
// - we have TUP   in the interface
// - we need Ns... in the implementation
// - introduce a helper to do conversion
//
template<typename TUP, std::size_t...Ns, typename...Ts>
auto make_push_back_tuple_helper(const TUP& tup, idx_seq<Ns...> dummy, const Ts&...xs) 
{
    return std::make_tuple(std::get<Ns>(tup)..., xs...);
}

template<typename TUP, typename...Ts>
auto make_push_back_tuple(const TUP& tup, const Ts&...xs) 
{
    return make_push_back_tuple_helper(tup, typename idx_seq_generator<std::tuple_size<TUP>::value>::type{}, xs...);
}



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
    using type = typename push_back_tuple<typename reverse_tuple2<std::tuple<Ts...>>::type, T>::type;
};

// *** Factory *** //
template<typename TUP, std::size_t...Ns> // BUG : Don't use std::uint32_t instead of std::size_t, as std::tuple_size returns the latter
auto make_reverse_tuple_helper(const TUP& tup, idx_seq<Ns...> dummy)
{
    return std::make_tuple(std::get<std::tuple_size<TUP>::value-1-Ns>(tup)...); // BUG : Don't forget minus one, otherwise it goes out of tuple range
}

template<typename TUP>
auto make_reverse_tuple(const TUP& tup)
{
    return make_reverse_tuple_helper(tup, typename idx_seq_generator<std::tuple_size<TUP>::value>::type{}); // BUG : Don't forget "typename" and "::type" 
}



// ********************* //
// **** Filter tuple *** //
// ********************* //



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
    using type = typename push_back_tuple<TUP0,T>::type;
};

template<typename TUP0, typename T, typename...Ts>
struct tuple_cat<TUP0, std::tuple<T,Ts...>>
{
    using type = tuple_cat<typename push_back_tuple<TUP0,T>::type, std::tuple<Ts...>>::type;
}; 

// *** Factory *** //
template<typename TUP0, typename TUP1, std::size_t...Ns> 
auto make_tuple_cat_helper(const TUP0& tup0, const TUP1& tup1, idx_seq<Ns...> dummy)
{
    return make_push_back_tuple(tup0, std::get<Ns>(tup1)...);
}

template<typename TUP0, typename TUP1>  
auto make_tuple_cat(const TUP0& tup0, const TUP1& tup1)
{
    return make_tuple_cat_helper(tup0, tup1, typename idx_seq_generator<std::tuple_size<TUP1>::value>::type{});
}



// ******************* //
// *** Tuple apply *** //
// ******************* //
// There is no struct traits for this part.

template<typename TUP, std::size_t...Ns, typename F>
void apply_helper(TUP& tup, idx_seq<Ns...> dummy, F& fct)
{
    fct(std::get<Ns>(tup)...);
}

template<typename TUP, typename F>
void apply(TUP& tup, F& fct)
{
    apply_helper(tup, typename idx_seq_generator<std::tuple_size<TUP>::value>::type{}, fct);
}



