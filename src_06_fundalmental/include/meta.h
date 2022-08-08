#include<iostream>



template<std::uint32_t N, std::uint32_t M>
consteval bool less_than() 
{
    return N<M;
}

template<std::uint32_t N, std::uint32_t M>
consteval std::uint32_t abs_diff() 
{
    if (less_than<N,M>()) return M-N; // if consteval is optional here ...
    else return N-M;
}

template<std::uint32_t N, std::uint32_t M, typename std::enable_if<less_than<N,M>(), int>::type dummy = 0>
consteval std::uint32_t abs_diff2()
{
    return M-N;
}

template<std::uint32_t N, std::uint32_t M, typename std::enable_if<!less_than<N,M>(), int>::type dummy = 0>
consteval std::uint32_t abs_diff2()
{
    return N-M;
};



// ************************ //
// *** Min/Max function *** //
// ************************ //
// BUG : using signed integer so that it supports negative profit in max-profit-problem
//
template<std::int32_t N, std::int32_t M>
consteval std::int32_t min() 
{
    if (N<M) return N;
    else return M;
}

template<std::int32_t N, std::int32_t M>
consteval std::int32_t max() 
{
    if (N>=M) return N;
    else return M;
}



// ******************************* //
// *** Min/Max variadic traits *** //
// ******************************* //
// BUG : using signed integer so that it supports negative profit in max-profit-problem
//
template<std::int32_t N, std::int32_t...Ns>
struct min_item
{
    static const std::int32_t value = min_item<N, min_item<Ns...>::value>::value;
};

template<std::int32_t N, std::int32_t M> // Why no ambiguity?
struct min_item<N,M>
{
    static const std::int32_t value = min<N, M>();
};

template<std::int32_t N, std::int32_t...Ns>
struct max_item
{
    static const std::int32_t value = max_item<N, max_item<Ns...>::value>::value;
};

template<std::int32_t N, std::int32_t M>
struct max_item<N,M>
{
    static const std::int32_t value = max<N, M>();
};



// ************************** //
// *** Max profit problem *** //
// ************************** //
// Max N-M, where N lies in front of M (opposite ordering to original problem)
// N0  = price at time t
// N1  = price at time t-1
//
// With template metaprogramming, we have to use recursive approach, hence duplicated calculation.
// 
template<std::int32_t N0, std::int32_t N1, std::int32_t...Ns>
struct max_profit_modified
{
    static const std::int32_t value = max_item
    <
        N0-N1, 
        N0-N1 + max_profit_modified<N1,Ns...>::value
    >
    ::value;
};

template<std::int32_t N0, std::int32_t N1>
struct max_profit_modified<N0,N1>
{
    static const std::int32_t value = N0-N1;
};

template<std::int32_t N, std::int32_t...Ns>
struct max_profit
{
    static const std::int32_t value = max_item
    <
        max_profit<Ns...>::value, 
        max_profit_modified<N,Ns...>::value
    >
    ::value;
};

template<std::int32_t N0, std::int32_t N1>
struct max_profit<N0,N1>
{
    static const std::int32_t value = max_profit_modified<N0,N1>::value;
};



// ********************** //
// *** Max subseq sum *** //
// ********************** //
template<std::int32_t N, std::int32_t...Ns>
struct max_subseq_sum_modified
{
    static const std::int32_t value = max_item
    <
        N, 
        N + max_subseq_sum_modified<Ns...>::value
    >
    ::value;
};

template<std::int32_t N>
struct max_subseq_sum_modified<N>
{
    static const std::int32_t value = N;
};

template<std::int32_t N, std::int32_t...Ns>
struct max_subseq_sum
{
    static const std::int32_t value = max_item
    <
        max_subseq_sum<Ns...>::value, 
        max_subseq_sum_modified<N,Ns...>::value
    >
    ::value;
};

template<std::int32_t N>
struct max_subseq_sum<N>
{
    static const std::int32_t value = max_subseq_sum_modified<N>::value;
};



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








