// ******************************** //
// *** This file is not tested. *** //
// ******************************** //
template<typename ITER>
void select_sort(ITER begin, ITER end)
{
    for(ITER i=begin; i!=end; ++i)
    {
        ITER win = i;
        for(ITER j=i+1; j!=end; ++j) 
        {
            if (*j < *WIN) WIN = j;
        }
        std::swap(*i, *win);
    }
}

template<typename ITER>
void bubble_sort(ITER begin, ITER end)
{
    for(ITER i=begin; i!=end; ++i)
    {
        for(ITER j=end-1; j!=i; --j)
        {
            if (*j < *(j-1)) std::swap(*j, *(j-1));
        }
    }
}

template<typename ITER>
void insert_sort(ITER begin, ITER end)
{
    for(ITER i=begin+1; i!=end; ++i)
    {
        for(ITER j=i; j!=0; --j) 
        {
            if (*j < *(j-1)) std::swap(*j, *(j-1));
            else break;
        }
    }
}

template<typename ITER>
void quick_sort(ITER begin, ITER end)
{
    ITER i = begin;
    ITER j = end-1;

    while(i!=j)
    {
        if (*i <= *j)
        {
            --j;
        }
        else 
        {
            typename std::iterator_traits<ITER>::value_type temp = *i;
            *i = *j;
            *j = *(i+1);
            *(i+1) = temp;
            ++i;
        }
    }
    quick_sort(begin, i);
    quick_sort(i+1, end);
}

template<typename ITER>
void merge_sort(ITER begin, ITER end)
{
    if (begin   == end) return;
    if (begin+1 == end) return;
    ITER mid = (begin + end)/2;
    std::vector<typename std::iterator_traits<ITER>::value_type> aux;

    std::copy(begin, mid, std::back_inserter(aux));
    merge_sort(aux.begin(), aux.end());
    merge_sort(mid, end);
    std::merge(aux.begin(), aux.end(), mid, end, begin);
}

template<typename ITER, typename BACK_ITER>
void merge(ITER begin0, ITER end0, ITER begin1, ITER end1, BACK_ITER out)
{
    while(begin0!=end0 && begin1!=end0)
    {
        if(*begin0 < *begin1) { *out = *begin0; ++begin0; ++out; }
        else                  { *out = *begin1; ++begin1; ++out; }
    }
    while(begin0 != end0) { *out = *begin0; ++begin0; ++out; }
    while(begin1 != end1) { *out = *begin1; ++begin1; ++out; }
}
