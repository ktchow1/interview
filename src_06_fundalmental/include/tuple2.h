#include<iostream>


// *************************** //
// *** Design my own tuple *** //
// *************************** //
// Keep tuple as POD, no explicit constructor is needed, aggregate initialization works.
//
template<typename T, typename...Ts>
struct my_tuple
{
    T x;
    my_tuple<Ts...> tup;
};

template<typename T>
struct my_tuple<T>
{
    T x;
};



// ****************** //
// *** Tuple size *** //
// ****************** //
template<typename T>
struct my_tuple_size
{
    using type = std::integral_constant<std::size_t, 0>;
    static const std::size_t value = type::value;
};

template<typename...Ts>
struct my_tuple_size<my_tuple<Ts...>>
{
    using type = std::integral_constant<std::size_t, sizeof...(Ts)>;
    static const std::size_t value = type::value;
};



// ********************* //
// *** Tuple element *** //
// ********************* //
// see implementation in tuple.h



// ********************************************** //
// *** Get (method 1 : works but complicated) *** //
// ********************************************** //
// template<typename TUP, std::size_t...Ns, std::size_t N0, std::size_t N1>
// auto get_helper(const TUP& tup, idx_seq<N0,Ns...> current, std::integral_constant<std::size_t, N1> target)
// {
//     return get_helper(tup.tup, idx_seq<Ns...>{}, target);
// }
//
// template<typename TUP, std::size_t...Ns, std::size_t N>
// auto get_helper(const TUP& tup, idx_seq<N,Ns...> current, std::integral_constant<std::size_t, N> target)
// {
//     return tup.x;
// }
//
// template<std::size_t N, typename TUP>
// auto get(const TUP& tup)
// {
//     return get_helper(tup, typename idx_seq_generator<my_tuple_size<TUP>::value>::type{}, std::integral_constant<std::size_t,N>{});
// }



// ***************************************** //
// *** Get (method 1 2 works and simple) *** //
// ***************************************** //
template<typename TUP, std::size_t N0, std::size_t N1>
auto get_helper(const TUP& tup, 
                std::integral_constant<std::size_t,N0> current, 
                std::integral_constant<std::size_t,N1> target)
{
    return get_helper(tup.tup, std::integral_constant<std::size_t,N0+1>{}, target);
}

template<typename TUP, std::size_t N>
auto get_helper(const TUP& tup, 
                std::integral_constant<std::size_t,N> current, 
                std::integral_constant<std::size_t,N> target)
{
    return tup.x;
}

template<std::size_t N, typename TUP>
auto get(const TUP& tup)
{
    return get_helper(tup, std::integral_constant<std::size_t,0>{},
                           std::integral_constant<std::size_t,N>{});
}


// *************** //
// *** Factory *** //
// *************** //
template<typename...Ts>
auto make_my_tuple(const Ts&...ts)
{
    return my_tuple<Ts...>{ts...}; 
}
