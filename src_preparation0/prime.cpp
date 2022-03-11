#include<iostream>
#include<numeric>
#include<vector>
#include<math.h>

std::uint32_t num_of_prime(std::uint32_t N)
{
    std::vector<bool> is_prime(N+1, true);
    is_prime[0] = false;
    is_prime[1] = false;

    std::uint32_t sqrtN = static_cast<std::uint32_t>(sqrt(N))+1;
    for(std::uint32_t n=2; n<=sqrtN; ++n)
    {
        if (is_prime[n])
        {
            auto m = n+n;
            while(m<=N)
            {
                is_prime[m] = false;
                m += n;
            }
        }
    }
    std::cout << "\nN is prime? " << is_prime[N];
    return std::accumulate(is_prime.begin(), is_prime.end(), 0);
}


