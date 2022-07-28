#include<iostream>
#include<utility> // std::index_sequence
#include<tuple>
#include<assert.h> // for assert

// For index sequence, we can get reversed type :
//
// reverse_idx_seq<idx_seq<1,2,3,4,5>>::type == idx_seq<5,4,3,2,1>
//
// For tuple, since it has members, we can get reversed type AND reversed object :
// 
// reverse_tuple<std::tuple<A,B,C,D,E>>::type == std::tuple<E,D,C,B,A>    <--- reverse the type 
// reverse_tuple_object(t0) == t1                                         <--- reverse the type and the value <--- in fact this is easier, see remark
//
// where t0 is an object of std::tuple<A,B,C,D,E>
//       t1 is an object of std::tuple<E,D,C,B,A>
//
// Remark : reverse of tuple object is easier than reverse of tuple type, because :
// 1. there is an input object (like x below), we can operate with
// 2. there is a factory called std::make_tuple<>() which can resolve T... 
// 

  
template<typename T, std::size_t...Ns>
auto make_shuffle_tuple(const T& x, std::index_sequence<Ns...> dummy)
{
    return std::make_tuple(std::get<Ns>(x)...);
}

template<typename T, std::size_t...Ns>
auto make_reverse_tuple_impl(const T& x, std::index_sequence<Ns...> dummy)
{
    return std::make_tuple(std::get<std::tuple_size<T>::value-1-Ns>(x)...); // BUG : Don't forget minus one, otherwise it goes out of tuple range
}

template<typename T>
auto make_reverse_tuple(const T& x)
{
    return make_reverse_tuple_impl(x, std::make_index_sequence<std::tuple_size<T>::value>{}); 
}


// ****************************** //
// *** Tuple size and element *** //
// ****************************** //
template<typename T>
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

template<std::uint32_t N, typename T> // <--- interface
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

// Another approach 
template<std::uint32_t N, typename T> 
struct tuple_element2
{
    using type = std::remove_cvref_t<decltype(std::get<N>(std::declval<T>()))>;
//  using type =                     decltype(std::get<N>(std::declval<T>())); // BUG : Does not work !!!
};

// ***************** //
// *** Tuple cat *** //
// ***************** //


void test_template_tuple2()
{
    using T0 = std::tuple<char, std::uint32_t, std::string, std::pair<double,double>, double>;
    auto  x0 = T0{'a', 12345, "wxyz", std::make_pair(3.1415, 1.4141), 0.98765};
      

    // *** shuffle *** //
    auto  x1 = make_shuffle_tuple(x0, std::index_sequence<4,3,2,1>{}); // remember to construct an object here
    using T1 = std::tuple<double, std::pair<double,double>, std::string, std::uint32_t>;

    static_assert(std::is_same_v<T1,decltype(x1)>, "failed to make_shuffle_tuple");
    assert(std::get<1>(x0) == std::get<3>(x1));
    assert(std::get<2>(x0) == std::get<2>(x1));


    // *** reverse *** //
    auto  x2 = make_reverse_tuple_impl(x0, std::index_sequence<0,1,2,3,4>{}); 
    using T2 = std::tuple<double, std::pair<double,double>, std::string, std::uint32_t, char>;

    static_assert(std::is_same_v<T2,decltype(x2)>, "failed to make_shuffle_tuple");
    assert(std::get<0>(x0) == std::get<4>(x2));
    assert(std::get<1>(x0) == std::get<3>(x2));
    assert(std::get<2>(x0) == std::get<2>(x2));


    // *** reverse *** //
    auto  x3 = make_reverse_tuple(x0); 
    using T3 = std::tuple<double, std::pair<double,double>, std::string, std::uint32_t, char>;

    static_assert(std::is_same_v<T3,decltype(x3)>, "failed to make_shuffle_tuple");
    assert(std::get<0>(x0) == std::get<4>(x3));
    assert(std::get<1>(x0) == std::get<3>(x3));
    assert(std::get<2>(x0) == std::get<2>(x3));


    // *** tuple size and element *** //
    using T4 = tuple_size<T0>::type;
    using T5 = tuple_size<T1>::type;

    static_assert(std::is_same_v<T4, std::integral_constant<std::uint32_t,5>>, "failed to tuple_size");
    static_assert(std::is_same_v<T5, std::integral_constant<std::uint32_t,4>>, "failed to tuple_size");

    using X0 = tuple_element<0,T0>::type;
    using X1 = tuple_element<1,T0>::type;
    using X2 = tuple_element<2,T0>::type;
    using X3 = tuple_element<3,T0>::type;
    using X4 = tuple_element<4,T0>::type;

    static_assert(std::is_same_v<X0, char>,                     "failed to tuple_element");
    static_assert(std::is_same_v<X1, std::uint32_t>,            "failed to tuple_element");
    static_assert(std::is_same_v<X2, std::string>,              "failed to tuple_element");
    static_assert(std::is_same_v<X3, std::pair<double,double>>, "failed to tuple_element");
    static_assert(std::is_same_v<X4, double>,                   "failed to tuple_element");

    using Y0 = tuple_element2<0,T0>::type;
    using Y1 = tuple_element2<1,T0>::type;
    using Y2 = tuple_element2<2,T0>::type;
    using Y3 = tuple_element2<3,T0>::type;
    using Y4 = tuple_element2<4,T0>::type;

    static_assert(std::is_same_v<Y0, char>,                     "failed to tuple_element");
    static_assert(std::is_same_v<Y1, std::uint32_t>,            "failed to tuple_element");
    static_assert(std::is_same_v<Y2, std::string>,              "failed to tuple_element");
    static_assert(std::is_same_v<Y3, std::pair<double,double>>, "failed to tuple_element");
    static_assert(std::is_same_v<Y4, double>,                   "failed to tuple_element");
}
