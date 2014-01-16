#include <iostream>
#include <fstream>
#include <boost/timer.hpp>
#include <boost/cstdint.hpp>
#include <boost/variant.hpp>

// --------------------------------------------------

class B
{
public:
    virtual int get() = 0;
    virtual void set(int i) = 0;
    virtual void add() = 0;
    virtual int rec(int i) = 0;
};

template <boost::uint8_t Tag>
class D : public B
{
public:
    D(int i) : m_i(i) {}

private:
    int get() { return m_i; }
    void set(int i) { m_i = i; }
    void add() { m_i += Tag; }
    int rec(int i) { return i <= 0 ? 1 : m_i * ((B*)this)->rec(i-1); }

    int m_i;
};

// --------------------------------------------------

class VB
{
protected:
    typedef void (*fpointer)();    

    boost::uint8_t m_tag;
    static fpointer m_vtable[4][2];

    typedef int (*get_t)(VB &);
    typedef void (*set_t)(VB &, int);
    typedef void (*add_t)(VB &);
    typedef int (*rec_t)(VB &, int);

    inline VB(boost::uint8_t tag) : m_tag(tag) {}

public:
    inline int get()
    {
        return ((get_t)(m_vtable[0][m_tag]))(*this);
    }
    inline void set(int i)
    {
        ((set_t)(m_vtable[1][m_tag]))(*this, i);
    }
    inline void add()
    {
        ((add_t)(m_vtable[2][m_tag]))(*this);
    }

    inline int rec(int i)
    {
        return ((rec_t)(m_vtable[3][m_tag]))(*this, i);
    }
};

VB::fpointer VB::m_vtable[4][2];

template <boost::uint8_t Tag>
class VD : public VB
{
public:
    VD(int i) : VB(Tag), m_i(i)
    {
        ((get_t&)(m_vtable[0][m_tag])) = get_impl;
        ((set_t&)(m_vtable[1][m_tag])) = set_impl;
        ((add_t&)(m_vtable[2][m_tag])) = add_impl;
        ((rec_t&)(m_vtable[3][m_tag])) = rec_impl;
    }
    
private:
    static int get_impl(VB & b) { return ((VD&)(b)).m_i; }
    static void set_impl(VB & b, int i) { ((VD&)(b)).m_i = i; }
    static void add_impl(VB & b) { ((VD&)(b)).m_i += Tag; }
    static int rec_impl(VB & b, int i) { return i <= 0 ? 1 : ((VD&)(b)).m_i * b.rec(i-1); }

    int m_i;
};

// --------------------------------------------------

class VVB
{
protected:
    struct vtable_t
    {
        int (*get)(VVB &);
        void (*set)(VVB &, int);
        void (*add)(VVB &);
        int (*rec)(VVB &, int);
    };
    
    static vtable_t m_vtable[2];
    boost::uint8_t m_tag;

    inline VVB(boost::uint8_t tag) : m_tag(tag) {}

public:
    inline int get()
    {
        return (m_vtable[m_tag].get)(*this);
    }
    inline void set(int i)
    {
        (m_vtable[m_tag].set)(*this, i);
    }
    inline void add()
    {
        (m_vtable[m_tag].add)(*this);
    }
    inline int rec(int i)
    {
        return (m_vtable[m_tag].rec)(*this, i);
    }
};

VVB::vtable_t VVB::m_vtable[2];

template <boost::uint8_t Tag>
class VVD : public VVB
{
public:
    VVD(int i) : VVB(Tag), m_i(i)
    {
        m_vtable[m_tag].get = get_impl;
        m_vtable[m_tag].set = set_impl;
        m_vtable[m_tag].add = add_impl;
        m_vtable[m_tag].rec = rec_impl;
    }
    
private:
    static int get_impl(VVB & b) { return ((VVD&)(b)).m_i; }
    static void set_impl(VVB & b, int i) { ((VVD&)(b)).m_i = i; }
    static void add_impl(VVB & b) { ((VVD&)(b)).m_i += Tag; }
    static int rec_impl(VVB & b, int i) { return i <= 0 ? 1 : ((VVD&)(b)).m_i * b.rec(i-1); }

    int m_i;
};

// --------------------------------------------------

template <boost::uint8_t Tag>
class A
{
public:
    A(int i) : m_i(i) {}

    int get() { return m_i; }
    void set(int i) { m_i = i; }
    void add() { m_i += Tag; }

    int m_i;
};

typedef boost::variant< A<0>, A<1> > AA;

struct get_visitor : public boost::static_visitor<int>
{
    template <typename T>
    int operator()(T & t) const { return t.get(); }
};
struct set_visitor : public boost::static_visitor<>
{
    set_visitor(int i) : m_i(i) {}
    int m_i;
    template <typename T>
    void operator()(T & t) const { t.set(m_i); }
};
struct add_visitor : public boost::static_visitor<>
{
    template <typename T>
    void operator()(T & t) const { t.add(); }
};
struct rec_visitor : public boost::static_visitor<int>
{
    rec_visitor(int i, AA *v) : m_i(i), m_v(v) {}
    int m_i;
    AA * m_v;
    template <typename T>
    int operator()(T & t) {
        int i = m_i;
        m_i = m_i - 1;
        return i <= 0 ? 1 : t.m_i * boost::apply_visitor(*this, *m_v); }
};

// --------------------------------------------------

int main()
{
    unsigned long long N = 200000000;
    unsigned long long N2 = 50000000;
    int a = 1, b = 1;

    std::ifstream ifs("dummy");
    ifs >> a; ifs >> b; ifs >> N;
    std::ofstream ofs("dummy");
    extern int dummy;
    
    {
        D<0> d1(a);
        D<1> d2(b);
        B & b1 = d1;
        B & b2 = d2;

        int c = 0;
        boost::timer t;
        for ( unsigned long long i = 0 ; i < N ; ++i )
        {
            b1.add();
            b2.add();
            b1.set(b1.get() + b2.get());
        }
        double s = t.elapsed();
        std::cout << "virtual functions: " << s << std::endl;
        dummy = b1.get() + b2.get();
        ofs << dummy << std::endl;
    }
    
    {
        VD<0> d1(a);
        VD<1> d2(b);
	VB & b1 = d1;
	VB & b2 = d2;

        boost::timer t;
        for ( unsigned long long i = 0 ; i < N ; ++i )
        {
            b1.add();
            b2.add();
            b1.set(b1.get() + b2.get());
        }
        double s = t.elapsed();
        std::cout << "vertical vtable: " << s << std::endl;
        dummy = b1.get() + b2.get();
        ofs << dummy << std::endl;
    }

    {
        VVD<0> d1(a);
        VVD<1> d2(b);
	VVB & b1 = d1;
	VVB & b2 = d2;

        boost::timer t;
        for ( unsigned long long i = 0 ; i < N ; ++i )
        {
            b1.add();
            b2.add();
            b1.set(b1.get() + b2.get());
        }
        double s = t.elapsed();
        std::cout << "vtable: " << s << std::endl;
        dummy = b1.get() + b2.get();
        ofs << dummy << std::endl;
    }

    {
	    AA b1 = A<0>(a);
        AA b2 = A<1>(b);

        boost::timer t;
        for ( unsigned long long i = 0 ; i < N ; ++i )
        {
            boost::apply_visitor(add_visitor(), b1);
            boost::apply_visitor(add_visitor(), b2);
	    boost::apply_visitor(set_visitor(
                boost::apply_visitor(get_visitor(), b1) +
                boost::apply_visitor(get_visitor(), b2)
            ), b1);
        }
        double s = t.elapsed();
        std::cout << "boost::variant: " << s << std::endl;
        dummy = boost::apply_visitor(get_visitor(), b1) + boost::apply_visitor(get_visitor(), b2);
        ofs << dummy << std::endl;
    }

    std::cout << "\nrecursion\n" << std::endl;

    {
        D<0> d(a);
        B & b = d;

        dummy = 0;
        boost::timer t;
        for ( unsigned long long i = 0 ; i < N2 ; ++i )
        {
            dummy += b.rec(10);
        }
        double s = t.elapsed();
        std::cout << "virtual functions: " << s << std::endl;
        ofs << dummy << std::endl;
    }

    {
        VD<0> d(a);
        VB & b = d;

        dummy = 0;
        boost::timer t;
        for ( unsigned long long i = 0 ; i < N2 ; ++i )
        {
            dummy += b.rec(10);
        }
        double s = t.elapsed();
        std::cout << "vertical vtable: " << s << std::endl;
        ofs << dummy << std::endl;
    }

    {
        VVD<0> d(a);
        VVB & b = d;

        dummy = 0;
        boost::timer t;
        for ( unsigned long long i = 0 ; i < N2 ; ++i )
        {
            dummy += b.rec(10);
        }
        double s = t.elapsed();
        std::cout << "vtable: " << s << std::endl;
        ofs << dummy << std::endl;
    }

    {
        AA b = A<0>(a);

        dummy = 0;
        boost::timer t;
        for ( unsigned long long i = 0 ; i < N2 ; ++i )
        {
            rec_visitor v(10, &b);
            dummy += boost::apply_visitor(v, b);
        }
        double s = t.elapsed();
        std::cout << "boost::variant: " << s << std::endl;
        ofs << dummy << std::endl;
    }
    
    return 0;
}

int dummy = 0;
