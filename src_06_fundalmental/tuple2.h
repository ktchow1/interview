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



// *********** //
// *** Get *** //
// *********** //
template<typename TUP, std::size_t...Ns, std::size_t N0, std::size_t N1>
auto get_helper(const TUP& tup, idx_seq<N0,Ns...> dummy, std::integral_constant<std::size_t, N1> index_of_item)
{
    return get_helper(tup.tup, idx_seq<Ns...>{}, index_of_item);
}

template<typename TUP, std::size_t...Ns, std::size_t N>
auto get_helper(const TUP& tup, idx_seq<N,Ns...> dummy, std::integral_constant<std::size_t, N> index_of_item)
{
    return tup.x;
}

template<std::size_t N, typename TUP>
auto get(const TUP& tup)
{
    return get_helper(tup, typename idx_seq_generator<my_tuple_size<TUP>::value>::type{}, std::integral_constant<std::size_t,N>{});
}



// *************** //
// *** Factory *** //
// *************** //
template<typename...Ts>
auto make_my_tuple(const Ts&...ts)
{
    return my_tuple<Ts...>{ts...}; 
}
