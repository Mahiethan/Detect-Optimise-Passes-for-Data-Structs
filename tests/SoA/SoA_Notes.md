## How to detect SoA?

1. First get all named structs and iterate through them

2. For each struct, check if it contains an static array

- if a field is of aggregate type, if size > 1, struct of static arrays found, add to 'list to find'

3. iterate through all code and find struct values/pointers that use these structs, then add to 'confirmed'

- iterate through global variables/pointers too

- if pointer of struct type, add to potential
    if potential SoA malloced/calloced to size 1 of element struct, this counts as a SoA (add to confirmed), if size > 1, it is a AoSoA

- check function calls which may create its own SoA

## How to define an SoA?

### 1. Declaration of local struct variable (inside function body)

```C
struct StructureOne s1;
```

```llvm
%s1 = alloca %struct.StructureOne, align 4
```

### 2. Declaration of global struct variable (outside function body)

```C
struct StructureOne g1;
```

```llvm
@g1 = dso_local global %struct.StructureOne zeroinitializer, align 4
```

### 3. Declaration of local struct pointer (inside function body)

```C
struct StructureOne* s1 = (struct StructureOne*) malloc(sizeof(struct StructureOne));
```

```llvm
%s1 = alloca ptr, align 8
...
%call = call noalias ptr @malloc(i64 noundef 1500000) #4
store ptr %call, ptr %s1, align 8
```

For some reason, the allocation operand is appearing as 1500000 - this is probably the size of the struct

Probably have to compare this value with the size of each struct in "toFind" list in order to detect what SoA was used, if any.

### 4. Declaration of global struct pointer (outside function body)

```C
struct StructureOne* gs1;
...
//inside function body
gs1 = (struct StructureOne*) malloc(sizeof(struct StructureOne));
```

```llvm
%call = call noalias ptr @malloc(i64 noundef 1500000) #5
store ptr %call, ptr @gs1, align 8
```

To find 3. and 4., use same method:

1. Find a malloc function call
2. Set mallocFlag to true to indicate that the next instruction is a StoreInst
3. Get the second operand of this instruction
4. Check if is a potential global SoA pointer, if it is, add to confirmed, otherwise, add to potential
5. Get all function calls in main function and check each function to see if a GEP is found


## How to define an AoSoA?

```C
struct StructureOne* s1 = (struct StructureOne*) malloc(10 * sizeof(struct StructureOne)); //AoSoA of size 10
```

```llvm
%s1 = alloca ptr, align 8
...
%call = call noalias ptr @malloc(i64 noundef 15000000) #5
store ptr %call, ptr %s1, align 8
```

Global AoSoA can also be defined in a similar way:

```C
struct StructureOne* gs2;
...
gs2 = (struct StructureOne*) malloc(10 * sizeof(struct StructureOne)); //global AoSoA of size 10
```

```llvm
@gs2 = dso_local global ptr null, align 8
...
%call1 = call noalias ptr @malloc(i64 noundef 15000000) #5
store ptr %call1, ptr @gs2, align 8
```

The allocation size here is 1500000 x 10 = 15000000
