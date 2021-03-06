#include <iostream>
#include <cstdio>
#include <vector>
#include "trampoline.h"
#include <cassert>
#include <functional>
#include <string>

using namespace std;

struct test_struct_1 {
    int operator()(int c1, int c2, int c3, int c4, int c5) {
        return 123;
    }
};

void test_base() {
    cout << "Test 0 started" << endl;
    test_struct_1 tmp;
    trampoline<int (int, int, int, int, int)> tr(tmp);
    int (*p)(int, int, int, int, int) = tr.get();
    assert (123 == p(1, 1, 1, 1, 1));
    cout << "Test 0 PASSED" << endl << endl;
}

void test_multipal_types_less_six_args() {

    cout << "Test 1 started" << endl;

    {
        trampoline<int (int, int, int, int, int)>
        t([&] (int p0, int p1, int p2, int p3, int p4) {return p1 + p2 + p3 + p4 + p0;});
        auto p = t.get();
        assert (5 == p(1, 1, 1, 1, 1));
        cout << " int test completed"  << endl;
    }
    {
        trampoline<double (double, double, double, double, double)>
        t([&] (double p0, double p1, double p2, double p3, double p4) {return p1 + p2 + p3 + p4 + p0;});
        auto p = t.get();
        assert (5.2 == p(1.0, 1.2, 1, 1, 1));
        cout << " double test completed"  << endl;
    }
    {
        trampoline<float (float, float, float, float, float)>
        t([&] (float p0, float p1, float p2, float p3, float p4) {return p1 + p2 + p3 + p4 + p0;});
        auto p = t.get();
        float res = 5.2;
        assert (res == p(1.0, 1.2, 1, 1, 1));
        cout << " float test completed"  << endl;
    }
    {
        trampoline<float (int, double, int, float, float)>
        t([&] (int p0, double p1, int p2, float p3, float p4) {return (p1 + p2 + p3 + p4 + p0);});
        auto p = t.get();
        float res = 5.2;
        assert (res == p(1.0, 1.2, 1, 1, 1));
        cout << " int/double/float test completed"  << endl;
    }
    {
        trampoline<int (int&)>
        t([&] (int p0) {return p0;});
        auto p = t.get();
        int a = 1;
        assert (1 == p(a));
        cout << " & test completed"  << endl;
    }
    cout << "Test 1 PASSED" << endl << endl;
}

void test_multipal_types_more_five_args_simple() {
    cout << "Test 2 started" << endl;
    {
        trampoline<long long (int, int, int, int, int, int, int, int)>
        t([&] (int p0, int p1, int p2, int p3, int p4, int p5, int p6, int p7)
        {return p1 + p2 + p3 + p4 + p0 + p5 + p6 + p7;});
        auto p = t.get();
        assert (8 == p(1, 1, 1, 1, 1, 1, 1, 1));
        cout << " int test completed"  << endl;

    }
    {
        trampoline<long long (double, double, double, double, double, double, double, double)>
        t([&] (double p0, double p1, double p2, double p3, double p4, double p5, double p6, double p7)
          {return p1 + p2 + p3 + p4 + p0 + p5 + p6 + p7;});
        auto p = t.get();
        assert (8 == p(1, 1, 1, 1, 1, 1, 1, 1));
        cout << " double test completed"  << endl;
    }
    {
        trampoline<long long (float, float, float, float, float, float, float, float)>
        t([&] (float p0, float p1, float p2, float p3, float p4, float p5, float p6, float p7)
          {return p1 + p2 + p3 + p4 + p0 + p5 + p6 + p7;});
        auto p = t.get();
        assert (8 == p(1, 1, 1, 1, 1, 1, 1, 1));
        cout << " float test completed"  << endl;
    }
    {
        trampoline<long long (double, int, float, int, int, double, double, float)>
        t([&] (double p0, int p1, float p2, int p3, int p4, double p5, double p6, float p7)
          {return p1 + p2 + p3 + p4 + p0 + p5 + p6 + p7;});
        auto p = t.get();
        assert (8 == p(1, 1, 1, 1, 1, 1, 1, 1));
        cout << " int/double/float test completed"  << endl;
    }
    {
        trampoline<long long (double&, int&, float&, int, int, double, double, float&)>
        t([&] (double p0, int p1, float p2, int p3, int p4, double p5, double p6, float p7)
          {return p1 + p2 + p3 + p4 + p0 + p5 + p6 + p7;});
        auto p = t.get();
        double a = 1;
        int b = 1;
        float c = 1, d = 1;
        assert (8 == p(a, b, c, 1, 1, 1, 1, d));
        cout << " &/int/double/float test completed"  << endl;
    }
    cout << "Test 2 PASSED" << endl << endl;
}

void test_multipal_types_more_five_args() {
    cout << "Test 3 started" << endl;
    {
        trampoline<float (double, int, float, int, int, double, double, float)>
        t1([&] (double p0, int p1, float p2, int p3, int p4, double p5, double p6, float p7)
          {return p1 + p2 + p3 + p4 + p0 + p5 + p6 + p7;});
        auto p1 = t1.get();

        trampoline<float (double, int, float, int, const int&, double&, double, float&)>
        t2([&] (double p0, int p1, float p2, int p3, int p4, double p5, double p6, float p7)
           {return p1 + p2 + p3 + p4 + p0 + p5 + p6 + p7;});
        const int a = 1;
        double b = 3.7;
        float c = 4.1;
        auto p2 = t2.get();
        assert ((float)(p1(1, 1, 1, 1, 1, 1, 1, 1) + 103.8) == p2(1, 2, 100, -1, a, b, 1, c));
        cout << " int/double/float test completed"  << endl;
    }
    {
        trampoline<float (double, int, float, int, int, double, double, float)>
        t([&] (double p0, int p1, float p2, int p3, int p4, double p5, double p6, float p7)
           {return p2;});
        auto p = t.get();
        assert(p(1, 2, 3, 4, 5, 6, 7, 8) == 3);
        cout<<" parms sequence test completed" << endl;
    }
    {
        trampoline<int (double, int, float, int, int, double, double, float)>
        t([&] (double p0, int p1, float p2, int p3, int p4, double p5, double p6, float p7)
           {return p7;});
        auto p = t.get();
        assert(p(1, 2, 3, 4, 5, 6, 7, 8.8) == 8);
        cout<<" return parm test completed" << endl;
    }
    {
        typedef std::string st;
        //function concatenates provided chars and returns a string with them
        std::function<int(st *c1, st *c2, st *c3, st *c4, st * c5, st *c6, st &c7, st *c8, st &c9)>
                fun = [](st *c1, st *c2, st  *c3, st *c4, st * c5, st *c6, st &c7, st *c8, st &c9)
        {return std::string("").append(*c1).append(*c2).append(*c3).append(*c4).append(*c5).append(*c6).append(c7).append(*c8).append(c9).size();};
        std::string sample = "abcababbaa";
        char const *sp = sample.c_str();
        std::vector<std::string> v;
        for(size_t i = 0; i < 9; i++) {
            v.push_back(std::string(sp + i));
        }
        int sz = fun(&v[0], &v[1], &v[2], &v[3], &v[4], &v[5], v[6], &v[7], v[8]);
        int tr_sz = trampoline<int(st *c1, st *c2, st *c3, st *c4, st *c5, st *c6, st& c7, st *c8, st& c9)>(fun)
                .get()(&v[0], &v[1], &v[2], &v[3], &v[4], &v[5], v[6], &v[7], v[8]);
        assert(sz == tr_sz);

    }
    cout << "Test 3 PASSED" << endl << endl;
}

void test_constructors()
{
    trampoline<int(int)> tr([](int a){return a * 2;});
    trampoline<int(int)> trr(std::move(tr));
}

int main()
{
    test_base();
    test_multipal_types_less_six_args();
    test_multipal_types_more_five_args_simple();
    test_multipal_types_more_five_args();
    test_constructors();
    return 0;
}
