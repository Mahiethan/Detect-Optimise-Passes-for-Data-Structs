# AoS Optimisation - Struct Peeling

This involves splitting the struct fields into two: one with 'hot' (high affinity) fields and one with 'cold' (low affinity) fields [1].

The 'hotness'/affinity of a struct field is determined by:
- The number of accesses of a field
- If they are consistently accessed with other fields in the struct

For example, check the example below:

```C
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
};

struct nodeOneOld arrayOneOld[999999];
struct nodeOneOld arrayTwoOld[999999];


// void populateNodeOneOld(struct nodeOneOld array[], int size)
void populateNodeOneOld(struct nodeOneOld* array, int size)
{
    for(int i = 0; i < size; i++)
    {
        array[i].a = 1;
        array[i].b = 10.0;
        array[i].c = 9;
        array[i].d = 23.0;
        array[i].e = 'a';
        array[i].f = 23.0;
        array[i].g = 23.0;
        //array[i].b = array[i].a + array[i].b;
    }
}

void multNodeOneOld(struct nodeOneOld* array, int size)
{
    for(int j = 0; j < 30; j++)
    {
        for(int i = 0; i < size; i++)
        {
            array[i].a = (array[i].b * array[i].a) + (100 / 5);
            array[i].b = (array[i].a / array[i].c) - (99 * 3);
        }
    }
}

void multArrays(struct nodeOneOld* arrayOne, struct nodeOneOld* arrayTwo, int size)
{
    for(int j = 0; j < 30; j++)
    {
        for(int i = 0; i < size; i++)
        {
            arrayOne[i].a = arrayOne[i].a * arrayTwo[i].a;
            arrayOne[i].b = arrayOne[i].b * arrayTwo[i].b;
        }
    }
}
```

In the code excerpt above, the fields `a`, `b`, `c`, `d`, `e`, `f` and `g` are used, whilst the other fields are left unused.

Out of these used fields, `a`, `b` and `c` are used heavily and used together, giving them a high affinity, compared to `d`, `e`, `f` and `g` which is not used as much.

Therefore, `a`, `b` and `c` can be put into one 'hot' struct, since they are commonly used together and this will make better use of the cache, whereas `d`, `e`, `f`, `g`  and the other fields can be placed in a 'cold' struct.

```C
//Applying structure peeling
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
};
```
Fields `d`, `e`, `f` and `g` could be placed in the 'hot' structure, since they are used unlike `h`, `i` and `j`. However, this increases the size of the `hot` struct which means that for the functions `multNodeOneOld` and `multArrays` which benefit from this `hot` structure, there are more likey for cache misses to occur since the required data for `a`, `b` and `c` may not be present in the cache.

By reducing the struct to the most needed fields, it can also help reduce memory consumption too.

This optimisation is only beneficial if the field do not already have a good affinity. In this case, the original struct size is quite large, with most of it left mostly unused so struct peeling yields good reductions in runtime and memory consumption. This optimisation should only be done if struct field reordering is not possible in the first place[2].

If a struct contains a pointer to its type, then struct peeling is *not* viable. Struct splitting should be used instead [2].

## Limitations

We do not perform structure peeling if the structure is passed into a function as an argument or returned by an argument. 

This is because new parameters need to be introduced to the function prototypes to make use of the new 'hot' and 'cold' structs so the function protoype needs to be changed [2]. This could be done but it could break the functionality of the whole program, therefore to avoid this risk, AoS variables passed in as arguments are not applied this optimisation. This optimisation can still be applied to <u>global</u> AoS variables, which do not need to be passed into functions as arguments. 

Struct splitting can be used instead, which makes use of a pointer inside the struct so only accesses to cold fields need to be changed, but not sure if this going to be easily implemented.

The main target data structures for struct peeling are:
1) AoS data structures declared and used locally within a function body (not used as function arguments anywhere in the program)
2) Globally declared AoS data structures.

This means that if AoS 1 using struct 1 is used as a function argument, AoS 2 using struct 2 cannot be peeled.

First, see if an AoS can be peeled using the above criteria, if not, do struct splitting on that AoS.

Similar to the conditions for struct field reordering, if addresses or casting is used, this optimisation is invalidated.

# References

[1] https://www.capsl.udel.edu/conferences/open64/2008/Papers/111.pdf (Accessed 2/12/2023)

[2] https://llvm.org/devmtg/2014-10/Slides/Prashanth-DLO.pdf (Accessed 3/12/2023)