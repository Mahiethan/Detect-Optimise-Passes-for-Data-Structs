# AoS Optimisation - Struct Splitting

This is very similar to struct peeling, but it is only applied to recursive structs; these are structs that contain a pointer of the same type [1]. Struct peeling cannot be applied to recursive structs [2].

```C
//Unsplit struct
struct nodeOneOld
{
    //// Hot fields - most commonly used fields
    int a;
    double b;
    int c;
    //// Cold fields - least used fields
    double d;
    char e;
    double f;
    double g;
    double h;
    double i;
    float j;
    //pointer to same type
    struct nodeOneOld* next;
};
```

Now applying struct splitting to the above struct data structure:

```C
struct nodeOneCold
{
    //// Cold fields - least used fields
    double d;
    char e;
    double f;
    double g;
    double h;
    double i;
    float j;
};

struct nodeOneHot
{
    //// Hot fields - most commonly used fields
    int a;
    double b;
    int c;
    struct nodeOneCold* cold;
    //pointer to same type
    struct nodeOneHot* next;
};
```

This optimisation shares the same details and benefits of struct peeling (see notes on struct peeling). 

The only major difference is that the 'cold' struct is accessed using a pointer inside the 'hot' struct, instead of accessing it as a separate struct. There will be no overheads in accessing the 'hot' fields but there will be an increase in overheads when accessing the 'cold' fields since these fields are accessed via a pointer [1]. 

This optimisation only requires changes to the struct layout and changes to the accesses to the 'cold' fields in the code. They can be passed into functions as arguments without needing to change the function prototype, unlike struct peeling, therefore this optimisation can be applied to structs that are used as function arguments.

The main target data structures for struct splitting are:
1) AoS data structures used as function arguments
2) AoS data structures that uses a struct that contains a pointer field, which could make it a recursive struct.

In LLVM-IR, there is no reliable way to determine the type of a ptr. This means that it would be difficult to identify whether a struct contains a ptr to itself thus we cannot identify any recursive structs confidently. 

Therefore, it was decided that any structs that contain a ptr field element could possibly be recursive so struct splitting should be applied to it instead of struct peeling.

A disadvantage of this optimisation is that the size of the 'hot' data structure is increased by a pointer size [1], since the 'cold' fields are accessed using a pointer stored in the 'hot' struct. This mean that the performance improvement will come at a cost of increased memory consumption. 

# References

[1] https://www.capsl.udel.edu/conferences/open64/2008/Papers/111.pdf (Accessed 2/12/2023)

[2] https://llvm.org/devmtg/2014-10/Slides/Prashanth-DLO.pdf (Accessed 3/12/2023)

