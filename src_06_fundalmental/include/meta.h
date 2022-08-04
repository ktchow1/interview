#include<iostream>



template<std::uint32_t N, std::uint32_t M>
constexpr bool less_than() 
{
    return N<M;
}

template<std::uint32_t N, std::uint32_t M>
constexpr std::uint32_t abs_diff() 
{
    if (less_than<N,M>()) return M-N; // if constexpr is optional here ...
    else return N-M;
}

template<std::uint32_t N, std::uint32_t M, typename std::enable_if<less_than<N,M>(), int>::type dummy = 0>
constexpr std::uint32_t abs_diff2()
{
    return M-N;
}

template<std::uint32_t N, std::uint32_t M, typename std::enable_if<!less_than<N,M>(), int>::type dummy = 0>
constexpr std::uint32_t abs_diff2()
{
    return N-M;
};



// ************************ //
// *** Min/Max function *** //
// ************************ //
template<std::uint32_t N, std::uint32_t M>
constexpr std::uint32_t min() 
{
    if (N<M) return N;
    else return M;
}

template<std::uint32_t N, std::uint32_t M>
constexpr std::uint32_t max() 
{
    if (N>=M) return N;
    else return M;
}



// ******************************* //
// *** Min/Max variadic traits *** //
// ******************************* //
template<std::uint32_t N, std::uint32_t...Ns>
struct min_item
{
    static const std::uint32_t value = min_item<N, min_item<Ns...>::value>::value;
};

template<std::uint32_t N, std::uint32_t M> // Why no ambiguity?
struct min_item<N,M>
{
    static const std::uint32_t value = min<N, M>();
};

template<std::uint32_t N, std::uint32_t...Ns>
struct max_item
{
    static const std::uint32_t value = max_item<N, max_item<Ns...>::value>::value;
};

template<std::uint32_t N, std::uint32_t M>
struct max_item<N,M>
{
    static const std::uint32_t value = max<N, M>();
};



// ************************** //
// *** Max profit problem *** //
// ************************** //
// Max N-M, where N lies in front of M (opposite ordering to original problem)
//
// template<std::uint32_t N, std::uint32_t...Ns>
// struct max_profit
// {
//     static const std::uint32_t value = ;
// };



// ********************** //
// *** Reverse binary *** //
// ********************** //
template<bool...Bs>
struct reverse_boolean
{
};

template<bool B>
struct reverse_boolean<B>
{
    static const std::uint32_t value = B;
};

template<bool B, bool...Bs>
struct reverse_boolean<B, Bs...>
{
    static const std::uint32_t value = B + 2 * reverse_boolean<Bs...>::value;
};








