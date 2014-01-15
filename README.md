Devirtualization test

Inspired by "Writing Quick Code in C++, Quickly" by Andrei Alexandrescu

(http://channel9.msdn.com/Events/GoingNative/2013/Writing-Quick-Code-in-Cpp-Quickly)

RESULTS:

Machine 1 (Intel i7 870 Win7 SP1 x64)

VS2010 /O2 /DNDEBUG (x86)
virtual functions: 1.37
vertical vtable: 1.87
vtable: 1.88
boost::variant: 0.17

IC XE 14.0 /O2 /DNDEBUG (x86) (AVX, SSE4.2 SSSE3 doesn't matter)
virtual functions: 1.372
vertical vtable: 1.872
vtable: 1.872
boost::variant: 0.172

MinGW GCC 4.7.0 -02 -DNDEBUG (x86)
virtual functions: 1.416
vertical vtable: 1.71
vtable: 1.789
boost::variant: 0.119

VS2010 /O2 /DNDEBUG (x64)
virtual functions: 1.437
vertical vtable: 1.52
vtable: 1.512
boost::variant: 2.359

IC XE 14.0 /O2 /DNDEBUG (x64)
virtual functions: 1.341
vertical vtable: 1.404
vtable: 1.404
boost::variant: 2.2

Machine2 (Intel i5 3320M Linux Mint 15 x64)

GCC 4.7.3 -02 -DNDEBUG (x64)
virtual functions: 1.4
vertical vtable: 1.52
vtable: 1.6
boost::variant: 0.12

clang 3.2.1 -O2 -DNDEBUG (x64)
virtual functions: 1.47
vertical vtable: 1.53
vtable: 1.53
boost::variant: 0
