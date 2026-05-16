#include <iostream>
using namespace std;

struct Concept {
    virtual void operator()() = 0;
    virtual ~Concept() = default;
};


template <typename F>
struct Impl : Concept { // we inherit from the Concept class in order to use the operator() function
    F callable;  // the purpose is to store the function pointer
    Impl(F f): callable(std::move(f)) {} // callable is a function pointer
    //Impl is a constructor that takes a function pointer and stores it in the callable variable
    void operator()() override { callable();} // call the function
};


class function {
    Concept* ptr;

    public:
        function(F f): ptr(new Impl<F>(std::move(f))) {}
        ~function() { delete ptr;}

        void operator()() {(*ptr)();}
};


int main() {
    auto lambda = [] { std::cout << "Hello, World!" << std::endl; };
    void func() { std::puts("Hello, World!");}

    function f1 = lambda;
    function f2 = func;
    f1();
    f2();
}