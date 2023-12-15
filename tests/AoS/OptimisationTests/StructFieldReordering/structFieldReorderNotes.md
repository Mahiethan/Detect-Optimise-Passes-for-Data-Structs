# AoS Optimisation - Struct Field Reordering

The CPU accesses one word from memory per CPU cycle. The size of this word depends on the CPU architecture. For example, 32-bit CPUs have a word size of 4 bytes (32/8) and 64-bit CPUs have a word size of 8 bytes (64/8). Each address in memory is assigned to a word.

## Alignment

In C, data object types have a property called an alignment requirement, which defines the number of bytes between sucessive address (i.e. in a word) that objects of this type can be allocated [1]. Data types are aligned to ensure that they fit in a word, so they can be accessed within one CPU cycle. If they were misaligned, say an int of 4 bytes is has two bytes in one address and two bytes in another address, two CPU cycles are required to access both of these memory addresses to obtain the value of int type. Therefore, alignment is important to reduce CPU cycles required for memory accesses. This can be used to improve memory bound performance. 

## Padding

To meet the alignment requirements of fields inside structs, padding may be inserted after some of these fields [1], to ensure that they fit inside a word. In the example below:

```C
struct nodeOne //size 24 bytes
{
    /////////////////////// Word 0
    int a; // 4 bytes
    // 4 bytes padding
    /////////////////////// Word 1
    double b; // 8 bytes
    /////////////////////// Word 2
    char c; // 1 byte
    char d; // 1 byte
    // 6 bytes padding#
};
```
Add StructFieldOrdering-Unordered.png to Final Report!

In this `nodeOne` struct, the total size is 24 bytes, with included padding after `int a` and `char d` so that the all of these fields can be accessed within one memory word access. To access all elements of this struct, three CPU cycles are needed to access the data as three word chunks. 

Padding is also applied to the end of a struct, to make sure it is aligned itself, especially when it is an element of an Array of Structures.

The actual size of the struct, excluding the padding, is 14 bytes. With padding, there has been an increase of 10 bytes. Padding can improve performance at the cost of increased memory usage. This increase in memory usage seems insignificant, however for more of these struct objects, the increase in size caused by padding can be significant. 

For example, 10000 instances of this `nodeOne` struct will have 10000 bytes of extra memory usage.

This extra usage of memory can be reduced/removed entirely by re-ordering these struct fields. 

## Struct field reordering

The fields of the above struct `nodeOne` can be reordered as follows:

```C
struct nodeOneNew
{
    /////////////////////// Word 0
    double b; // 8 bytes
    /////////////////////// Word 1
    int a; // 4 bytes
    char c; // 1 byte
    char d; // 1 byte
    //// 2 bytes padding
};
```

Add StructFieldOrdering-Ordered.png to Final Report!

The fields can be re-ordered by decreasing alignment requirements. By doing this, there is less padding required after the fields to fill up the word line, thus less memory is wasted. 

In this case, the size of the struct is 16 bytes, where its total padding size is only 2 bytes; a reduction of 8 bytes (a single word). This example provides an additional benefit of only requiring two words to access the whole struct elements, instead of three words when the fields were unordered.

Note that struct field reordering is not possible with all structs with varying field types and quantites, as seen in this example.

```C
struct nodeTwoOld
{
   /////////////////////// Word 0
   int a;
   char b;
   //3 bytes padding
   /////////////////////// Word 1
   double c;
   /////////////////////// Word 2
   int d;
   //4 bytes padding
};


struct nodeTwoNew
{
    /////////////////////// Word 0
    double c;
    /////////////////////// Word 1
    int a;
    int d;
    /////////////////////// Word 2
    char b;
    //7 bytes padding
};

//Both structs (unordered and reordered) are still the same size with padding: 24 bytes
```

The compiler pass can make sure to check that this optimisation is profitable, by applying the reordering and checking whether the size of the struct has been reduced. If it has, the optimisation can be applied, otherwise it can be skipped. 

The C standard states that the addresses of elements increases in the order that they were defined. Certain C compilers conform to this standard and do not allow field re-ordering, only applying padding instead. 

This is because the programmer often uses the data layout they defined for the structure when writing the rest of the application [4]. By changing the ordering of the structure fields, the rest of the application code may not work as intended. Therefore, C compilers aim to follow the defined layout of structures, and only optimise it via padding. This means that the programmer is mainly responsible for making the struct reordering optimisation in their code.

But there are cases where the programmer may not have applied this optimisation to structs. This is where the compiler pass should apply this optimisation automatically and legally, by analysing whether this optimisation is profitable, then checking whether is is valid and can be applied without breaking the program functionality.

There are two legality checks that will be used to check whether optimisation to struct T can be made [4]:

1) If a cast to/from a pointer-to-T is found, optimisation is invalid
    - This means that the pointer data is manipulated and can be accessed in different ways.
2) If an address of a field of struct T is taken, optimisation is invalid
    - This indicates that address arithmetic has been applied on the data fields so changing the order of the fields may causes errors when accessing these addresses.

When running the `.c` files to compare populating and multiplying times between unorderedAoS and orderedAoS, time reduction was close to or at one second and memory usage saved by 16000KB, when running 200 times in a loop.

## Further optimisation

It was suggested that padding can be filled with fields if they can be fitted inside.

```C
struct nodeThreeOld
{
    /////////////////////// Word 0
    double a;
    /////////////////////// Word 1
    long long int b : 48; // 64-bit integer with bit-cast to 6 bytes (48 bits)
    // 2 byte padding
    /////////////////////// Word 2
    int c;
    int d;
    /////////////////////// Word 3
    char e;
    //7 bytes padding
};

// Total size: 32 bytes
```
If some 6 bytes field exists, the 2 byte padding can be filled with `char d`, which reduces the 8 bytes (word 3) at the end of the struct, and overall, reduces the number of words required to access the struct.

```C
struct nodeThreeNew
{
    /////////////////////// Word 0
    double a;
    /////////////////////// Word 1
    long long int b : 48; // 64-bit integer with bit-cast to 6 bytes (48 bits)
    char e;
    // 1 byte padding
    /////////////////////// Word 2
    int c;
    int d;
};

// Total size: 24 bytes
```

Even though this has reduced the size of the struct and possibly reduced the memory consumption of the program, this is *not* possible.

Each variable can be aligned to a power of 2 [5]. This means that struct fields are aligned to 2, 4 or 8 bytes depending on whether its size is 2, 4 or 8 bytes respectively. The above example makes use of bit-casts, which does use less bits, but still has the same alignment as a `long long int`, which is 64 bits (8 bytes). It is not possible to have a 6 byte alignment, as it is not a power-of-2. 

Since all fields are aligned to powers-of-2, the example above cannot be further optimised by adding fields in the padding. If the `long long int b` variable is replaced with three `short` variables, the fields will be ordered from highest size to lowest size to ensure that the struct size is reduced.

Run the script `runTestTwo.sh` to execute the `unorderedTwo.c` and `orderedTwo.c` files. There will be no significant difference in runtime and memory consumption when the `char e` field is moved, like shown above.

# References

[1] C Reference: Objects and alignment, https://en.cppreference.com/w/c/language/object (Accessed 28/11/2023)

[2] The Lost Art of Structure Packing, Eric S. Raymond, http://www.catb.org/esr/structure-packing/ (Accessed 29/11/2023)

[3] C Reference: Struct declaration, https://en.cppreference.com/w/c/language/struct (Accessed 29/11/2023)

[4] https://www.capsl.udel.edu/conferences/open64/2008/Papers/111.pdf (Refer correctly)

[5] https://stackoverflow.com/questions/24788262/why-alignment-is-power-of-2 (refer correctly)