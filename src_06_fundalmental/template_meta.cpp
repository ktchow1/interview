#include<iostream>
#include<meta.h>



void test_template_meta()
{

    static_assert(less_than<2,4>() == true);
    static_assert(less_than<8,4>() == false);
    static_assert(abs_diff<2,4>() == 2);
    static_assert(abs_diff<8,4>() == 4);
    static_assert(abs_diff2<200,400>() == 200);
    static_assert(abs_diff2<800,400>() == 400);
    static_assert(min<2,4>() == 2);
    static_assert(max<8,4>() == 8);

    static_assert(min_item<1,2>::value == 1);
    static_assert(min_item<2,3,4>::value == 2);
    static_assert(min_item<3,4,5,6>::value == 3);
    static_assert(min_item<4,5,6,7,8>::value == 4);
    static_assert(max_item<1,2>::value == 2);
    static_assert(max_item<2,3,4>::value == 4);
    static_assert(max_item<3,4,5,6>::value == 6);
    static_assert(max_item<4,5,6,7,8>::value == 8);

    static_assert(reverse_boolean<false>::value              == 0, "failed to reverse boolean");
    static_assert(reverse_boolean< true>::value              == 1, "failed to reverse boolean");
    static_assert(reverse_boolean<false, false, true>::value == 4, "failed to reverse boolean");
    static_assert(reverse_boolean<false,  true, true>::value == 6, "failed to reverse boolean");
    static_assert(reverse_boolean<true,   true, true>::value == 7, "failed to reverse boolean");
    static_assert(reverse_boolean<false, false, true, true>::value == 12, "failed to reverse boolean");

}