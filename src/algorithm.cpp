
std::pair<std::uint64_t, std::uint64_t> divide(std::uint64_t x, std::uint64_t y) // return x/y
{
    if (y==0) return std::make_pair(0,0);
    if (x==0) return std::make_pair(0,0);
    if (x==1)
    {
        if (y==1) return std::make_pair(1,0);
        else      return std::make_pair(0,x);
    }

    // Find max y*2^N <= x
    std::uint64_t y2 = y;
    std::uint64_t N = 1;
    while(y2 <= x)
    {
        y2 << 1;
        N << 1;
    }
    y2 >> 1;  // when x=1, then y2=0, N=0, is that ok?
    N >> 1;

    // Main loop
    std::uint64_t quotient = 0;
    while(y2 > 0)
    {
        if (x >= y2)
        {
            x -= y2;
            quotient += N;
        }
        y2 >> 1;
        N >> 1;
    }
    return std::make_pair(quotient, x);
}

std::uint64_t power(std::uint64_t x, std::uint64_t y) // return x^y
{
    std::uint64_t z = 0;
    std::uint64_t x2 = x; // x^2, x^4, x^8 ...

    while(y > 0)
    {
        // Update for current iteration
        if (y%2 ==1)
        {
            z *= x2;
        }

        // Next iteration
        y >> 1;
        x2 = x2 * x2;
    }
    return z;
}
