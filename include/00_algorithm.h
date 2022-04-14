#pragma once

// ***************** //
// *** Algorithm *** //
// ***************** //
// 64644 + 6428 
// ----------------- //
// 0.0 Arithmetic     6 
// 0.1 Bisection      4
// 0.2 Vector         6
// 0.3 Stack          4
// 0.4 List           4
// 0.5 Tree           6
// 0.6 Tree variant   4
// 0.7 Graph          2
// 0.8 Sorting        8
// ----------------- //

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
    // 1. tree two definitions 
    // 2. binary tree 
    // 3. binary search tree 
    // 4. balanced binary tree 
    //
    // 1. depth / find / insert / traverse / rotate / balance
    // 2. definition of traverse : 
    //
    //    BFS                 no recursive     iterative by queue
    //    DFS (pre-order)        recursive     iterative by stack
    //    DFS (in-order)         recursive     iterative with stack inside iterator
    //    DFS (post-order)       recursive     complicated 
    //
    // 3. definition of rotation 
    // 4. definition of balance

    template<typename T> 
    std::uint32_t tree_depth(const node<T>* this_node);

    template<typename T> 
    node<T>* tree_find(const node<T>* this_node, const T& x);

    template<typename T> 
    node<T>* tree_insert(const node<T>* this_node, const T& x);

    enum DFS_type
    {
        in_order,
        pre_order,
        post_order
    };

    template<typename T> 
    void tree_DFS_recursive(const T& x, std::function<void(const T&)>& fct, DFS_Type type);
    template<typename T> 
    void tree_BFS_iterative(const T& x, std::function<void(const T&)>& fct);
    template<typename T> 
    void tree_DFS_pre_order_iterative(const T& x, std::function<void(const T&)>& fct);
    template<typename T> 
    void tree_DFS_in_order_iterative(const T& x, std::function<void(const T&)>& fct);

    template<typename T> 
    node<T>* tree_rotate_LHS(const node<T>* this_node); // return new root
    template<typename T> 
    node<T>* tree_rotate_RHS(const node<T>* this_node); // return new root

    template<typename T> 
    node<T>* tree_balance(const node<T>* this_node); // return new root

    // 4 CAFG questions
    template<typename T>
    bool check_post_order(const typename std::vector<T>::iterator& begin,
                          const typename std::vector<T>::iterator& end);

    template<typename T>
    bool check_tree_sorted(const node<T>* this_node);

    template<typename T>
    node<T>* create_tree_from_vector(const typename std::vector<T>::iterator begin,
                                     const typename std::vector<T>::iterator end);

    template<typename T>
    std::pair<node<T>*, node<T>*> doubly_list_from_tree(const node<T>* this_node);
}

// 2. B-tree 
// 3. skip-list 
// 4. trie-tree

namespace algorithm
{
    template<typename T>
    class heap
    {
    public:

    private:
    };
}

// 1. Topological sorting 
// 2. Dijkstra algorithm 

namespace algorithm
{

//                  best      aver      worst     inplace  stable  iterator
// --------------------------------------------------------------------------------
//  select          O(N^2)    O(N^2)    O(N^2)    yes      no      forward
//  bubble          O(N)      O(N^2)    O(N^2)    yes      yes     bi-directional
//  insert          O(N)      O(N^2)    O(N^2)    yes      yes     bi-directional
//  quick           O(NlogN)  O(NlogN)  O(N^2)    yes      no      bi-directional
//  merge           O(NlogN)  O(NlogN)  O(NlogN)  no       yes     random access
//  heap            O(NlogN)  O(NlogN)  O(NlogN)  yes      no      random access
//  pigeon          O(N)      O(N)      O(N)      no       yes     forward
//  topological

    template<typename ITER>
    void sort(ITER begin, ITER end);
}

