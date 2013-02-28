
all: TypeInfo.test.exe
	./TypeInfo.test.exe

TypeInfo.test.exe: TypeInfo.test1.cpp TypeInfo.test2.cpp TypeInfo.test1.h ../TypeInfo.h ../static_hash.h ../va_macro.h makefile
	g++ -I .. -E -o TypeInfo.test1.cc TypeInfo.test1.cpp
	g++ -I .. -Wall -Winline -O2 TypeInfo.test1.cpp TypeInfo.test2.cpp -o TypeInfo.test.exe
