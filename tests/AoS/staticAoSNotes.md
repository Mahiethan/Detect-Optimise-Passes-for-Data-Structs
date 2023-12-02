# Static AoS in LLVM IR Assembly

Static AoS data structures are stored in the stack, which means the `alloca` instructions is used.

## Detection 

Psuedocode:
```pseudo
void detectAoS(AllocaInst* v)

  get allocatedtype
  get allocated type as a string

  if it is an arrayallocation (of size greater than 0)
    check if allocatedtype is a struct
      static AoS found!
      store staticAoS in confirmed vector
  else if string contains 'struct' word inside '[ ]' - e.g. [50 x %struct.node]
    if size specified in string is not 0 e.g. [0 x ...]
      static AoS found!
      store staticAoS in confirmed vector
    endif
  endif
  
endif
```
## Representations in IR
 
There are three ways that static AoS are defined in C:

### First example

#### The size of the array is predefined in a separate variable

```C
int n = 67900
struct node array[n]; //n is a predefined variable
```

LLVM IR representation:

```llvm
%2 = alloca i32, align 4
store i32 697000, ptr %2, align 4 

%5 = load i32, ptr %2, align 4
%6 = zext i32 %5 to i64
%8 = alloca %struct.node, i64 %6, align 16
```

These instructions will be referred to as `%vla`, `%vla1`, etc. when using the `-fno-discard-value-names` flag with clang
### Second example

#### The struct is defined using a `type_def`.

Even when using the new name for the struct, it does not change how it is represented in the IR.

```C
typedef struct nodeOne
{
    int a;
    int b;
    char c;
}Node;
```
```C
int n = 697000; 
Node arrayOne[n]; //Node used instead of nodeOne
```
LLVM IR representation:

```llvm
%struct.nodeOne = type { i32, i32, i8 }
```
```llvm
%2 = alloca i32, align 4
store i32 697000, ptr %2, align 4

%5 = load i32, ptr %2, align 4
%6 = zext i32 %5 to i64
%8 = alloca %struct.nodeOne, i64 %6, align 16
```

### Third example

#### The size of the array is defined as a integer literal

```C
struct node array[6790]; 
```
LLVM IR representation:

```llvm
%3 = alloca [6790 x %struct.nodeOne], align 16
```
Note that if the size is 1, such as:

```C
struct node array[1];
```
These are still AoS data structures, as long as they are being used. If so, optimisations will still be made on `struct` datatype.

### Fourth example

#### Global variables

Accessing global AoS directly using GEP instructions:

```C
globalFive[1].a = 100;
```

```LLVM
store i32 100, ptr getelementptr inbounds ([100 x %struct.nodeOne], ptr @globalFive, i64 0, i64 1), align 16
```

In this case, need to find a `StoreInst` then check second operand and check its string. See if it contains 'getelementptr' and an AoS stored in potential, in this case: `@globalFive`

### What are not AoS data structures:

These are not AoS data structures:

```C
//even though these are allocas, they are not an array allocation
struct nodeOne x;
struct nodeTwo y;
struct nodeTwo z;
```

```C
//each element is not a `struct` type
int arrayOne[5];
```

```C
//element is not a `struct` type
int n = 5;
float array[n];
```

LLVM IR representations:

```llvm
%2 = alloca %struct.nodeOne, align 4
%3 = alloca %struct.nodeTwo, align 4
%4 = alloca %struct.nodeTwo, align 4
```
```llvm
%2 = alloca [5 x i32], align 16
```

```llvm
%2 = alloca i32, align 4
store i32 5, ptr %2, align 4

%5 = load i32, ptr %2, align 4
%6 = zext i32 %5 to i64
%8 = alloca float, i64 %6, align 16
```

i32 - 32-bit (4 byte) integer

i64 - 64-bit (8 byte) integer (float)