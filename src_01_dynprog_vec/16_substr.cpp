#include<iostream>
#include<string>
#include<unordered_map>

std::uint32_t longest_non_duplicated_substr(const std::string& str)
{
    std::unordered_map<char, std::uint32_t> index; // can be replaced by std::array 

    std::uint32_t ans=0;
    for(std::uint32_t n=0; n!=str.size(); ++n) 
    {
        if (auto iter=index.find(str[n]); iter!=index.end())
        {
            ans = std::max(ans, n-iter->second);
        }
        index[str[n]] = n;
    }
    return ans;
}

// This is nearly a exhaustive search, but probably a fast one. There is no promising dynprog.
std::uint32_t longest_palindrome_substr(const std::string& str) 
{
    std::uint32_t ans=1;
    for(std::uint32_t n=1; n!=str.size()-1; ++n) // n = centre of palindrome
    {
        std::uint32_t m=1;
        while(true)
        {
            if (n<m || n+m>=str.size()) 
            {
                break;
            }
            else if (str[n-m]==str[n+m])
            {
                ans = std::max(ans, 2*m+1);
                ++m;
            }
            else
            {
                break;
            }
        }
    }
    return ans;
}

// ************ //
// *** Test *** //
// ************ //
std::string gen_random_substr(std::uint32_t length)
{
    std::string str;
    for(std::uint32_t n=0; n!=length; ++n)
    {
        str.push_back(static_cast<char>('k' + rand()%10));
    }
    return str;
}

std::string gen_palindrome_substr(std::uint32_t length_of_one_side)
{
    std::string str;
    str.resize(2*length_of_one_side+1);
    for(std::uint32_t n=0; n!=length_of_one_side; ++n)
    {
        str[n] = static_cast<char>('a' + rand()%5);
        str[2*length_of_one_side-n] = str[n];
    }
    str[length_of_one_side] = 'z';
    return str;
}

std::string gen_palindrome_input(std::uint32_t length_of_one_side)
{
    std::string str;
    std::uint32_t N0 = rand()%4;
    std::uint32_t N1 = rand()%4;
    for(std::uint32_t n=0; n!=N0; ++n)
    {
        str.append(gen_random_substr(rand()%10));
        str.append(gen_palindrome_substr(rand()%length_of_one_side));
    }
    str.append(gen_palindrome_substr(length_of_one_side));
    for(std::uint32_t n=0; n!=N1; ++n)
    {
        str.append(gen_random_substr(rand()%10));
        str.append(gen_palindrome_substr(rand()%length_of_one_side));
    }
    return str;
}

void test_longest_palindrome_substr()
{
    for(std::uint32_t n=0; n!=100; ++n)
    {
        std::uint32_t length_of_one_side = rand()%5 + 5;
        std::string   str = gen_palindrome_input(length_of_one_side);
        std::uint32_t ans = longest_palindrome_substr(str);
        std::cout << "\n" << 2*length_of_one_side+1
                  << ", " << ans
                  << ": " << str 
                  << (ans!=2*length_of_one_side+1? " [ERROR]":"");
    }
}

void test_substr()
{
    test_longest_palindrome_substr();
}
