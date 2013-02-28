#include "TypeInfo.h"
#include <stdio.h>

namespace test
{
	class W {};
	class X : public W {};
	class Y : public W {};
	class Z : public X, public Y {};
	
	class A : public rtti::Identifiable { public: DECL_IDENTIFIABLE() A(int _a = 0) : m_a(_a) {} int m_a; };
	class B : public A { public: DECL_IDENTIFIABLE() B(int _b = 10) : A(_b + 1), m_b(_b) {} int m_b; };
	class C : public B { public: DECL_IDENTIFIABLE() C(int _c = 100) : B(_c + 1), m_c(_c) {} int m_c; };
	class D : public rtti::Identifiable, public X, public Y { public: DECL_IDENTIFIABLE() D(int _d = 1000) : m_d(_d) {} int m_d; };

	class T { public: struct U { int a; }; };
}
