#include<iostream>
#include<algorithm>
#include<string>
#include<unordered_map>

//   0 ->   0
//   1 ->   1
//   2 ->   1
//   9 ->   1
//  10 ->  10
//  11 ->  10
//  99 ->  10
// 100 -> 100
// 101 -> 100
int order(int n)
{
	int m = 1;
	while (n>=m) m *= 10;
	return m/10;
}

std::string int2str(int n)
{
	std::string str;		
	
	int m = order(n);	
	if (m==0) return "0";
	while(m > 0)
	{		
		str.push_back('0' + (n/m));
		n = n % m;
		m = m / 10;
	}
	return str;
}

void test_int2str()
{	
	for(int n=0; n!=100; ++n)
	{
		int m;
		int k = rand() % 5;
		if      (k == 0) m = rand() % 10;
		else if (k == 1) m = rand() % 100;
		else if (k == 2) m = rand() % 1000;
		else if (k == 3) m = rand() % 10000;
		else             m = rand() % 100000;

		if      (n == 95) m = 0;
		else if (n == 96) m = 10;
		else if (n == 97) m = 100;
		else if (n == 98) m = 1000;
		else if (n == 99) m = 10000;

		auto s = int2str(m);
		std::cout << "\n" << m << " " << s << " size = " << s.size();
	}
	std::cout << "\n\n";
}

inline int count_anagram(const std::string& s0, const std::string& s1)
{
	std::unordered_map<char, int> m0;
	std::unordered_map<char, int> m1;

	for (auto c : s0)
	{
		auto iter = m0.find(c);
		if (iter == m0.end()) m0[c] = 1;
		else ++iter->second;
	}
	for (auto c : s1)
	{
		auto iter = m1.find(c);
		if (iter == m1.end()) m1[c] = 1;
		else ++iter->second;
	}

	int common_count = 0;
	for (auto kv : m0)
	{
		auto iter = m1.find(kv.first);
		if (iter != m1.end()) common_count += std::min(kv.second, iter->second);
	}
	return common_count;
}

void test_anagram()
{
	std::cout << "\nAnagram = " << count_anagram("aaabbbbbbcccaaa", "abbbbbbaaaacccded");	
}
