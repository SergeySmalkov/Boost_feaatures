#include <cstdint>
#include <memory>
#include <iostream>
#include <boost/intrusive_ptr.hpp>

static char cache[32768];
static char* avail = cache;

struct Object {
    void* operator new( size_t size ) {
        void* ptr = avail;
        avail += size;
        std::cout << "Created object " << ptr << " size " << size << std::endl;
        return ptr;
    }
    void operator delete( void* ptr ) {
        std::cout << "Delete object " << ptr << std::endl;
    }
private:
    friend void intrusive_ptr_add_ref( Object* x) {
        ++x->count;
    }
    friend void intrusive_ptr_release( Object* x) {
        if (--x->count == 0) delete x;
    }
    int count = 0;
};

struct X : public Object {
    X(int a_) : a(a_) {}
    int a;
};

struct Y : public X {
    Y(int a_, int b_) : X(a_), b(b_) {}
    int b;
};

int main(int argc, char** argv) {
    boost::intrusive_ptr<X> x = new X(1);
    boost::intrusive_ptr<Y> y = new Y(3,4);
    std::cout << "x1: " << x->a << " " << std::endl;
    std::cout << "x2: " << y->a << " " << y->b << std::endl;
}
