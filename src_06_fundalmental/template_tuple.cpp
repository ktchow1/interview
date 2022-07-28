#include<iostream>
#include<vector>
#include"tuple.h"
#include<assert.h> // for assert


// ************ //
// *** Test *** //
// ************ //
void test_shuffle()
{
    using T   = std::tuple<char, std::uint32_t, std::string, std::pair<double,double>, double>;
    using X0  = shuffle_tuple <T,3,2,1>::type;
    using X1  = shuffle_tuple2<T,idx_seq<3,2,1>>::type;
    using ANS = std::tuple<std::pair<double,double>, std::string, std::uint32_t>;

    static_assert(std::is_same_v<X0, ANS>, "failed to shuffle_tuple");
    static_assert(std::is_same_v<X1, ANS>, "failed to shuffle_tuple");
    static_assert(std::is_same_v<std::tuple_size<T >::type, std::integral_constant<std::size_t,5>>, "failed to tuple_size");
    static_assert(std::is_same_v<std::tuple_size<X0>::type, std::integral_constant<std::size_t,3>>, "failed to tuple_size");
    static_assert(std::is_same_v<std::tuple_size<X1>::type, std::integral_constant<std::size_t,3>>, "failed to tuple_size");
    static_assert(std::tuple_size<T >::value == 5, "failed to tuple_size");
    static_assert(std::tuple_size<X0>::value == 3, "failed to tuple_size");
    static_assert(std::tuple_size<X1>::value == 3, "failed to tuple_size");

    // *** Factory *** //
    auto t  = T{'a', 12345, "wxyz", std::make_pair(3.1415, 1.4141), 0.98765};
    auto x2 = make_shuffle_tuple(t, std::index_sequence<3,2,1>{}); // remember to construct an object here

    static_assert(std::is_same_v<decltype(x2),ANS>, "failed to make_shuffle_tuple");
    assert(std::get<1>(t) == std::get<2>(x2));
    assert(std::get<2>(t) == std::get<1>(x2));
}

void test_append()
{
    using T    = std::tuple<char, std::uint32_t, std::string, std::pair<double,double>>;
    using X0   = append_tuple<T,  double>::type;
    using X1   = append_tuple<X0, double>::type;
    using ANS0 = std::tuple<char, std::uint32_t, std::string, std::pair<double,double>, double>;
    using ANS1 = std::tuple<char, std::uint32_t, std::string, std::pair<double,double>, double, double>;

    static_assert(std::is_same_v<X0, ANS0>, "failed to append_tuple");
    static_assert(std::is_same_v<X1, ANS1>, "failed to append_tuple");

    // *** Factory *** //
}

void test_reverse()
{
    using T   = std::tuple<char, std::uint32_t, std::string, std::pair<double,double>, std::vector<double>, double>;
    using X0  = reverse_tuple<T >::type;
    using X1  = reverse_tuple<X0>::type;
    using ANS = std::tuple<double, std::vector<double>, std::pair<double,double>, std::string, std::uint32_t, char>;
    static_assert(std::is_same_v<X0, ANS>, "failed to reverse_tuple");
    static_assert(std::is_same_v<X1, T>,   "failed to reverse_tuple");

    using X2 = reverse_tuple2<T >::type;
    using X3 = reverse_tuple2<X2>::type;
    static_assert(std::is_same_v<X2, ANS>, "failed to reverse_tuple2");
    static_assert(std::is_same_v<X3, T>,   "failed to reverse_tuple2");

    // *** Factory *** //
    auto  t  = T{'a', 12345, "wxyz", std::make_pair(3.1415, 1.4141), std::vector<double>{0.1,0.2,0.3,0.4}, 0.98765};
    auto  x4 = make_reverse_tuple_impl(t, std::index_sequence<0,1,3,4,5>{}); 
    auto  x5 = make_reverse_tuple(t); 
    using T4 = std::tuple<double, std::vector<double>, std::string, std::uint32_t, char>;
    using T5 = ANS;

    static_assert(std::is_same_v<T4, decltype(x4)>, "failed to make_shuffle_tuple");
    assert(std::get<0>(t) == std::get<4>(x4));
    assert(std::get<1>(t) == std::get<3>(x4));
    assert(std::get<2>(t) == std::get<2>(x4));

    static_assert(std::is_same_v<T5, decltype(x5)>, "failed to make_shuffle_tuple");
    assert(std::get<0>(t) == std::get<5>(x5));
    assert(std::get<1>(t) == std::get<4>(x5));
    assert(std::get<2>(t) == std::get<3>(x5));
}


void test_size_and_element()
{
    using T0 = std::tuple<char, std::uint32_t, std::string, std::pair<double,double>, double>;
    using T1 = std::tuple<char, std::uint32_t, std::string, std::pair<double,double>>;
    using T2 = tuple_size<T0>::type;
    using T3 = tuple_size<T1>::type;

    static_assert(std::is_same_v<T2, std::integral_constant<std::uint32_t,5>>, "failed to tuple_size");
    static_assert(std::is_same_v<T3, std::integral_constant<std::uint32_t,4>>, "failed to tuple_size");

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

void test_cat()
{
    using T0  = std::tuple<char, std::uint32_t, std::string, std::pair<double, double>>;
    using T1  = std::tuple<char, std::uint32_t, std::string>;
    using T2  = tuple_cat<T0,T1>::type;
    using ANS = std::tuple<char, std::uint32_t, std::string, std::pair<double, double>, char, std::uint32_t, std::string>;
    static_assert(std::is_same_v<T2, ANS>, "failed to tuple_cat");
}

void test_template_tuple()
{
    test_shuffle();
    test_append();
    test_reverse();
    test_size_and_element();
    test_cat();
}

