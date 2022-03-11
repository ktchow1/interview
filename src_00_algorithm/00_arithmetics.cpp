/*** Algorithm *** 
00. Arithmetic     6 
01. Bisection      4
02. Vector         6
03. Stack          4
04. List           4
05. Tree           6
06. Tree variant   4
07. Graph          2
08. Sorting        8
*/

namespace algorithm
{
    enum pillar { pillar_A, pillar_B, pillar_C };
    std::vector<pillar> Hanoi_tower(std::uint32_t N, const pillar& src, const pillar& des);

    std::uint32_t GCD(std::uint32_t x, std::uint32_t y);
    std::uint32_t LCM(std::uint32_t x, std::uint32_t y);

    std::string big_num_product(const std::string& x, const std::string& y);

    std::uint32_t reverse_digits(std::uint32_t x);
    std::uint32_t reverse_bits_1by1(std::uint32_t x);
    std::uint32_t reverse_bits_2by2(std::uint32_t x);
    std::uint32_t power_without_multiply(std::uint32_t x, std::uint32_t y);
    std::pair<std::uint32_t, std::uint32_t> divide_without_multiply(std::uint32_t x, std::uint32_t y);

    std::uint32_t next_integer(std::uint32_t x);

    std::uint32_t num_of_alphabet_encoding(std::uint32_t x);
}

namespace algorithm
{
    // From continuous to discrete
    // 1. no need to call fct
    // 2. various sortings      : increasing, uni-modal, cycle
    // 3. various objectives    : fct(x)=x, fct(x)=peak, fct(x)=0     
    // 4. maintain 3 conditions : xL, xU are not solution, xM!=xL, xM!=xU, there exists one solution in (xL,xU)
    // 5. update M  = (xL+xU)/2
    // 6. update xL = xM or xU = xM (not aggressively)  

    double bisection(const std::function<double()>& fct, double xL, double xU);
    std::uint32_t bisection_index(const std::vector<std::int32_t>& fct, std::uint32_t xL, std::uint32_t xU);
    std::uint32_t bisection_modal(const std::vector<std::int32_t>& fct, std::uint32_t xL, std::uint32_t xU);
    std::uint32_t bisection_cycle(const std::vector<std::int32_t>& fct, std::uint32_t xL, std::uint32_t xU);
}

namespace algorithm
{
    std::uint32_t max_binary_gap(std::uint32_t x);
    std::uint32_t count_passing_cars(std::vector<bool>& east_bounded_cars);

    std::uint32_t first_revisited_num(std::vector<std::uint32_t> x);
    std::uint32_t max_moving_average(const std::vector<std::uint32_t>& x);

    std::string reverse_words(const std::string& s);
    std::set<std::string> find_permutations(const std::string& s);
}

namespace algorithm
{
    template<typename T>
    class queue_with_2_stacks
    {
    public:
    private:
        std::stack<T> _stack0;
        std::stack<T> _stack1;
    };

    template<typename T>
    class stack_with_2_queues
    {
    public:
    private:
        std::queue<T> _queue0;
        std::queue<T> _queue1;
    };

    template<typename T>
    struct node
    {
        T value;
        node<T> next;
    };

    template<typename T>
    class stack
    {
    public:
        friend class stack_with_median<T>;

    private:
        node<T> _root;
    };

    template<typename T, std::uint32_t N>
    class mem_manager
    {
    public:
    private:
        node<T> _impl[N];
    };

    class stack_with_median
    {
    public:
        std::uint32_t median() const;

    private:
        std::stack<std::uint32_t> _impl; 
        std::uint32_t _median;
    };
}

namespace algorithm
{
    // 4 insert functions for singly-list
    // 4 insert functions for doubly-list
    // 2 delete functions 
    // 2 reverse functions
    class singly_list
    {
    public:
        void insert_before(const node<T>& this_node, const T& value);
        void insert_after(const node<T>& this_node const T& value);
        void push_begin(const T& value);
        void push_end(const T& value);
        void delete(node<T>& this_node);
        void reverse();

    private:
        node<T>* head;
    };

    class doubly_list
    {
    public:
        void insert_before(const node<T>& this_node, const T& value);
        void insert_after(const node<T>& this_node const T& value);
        void push_begin(const T& value);
        void push_end(const T& value);
        void delete(node<T>& this_node);
        void reverse();

    private:
        node<T>* head;
        node<T>* tail;
    };

    template<typename T>
    auto N_before_end(const std::list<T>& x, std::uint32_t N);

    template<typename T>
    bool intersect_lists(const std::list<T>& x, const std::list<T>& y);

    template<typename T>
    std::optional<std::pair<std::uint32_t, std::list<T>::const_iterator>> cycle_list(const std::list<T>& x); // return loop size and loop entry point
}

namespace algorithm
{
    // Tree
    // 1.
    // 2.
    // 3.
    // 4.
    // 5.
    // 6.

    template<typename T>

}


