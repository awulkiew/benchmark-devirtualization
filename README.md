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
    ------------------+-------------------------+---------------
    fastest           | BV      VF     BV       | VF      VF
    slowest           | V       V      V        | BV      BV

Recursive calls

                      |          x86            |     x64
    ------------------+-------------------------+---------------
                      | VS2010  IC14   MinGW4.7 | VS2010  IC14
    ------------------+-------------------------+---------------
    virtual functions | 1.455   1.292  1.525    | 1.001   1.404
    vertical vtable   | 1.700   1.506  1.360    | 1.154   1.178
    vtable            | 1.555   1.534  1.402    | 1.221   1.353
    boost::variant    | 1.914   2.443  2.625    | 2.107   2.024
    ------------------+-------------------------+---------------
    fastest           | VF      VF     VV       | VF      VV
    slowest           | BV      BV     BV       | BV      BV

##### Machine2 (Intel i5 3320M Linux Mint 15 x64)

Non-recursive calls

                      |          x86         |         x64
    ------------------+----------------------+----------------------
                      | GCC4.7.3  clang3.2.1 | GCC4.7.3  clang3.2.1
    ------------------+----------------------+----------------------
    virtual functions |  1.72      1.64      |  1.47      1.47
    vertical vtable   |  1.66      1.72      |  1.53      1.54
    vtable            |  1.66      1.72      |  1.52      1.53
    boost::variant    |  0.12      0.00      |  0.12      0.00
    ------------------+----------------------+----------------------
    fastest           |  BV        BV        |  BV        BV
    slowest           |  VF        VV/V      |  VV        VV

Recursive calls

                      |          x86         |         x64
    ------------------+----------------------+----------------------
                      | GCC4.7.3  clang3.2.1 | GCC4.7.3  clang3.2.1
    ------------------+----------------------+----------------------
    virtual functions |  1.30       1.34     |  0.73       0.75
    vertical vtable   |  1.32       1.34     |  0.75       0.75
    vtable            |  1.48       1.40     |  0.90       0.89
    boost::variant    |  2.20       1.76     |  2.21       1.04
    ------------------+----------------------+----------------------
    fastest           |  VF         VF/VV    |  VF         VF/VV
    slowest           |  BV         BV       |  BV         BV
    

##### Machine3 (Intel B960 Win8.1 x64)

Non-recursive calls

                      |       x86      |       x64
    ------------------+----------------+----------------
                      | VS2012  VS2013 | VS2012  VS2013
    ------------------+----------------+----------------
    virtual functions | 1.921   1.844  | 1.688   1.656
    vertical vtable   | 2.704   2.718  | 2.297   2.297
    vtable            | 2.828   2.891  | 2.282   2.281
    boost::variant    | 0.266   0.282  | 0.125   0.125
    ------------------+----------------+----------------
    fastest           | BV      BV     | BV      BV
    slowest           | V       V      | VV      VV

Recursive calls

                      |       x86      |       x64
    ------------------+----------------+----------------
                      | VS2012  VS2013 | VS2012  VS2013
    ------------------+----------------+----------------
    virtual functions | 1.828   1.750  | 1.547   1.797
    vertical vtable   | 2.031   2.031  | 1.781   1.750
    vtable            | 2.110   2.125  | 1.766   1.797
    boost::variant    | 7.391   7.000  | 5.203   4.609
    ------------------+----------------+----------------
    fastest           | VF      VF     | VF      VV
    slowest           | BV      BV     | BV      BV
