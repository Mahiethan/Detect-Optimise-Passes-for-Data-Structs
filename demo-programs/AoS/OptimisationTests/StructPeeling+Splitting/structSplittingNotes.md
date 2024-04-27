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

When splitting this struct, the `next` pointer field will always remain in the hot structure, regardless of whether it was used less frequently than the other 'hot' fields. Therefore, the recursive pointer field must be identified within a struct, if it exists, before it can be split. Since LLVM-17 now uses opaque (un-typed) pointers, there is no direct way to determine the type of a pointer, so we have to iterate through whole IR and look for GEP instructions that access the ptr field, and see whether an AoS in the 'confirmed' list or a struct has been stored in that field.

This optimisation only requires changes to the struct layout and changes to the accesses to the 'cold' fields in the code. They can be passed into functions as arguments without needing to change the function prototype, unlike struct peeling, therefore this optimisation can be applied to structs that are used as function arguments.

The main target data structures for struct splitting are:
1) AoS data structures used as function arguments
2) AoS data structures that uses a struct that contains a pointer field, which could make it a recursive struct.
3) Dynamic, locally declared AoS (declared using `malloc()/calloc()`)

In LLVM-IR, there is no reliable way to determine the type of a ptr. This means that it would be difficult to identify whether a struct contains a ptr to itself thus we cannot identify any recursive structs confidently. 

Therefore, it was decided that any structs that contain a ptr field element could possibly be recursive so struct splitting should be applied to it instead of struct peeling.

A disadvantage of this optimisation is that the size of the 'hot' data structure is increased by a pointer size [1], since the 'cold' fields are accessed using a pointer stored in the 'hot' struct. This mean that the performance improvement will come at a cost of increased memory consumption. 

It could be said that struct splitting is more superior to struct peeling, since it can be applied to all types structs, without any constraints. However, this optimisation is not as effective as struct peeling, due to the overhead caused by introducing pointers and the need to reference and dereference from it [1, pg 3]. Ultimately, it was decided to utilise both optimisations so the benefits of struct peeling can be seen as well as being able to make optimisations available to all types of AoS data structures using struct splitting.

## Additon of affinity groups

In some programs, splitting based on hotness (average field count) degrades runtime performance. This is because fields in structs are accessed together in certain blocks, and if these fields are not in the same struct, overheads are introduced by the additional ptr access to obtain these fields. Therefore, it is also worth it to split the structure based on affinity (how closely the fields are accessed in time) [1].

This is done by first getting the mean no. of accesses and setting the hot fields based on hotness (accesses > mean). Before creating the cold struct and setting its cold fields, the access patterns of each struct is obtained from each basic block, without storing duplicates. These basic blocks can be function bodies, for loop bodies, if statement bodies etc. These access patterns are stored as a vector of ints. Example shown below:

### Example of a block in a C program

```C
struct nodeOneOld
{
    int a;
    double b;
    int c;
    double d;
    char e;
    double f;
    double g;
    double h;
    double i;
    float j;
    struct nodeOneOld* next;
};

//...

for(int j = 0; j < 40; j++)
    {
        for(int i = 0; i < size; i++)
        {
            array[i].a = (array[i].b * array[i].a) + (100 / 5);
            array[i].b = (array[i].a / array[i].c) - (99 * 3);
        }
    }
```

The field access pattern vector for the above example is: {1,1,1,0,0,0,0,0,0,0,0}

This vector only stores a one or a zero value, based whether an access was detected or not detected respectively. 

The program will contain several of these *unique* field access patterns. If a vector of all ones was found, this means that all fields of the struct is used somewhere in the program. Since we are unable to count the frequency of these accesses based on loop iterations, due to difficulty running LoopAnalysis pass, we assume that the struct is fully used for most of the program's runtime i.e. assume that this block is in a for/while loop with a lot of iterations. This indicates that the struct has good affinity and there is no need to split it.

On the other hand, if such vector does not exist, we need to look out for fields that can be added to the hot struct. When analysing each vector, we make sure that at least one of the hot fields have their bit set to one (have been accessed in that block). With this condition met, we look for other bits that have their bit set to one. If the field represented by the bit is not already a hot field, this should be added as a *new* hot field.

For example, if the hot fields for the above example is fields, `int a` and `double b`, at least one of the hot fields are being accesed (in this case, both) in the vector. The third bit in the vector representing the cold field `int c` is set to one. This means that these three fields belong to the same affinity group and should belong in the same struct therefore,  `int c` should be added to the hot struct. 

Another example, with the vector: {0,0,0,1,1,1,0,0,0,0,0}, fields `double d`, `char e`, and `double f` belong to the same affinity group. However, in this block, none of the two hot fields, `int a` or `double b`, has been accessed. This means that it is perfectly fine for these three fields to be in the cold struct, where they belong in the same struct and affinity group.

# References

[1] https://www.capsl.udel.edu/conferences/open64/2008/Papers/111.pdf (Accessed 2/12/2023)

[2] https://llvm.org/devmtg/2014-10/Slides/Prashanth-DLO.pdf (Accessed 3/12/2023)

