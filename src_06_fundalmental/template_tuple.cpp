#include<iostream>
#include<vector>
#include"tuple.h"
#include<assert.h> // for assert


// ************ //
// *** Test *** //
// ************ //
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

    // *** tuple cat *** //
    using T6 = std::tuple<char, std::uint32_t, std::string, std::pair<double, double>>;
    using T7 = std::tuple<char, std::uint32_t, std::string>;
    using T8 = tuple_cat<T6,T7>::type;
    using T9 = std::tuple<char, std::uint32_t, std::string, std::pair<double, double>, char, std::uint32_t, std::string>;
    static_assert(std::is_same_v<T8,T9>, "failed to tuple_cat");

}
