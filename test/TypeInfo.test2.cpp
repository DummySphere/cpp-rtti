#include "TypeInfo.test1.h"
#include <stdio.h>

namespace test
{
	void f(rtti::Identifiable *_identifiable)
	{
		printf("_identifiable: %p\n", (void *)_identifiable);
		printf("type name: %s\n", _identifiable->GetTypeName());
		printf("full type name: %s\n", _identifiable->GetTypeFullName());
		printf("type id: %#x\n", _identifiable->GetTypeId());
		
		A *a = _identifiable->Cast<A>();
		if(a != NULL)
			printf("Gimme a A: %d !\n", a->m_a);
		B *b = _identifiable->Cast<B>();
		if(b != NULL)
			printf("Gimme a B: %d !\n", b->m_b);
		C *c = _identifiable->Cast<C>();
		if(c != NULL)
			printf("Gimme a C: %d !\n", c->m_c);
		
		W *w = _identifiable->Cast<W>();
		if(w != NULL)
			printf("Gimme a W !\n");
		X *x = _identifiable->Cast<X>();
		if(x != NULL)
			printf("Gimme a X !\n");
		Y *y = _identifiable->Cast<Y>();
		if(y != NULL)
			printf("Gimme a Y !\n");
		Z *z = _identifiable->Cast<Z>();
		if(z != NULL)
			printf("Gimme a Z !\n");
		
		printf("\n");
	}
}

using namespace test;

int main()
{
	printf("Start test\n\n");
	
	A a;
	f(&a);
	
	C c;
	f(&c);
	
	D d;
	f(&d);
	
	X x;
	rtti::IdentifiablePtr identifiableX(&x);
	f(&identifiableX);
	
	Y y;
	rtti::IdentifiableRef<Y> identifiableY(y);
	f(&identifiableY);
	
	rtti::IdentifiableObject<Z> identifiableZ;
	f(&identifiableZ);

	T::U u;
	rtti::IdentifiableRef<T::U> identifiableU(u);
	f(&identifiableU);

	printf("End test\n");
	return 0;
}
