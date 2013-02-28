cpp-rtti
========

C++ non-intrusive RTTI library.  
Allow to provide real-time type information for external types / from external libraries.  
Handle multiple inheritance.


* [Sources](#sources)
* [Documentation](#documentation)
* [License](#license)


Sources
-------

* _TypeInfo.h_: TypeInfo and Identifiable classes
* _static_hash.h_: Compile-time string hash
* _va_macro.h_: Preprocessor utilities for macros with variadic arguments


Documentation
-------------

In your .h:

	#include "TypeInfo.h"

	namespace example
	{
		class X {};
		class Y : public X {};
		class Z { public: struct S { ... }; };
		
		class A : public rtti::Identifiable { public: DECL_IDENTIFIABLE() ... };
		class B : public A { public: DECL_IDENTIFIABLE() ... };
	}
		
In your .cpp:

	#include "???.h"
	
	IMPL_TYPE_INFO(example, X)
	IMPL_TYPE_INFO(example, Y, example::X)
	IMPL_TYPE_INFO(example::Z, S)
	
	IMPL_IDENTIFIABLE(example, A)
	IMPL_IDENTIFIABLE(example, B, example::A)

Somewhere else:

	#include "???.h"
	
	void whatIsIt(rtti::Identifiable *_identifiable)
	{
		printf("type name: %s\n", _identifiable->GetTypeName());
		printf("full type name: %s\n", _identifiable->GetTypeFullName());
		printf("type id: %#x\n", _identifiable->GetTypeId());
		
		example::A *a = _identifiable->Cast<example::A>();
		if(a != NULL)
			printf("It is a A!\n");
	}
	
	void someTest()
	{
		example::Y y;
		rtti::IdentifiablePtr identifiableY(&y);
		whatIsIt(&identifiableY);
		
		example::B b;
		whatIsIt(&b);
	}

Please report bugs or suggestions ...


License
-------

Copyright (c) 2012-2013 Julien Duminil  
This project is released under the [MIT License](http://opensource.org/licenses/MIT).
