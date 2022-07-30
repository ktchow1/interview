#include<iostream>
#include"idx_seq.h"


// ************ //
// *** Test *** //
// ************ //
void test_template_index_sequence()
{
    static_assert(std::is_same_v<    idx_seq_generator <5>::type, idx_seq<0,1,2,3,4>>, "failed to generate idx_seq");
    static_assert(std::is_same_v<inv_idx_seq_generator <5>::type, idx_seq<4,3,2,1,0>>, "failed to generate rev idx_seq");
    static_assert(std::is_same_v<alt_idx_seq_generator<10>::type, idx_seq<0,2,4,6,8>>, "failed to generate alt idx_seq");
    static_assert(std::is_same_v<alt_idx_seq_generator<11>::type, idx_seq<1,3,5,7,9>>, "failed to generate alt idx_seq");


    using T0 = push_front_idx_seq<11, idx_seq<0,1,2,3,5,7,9>>::type;
    static_assert(std::is_same_v<T0, idx_seq<11,0,1,2,3,5,7,9>>, "failed to push_front_idx_seq");

    using T1 = push_front_idx_seq<99, idx_seq<>>::type;
    static_assert(std::is_same_v<T1, idx_seq<99>>, "failed to push_front_idx_seq");

    using T2 = push_back_idx_seq<11, idx_seq<0,1,2,3,5,7,9>>::type;
    static_assert(std::is_same_v<T2, idx_seq<0,1,2,3,5,7,9,11>>, "failed to push_back_idx_seq");

    using T3 = push_back_idx_seq<99, idx_seq<>>::type;
    static_assert(std::is_same_v<T3, idx_seq<99>>, "failed to push_back_idx_seq");

    using T4 = reverse_idx_seq<idx_seq<1,2,3,4,5,6>>::type;
    static_assert(std::is_same_v<T4, idx_seq<6,5,4,3,2,1>>, "failed to reverse_idx_seq");

    using T5 = reverse_idx_seq<idx_seq<80,70,60,50,40,30,20,10>>::type;
    static_assert(std::is_same_v<T5, idx_seq<10,20,30,40,50,60,70,80>>, "failed to reverse_idx_seq");

    using T6 = filter_idx_seq<idx_seq<10,20,30,40,50,60,70,80,90>>::type;
    static_assert(std::is_same_v<T6, idx_seq<10,30,50,70,90>>, "failed to filter_idx_seq");

    using T7 = filter_idx_seq<idx_seq<10>>::type;
    static_assert(std::is_same_v<T7, idx_seq<10>>, "failed to filter_idx_seq");

    using T8 = dedupe_idx_seq<idx_seq<>>::type;
    static_assert(std::is_same_v<T8, idx_seq<>>, "failed to dedupe_idx_seq");

    using T9 = dedupe_idx_seq<idx_seq<1,1,1,1,1>>::type;
    static_assert(std::is_same_v<T9, idx_seq<1>>, "failed to dedupe_idx_seq");

    using T10 = dedupe_idx_seq<idx_seq<1,1,1,1,1,2,3,3,3,4>>::type;
    static_assert(std::is_same_v<T10, idx_seq<1,2,3,4>>, "failed to dedupe_idx_seq");

    using T11 = dedupe_idx_seq<idx_seq<1,2,3,4,5,6,7>>::type;
    static_assert(std::is_same_v<T11, idx_seq<1,2,3,4,5,6,7>>, "failed to dedupe_idx_seq");

    using T12 = dedupe_idx_seq<idx_seq<1,1,1,2,3,3,3,4,4,4,5,6,7,7,7>>::type;
    static_assert(std::is_same_v<T12, idx_seq<1,2,3,4,5,6,7>>, "failed to dedupe_idx_seq");
}

