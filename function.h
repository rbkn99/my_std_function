//
// Created by rbkn99 on 19.01.19.
//

#ifndef MY_STD_FUNCTION_FUNCTION_H
#define MY_STD_FUNCTION_FUNCTION_H

#include <memory>
#include <cassert>
#include <iostream>

template<typename T>
class function;

template<typename R, typename ... Args>
class function<R(Args ...)> {
public:
    function() noexcept : is_small(false), p(nullptr) {
        std::cout << "in empty constructor" << std::endl;
    }

    function(std::nullptr_t) noexcept : is_small(false), p(nullptr) {
        std::cout << "in nullptr constructor" << std::endl;
    }

    function(const function &other) : is_small(other.is_small) {
        std::cout << "in copy constructor" << std::endl;
        if (is_small) {
            reinterpret_cast<base*>((char*)other.small_obj)->clone_small(small_obj);
        }
        else {
            p = other.p->clone();
        }
    }

    function(function &&other) noexcept: is_small(false) {
        std::cout << "in move constructor" << std::endl;
        new(small_obj) std::unique_ptr<base>(nullptr);
        swap(other);
    }

    template<class F>
    function(F f) {
        std::cout << "in constructor(F)" << std::endl;
        is_small = (sizeof(F) <= SMALL_OBJECT_SIZE);
        if (is_small) {
            new(small_obj) model<F>(f);
        } else {
            new (small_obj) std::unique_ptr<model<F>>(new model<F>(f));
        }
    }

    ~function() {
        std::cout << "in destructor" << std::endl;
        if (is_small) {
            reinterpret_cast<base*>(small_obj)->~base();
        }
        else {
            p.reset();
        }
    }

    function &operator=(const function &other) {
        std::cout << "in const assignment" << std::endl;
        function temp(other);
        swap(temp);
        return *this;
    }

    function &operator=(function &&other) noexcept {
        std::cout << "in move assignment" << std::endl;
        swap(other);
        return *this;
    }

    void swap(function &other) noexcept {
        std::cout << "in swap" << std::endl;
        std::swap(is_small, other.is_small);
        std::swap(small_obj, other.small_obj);
    }

    explicit operator bool() const noexcept {
        std::cout << "in bool operator" << std::endl;
        return is_small || static_cast<bool>(p);
    }

    R operator()(Args ... args) {
        std::cout << "in operator()" << std::endl;
        if (is_small) {
            return reinterpret_cast<base*>(small_obj)->invoke(args ...);
        }
        assert(p && "Error, callable function is null");
        return p->invoke(args ...);
    }

private:
    class base {
    public:
        base() noexcept = default;

        virtual ~base() = default;

        virtual R invoke(Args ... args) = 0;

        virtual std::unique_ptr<base> clone() = 0;

        virtual void clone_small(void* destination) = 0;


        base(const base &) = delete;

        void operator=(const base &) = delete;
    };

    typedef std::unique_ptr<base> f_pointer_t;

    template<typename FunctionT>
    class model : public base {
    public:
        explicit model(FunctionT func) : base(), func(func) {}

        virtual R invoke(Args ... args) {
            return func(args ...);
        }

        virtual f_pointer_t clone() {
            return f_pointer_t(new model<FunctionT>(func));
        }

        virtual void clone_small(void* destination) {
            new(destination) model<FunctionT>(func);
        }

    private:
        FunctionT func;
    };

    static const size_t SMALL_OBJECT_SIZE = 32;

    //std::aligned_storage<SMALL_OBJECT_SIZE, sizeof(void*)> data;

    bool is_small;

    union {
        f_pointer_t p;
        char small_obj[SMALL_OBJECT_SIZE];
    };
};


#endif //MY_STD_FUNCTION_FUNCTION_H
