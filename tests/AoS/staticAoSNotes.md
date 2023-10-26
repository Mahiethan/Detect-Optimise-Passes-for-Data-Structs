# Static AoS representations in LLVM IR Assembly

Static AoS data structures are stored in the stack, which means the `alloca` instructino is used.

There are two ways static AoS are defined in C:

## First example

### The size of the array is predefined in a separate variable

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
## Second example

### The struct is defined using a `type_def`.

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

## Third example

### The size of the array is defined as a integer literal

```C
struct node array[6790]; 
```
LLVM IR representation:

```llvm
%3 = alloca [6790 x %struct.nodeOne], align 16
```
Note that if the size is 1 or 0, such as:

```C
struct node array[1];
struct node emptyArray[0];
```
CHECK: These are still AoS data structures, regardless of the size. Optimisations will still be made on `struct` datatype.

## What are not AoS data structures:

These are not AoS data structures:

```C
struct nodeOne x;
struct nodeTwo y;
struct nodeTwo z;
```

```C
int arrayOne[5];
```

```C
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