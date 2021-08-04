#include<vector>


class interval_list_intersection 
{
public:
    std::vector<std::pair<int,int>> solve(std::vector<std::pair<int,int>>& list0, std::vector<std::pair<int,int>>& list1)
    {
        std::vector<std::pair<int,int>> output;
        auto iter0 = list0.begin();
        auto iter1 = list1.begin();
        while(iter0!=list0.end() && iter1!=list1.end())
        {
            if (is_strictly_less(*iter0,*iter1))
            {
                ++iter0;
            }
            else if (is_strictly_less(*iter1,*iter0))
            {
                ++iter1;
            }
            else
            {
                auto new_interval = overlap(*iter0,*iter1);                
                output.push_back(new_interval);

                if (iter0->second == new_interval.second) ++iter0;
                if (iter1->second == new_interval.second) ++iter1;
            }
        }
        return output;
    }
    
private:
    bool is_strictly_less(const std::pair<int,int>& interval0, const std::pair<int,int>& interval1)
    {
        if (interval0.second < interval1.first) return true;
        return false;
    }
    
    std::pair<int,int> overlap(const std::pair<int,int>& interval0, const std::pair<int,int>& interval1)
    {
        std::pair<int,int> output;
        output.first = std::max(interval0.first, interval1.first); 
        output.second = std::min(interval0.second, interval1.second); 
        return output;
    }
};

class interval_list_union
{
public:
    std::vector<std::pair<int,int>> solve(std::vector<std::pair<int,int>>& intervals, std::pair<int,int>& new_interval)
    {   	
        bool new_interval_added = false;
        auto iter = intervals.begin();
        while(iter!=intervals.end() && !new_interval_added)
        {
            if (is_less(*iter, new_interval))
            {
                output_push_back(*iter);
                ++iter;
            }
            else
            {
                output_push_back(new_interval);
                new_interval_added = true;
            }
        }

        while(iter!=intervals.end())
        {
            output_push_back(*iter);
            ++iter;
        }
        if (!new_interval_added)
        {
            output_push_back(new_interval);
            new_interval_added = true;
        }
        return output;
    }
    
private:
    bool is_less(const std::pair<int,int>& interval0, const std::pair<int,int>& interval1)
    {
        if (interval0.first < interval1.first) return true;
        return false;
    }

    void output_push_back(const std::pair<int,int>& new_interval)
    {
        if (output.size()==0)
        {
            output.push_back(new_interval);
        }
        else if (output.back().second < new_interval.first)
        {
            output.push_back(new_interval);
        }
        else if (output.back().second < new_interval.second)
        {
            output.back().second = new_interval.second; 
        } 
    }
    
    std::vector<std::pair<int,int>> output;
};






