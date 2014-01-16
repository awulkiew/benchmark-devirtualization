## Devirtualization (benchmark)

Inspired by "Writing Quick Code in C++, Quickly" by Andrei Alexandrescu

http://channel9.msdn.com/Events/GoingNative/2013/Writing-Quick-Code-in-Cpp-Quickly

#### RESULTS

All builds with options -O2 -DNDEBUG

##### Machine 1 (Intel i7 870 Win7 SP1 x64)

Non-recursive calls

                      |          x86            |     x64
    ------------------+-------------------------+---------------
                      | VS2010  IC14   MinGW4.7 | VS2010  IC14
    ------------------+-------------------------+---------------
    virtual functions | 1.546   0.409  1.551    | 1.426   0.175
    vertical vtable   | 2.051   1.735  1.812    | 1.493   1.552
    vtable            | 2.061   1.776  1.897    | 1.535   1.594
    boost::variant    | 0.239   1.585  0.239    | 2.254   2.364

Recursive calls

                      |          x86            |     x64
    ------------------+-------------------------+---------------
                      | VS2010  IC14   MinGW4.7 | VS2010  IC14
    ------------------+-------------------------+---------------
    virtual functions | 1.455   1.292  1.525    | 1.001   1.404
    vertical vtable   | 1.700   1.506  1.360    | 1.154   1.178
    vtable            | 1.555   1.534  1.402    | 1.221   1.353
    boost::variant    | 1.914   2.443  2.625    | 2.107   2.024

##### Machine2 (Intel i5 3320M Linux Mint 15 x64)

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

##### Machine3 (Intel B960 Win8.1 x64)

Non-recursive calls

                      |       x86      |       x64
    ------------------+----------------+----------------
                      | VS2012  VS2013 | VS2012  VS2013
    ------------------+----------------+----------------
    virtual functions | 3.140   3.015  | 3.281   3.203
    vertical vtable   | 3.860   3.828  | 3.281   3.281
    vtable            | 3.781   3.751  | 3.281   3.188
    boost::variant    | 8.298   8.313  | 4.563   4.750

Recursive calls

                      |       x86      |       x64
    ------------------+----------------+----------------
                      | VS2012  VS2013 | VS2012  VS2013
    ------------------+----------------+----------------
    virtual functions | 2.328   2.328  | 2.031   2.000
    vertical vtable   | 3.062   2.406  | 2.282   2.516
    vtable            | 2.313   2.719  | 2.250   2.297
    boost::variant    | 8.375   8.657  | 3.109   3.344
    
