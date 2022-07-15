#include<iostream>
#include<vector>


//               fast    medium  slow    inplace  
// -----------------------------------------------------------
// select sort   O(N^2)  O(N^2)  O(N^2)
// bubble sort   O(N^2)  O(N^2)  O(N^2)
// insert sort   O(N)    O(N^2)  O(N^2)
// quick sort    O(lnN)  O(lnN)  O(N^2)
// merge sort    O(lnN)  O(lnN)  O(N^2)
// heap sort     O(lnN)  O(lnN)

template<typename ITER> void select_sort(ITER& begin, ITER& end)
{
    for(ITER i=begin; i!=end; ++i)
    {
        ITER j=i; 
        ITER max_iter = i;
        for(++j; j!=end; ++j)
        {
            if (*max_iter < *j)
                 max_iter =  j;
        }
        std::swap(*i, *max_iter);
    }
}

/*
template<typename ITER> void bubble_sort(ITER& begin, ITER& end)
{
}*/


