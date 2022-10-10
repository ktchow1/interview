#pragma once
#include<memory>
#include<string>

// ***************** //
// *** Objective *** //
// ***************** //
// Wrap irrelevant classes (classes NOT derived from same base) into the same wrapper, like std::any
// then invoke the shared interface via type_erase. The shared interface is specified in type_erase.
// Type_erase is a non-template class that can be constructed from different types, like std::any.
//
// 1. type_erase must be non-template, which housekeeps a non-template object_base pointer.
// 2. type_erase must have a template constructor, which allows construction from different types.
//
// Remark # : no these parts in shared_ptr_with_deleter
//
class type_erase0
{
    class object_base
    {
    public:
        virtual ~object_base() {};
        virtual std::uint32_t shared_interface(const std::string& str) const = 0;
    };

    template<typename T>
    class object_wrapper : public object_base
    {
    public:
        object_wrapper(const T& object) : _object(object) // Remark #
        {
        }

        std::uint32_t shared_interface(const std::string& str) const override
        {
            return _object.function(str);
        }
        
    private:
        T _object; // Remark #
    };

private:
    std::unique_ptr<object_base> base_ptr;

public:
    template<typename U>
    type_erase0(U&& targeted_object) : base_ptr(new object_wrapper<U>(std::forward<U>(targeted_object)))
    {
    }
    
    inline std::uint32_t shared_interface(const std::string& str) const 
    {
        return base_ptr->shared_interface(str);
    }
};

class type_erase1
{
    class object_base
    {
    public:
        virtual ~object_base() {};
        virtual std::uint32_t shared_interface(const std::string& str) const = 0;
    };

    template<typename T>
    class object_wrapper : public object_base
    {
    public:
        object_wrapper(const T& object) : _object(object) // Remark #
        {
        }

        std::uint32_t shared_interface(const std::string& str) const override
        {
            return _object(str); // Main difference between type_erase0 and type_erase1
        }

    private:
        T _object; // Remark #
    };

private:
    std::unique_ptr<object_base> base_ptr;

public:
    template<typename U>
    type_erase1(U&& targeted_object) : base_ptr(new object_wrapper<U>(std::forward<U>(targeted_object)))
    {
    }
    
    inline std::uint32_t shared_interface(const std::string& str) const 
    {
        return base_ptr->shared_interface(str);
    }
};

inline void invoke_shared_interface(const type_erase0& any)
{
    any.shared_interface("hello world");
}

inline void invoke_shared_interface(const type_erase1& any)
{
    any.shared_interface("byebye");
}

namespace test {
class A
{
public: 
    A(std::uint32_t x, std::uint32_t y) : _x(x), _y(y) {}
    inline std::uint32_t function  (const std::string& str) const { std::cout << "\nA::function "   << str; return 0; }
    inline std::uint32_t operator()(const std::string& str) const { std::cout << "\nA::operator() " << str; return 0; }

private:
    std::uint32_t _x;
    std::uint32_t _y;
};

class B
{
public: 
    B(std::uint32_t x, std::uint32_t y, std::uint32_t z) : _x(x), _y(y), _z(z) {}
    inline std::uint32_t function  (const std::string& str) const { std::cout << "\nB::function "   << str; return 0; }
    inline std::uint32_t operator()(const std::string& str) const { std::cout << "\nB::operator() " << str; return 0; }

private:
    std::uint32_t _x;
    std::uint32_t _y;
    std::uint32_t _z;
};

inline std::uint32_t f(const std::string& str)
{
    std::cout << "\nglobal f " << str;
    return 123;
}

inline void test_type_erasure()
{
    invoke_shared_interface(type_erase0(A{1,2}));
    invoke_shared_interface(type_erase0(B{1,2,3}));
    invoke_shared_interface(type_erase1(A{1,2}));
    invoke_shared_interface(type_erase1(B{1,2,3}));
    invoke_shared_interface(type_erase1(f));
    invoke_shared_interface(type_erase1(+[](const std::string& str)
    {
        std::cout << "\nlamba prefixed with + " << str; return 0; 
    }));
}
}



