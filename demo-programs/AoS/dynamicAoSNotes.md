# Dynamic AoS in LLVM IR

## Detection

### <u>Steps for detection:</u>

Dynamic AoS data structures are pointers, meaning that they are malloced or calloced. There should be a method to differentiate a pointer with struct elements, against other pointers of different types (such as ints or floats).

#### A. Check for potential AoS

First, we will search for `malloc` in `main()`:

1. Look for a `call` instruction in the function
2. Check if it is a `malloc` 
3. Look for a `store` call, <i>following</i> a `malloc`, and get the second operand (the pointer being stored into)
4. Save this pointer value in `potential` vector for inspection.

A flag will be used to check the correct `store` instruction that follows the `malloc`. This saves time by not checking all `store` instructions in the function. Now we need to search through functions that take in as a parameter and/or access any of the pointers in the `potential` vector. Not all functions are going to be checked (for efficiency), so we check for function calls and add the name of the called function to `calledFunctions` vector. 

#### B. Obtaining all function calls

We will search `main` first:

1. Look for `call` instruction
2. If the function called is <u>not</u> `free`, then check whether the function takes in a pointer argument that is equal to one of the pointers stored in `potential`
3. If yes, store the function name and the argument index together as a pair in the vector `calledFunctions`
4. Otherwise, get the function name and set argument index to -1 and store it in the vector `calledFunctions`
<!-- 3. If the function argument is equal to a pointer stored in the `potential` vector, then add the function name to vector `calledFunctions`. -->

#### C. Adding global pointers to `potential` vector

Dynamic AoS can also be global variables. It is worth obtaining all global variables and adding them all to the `potential` vector, since they are all pointer types. They could be used in a `GPE` instruction (returning a struct) in any of the functions.

1. Get all global variables using `getGlobalList()`
<!-- 2. Iterate through this list and identify global variables with `pointer` types and store them in the `potential` vector -->
2. Add all global variables to vector `potential`

#### D. Checking for `GPE` instructions that return a `struct` and operate on a pointer stored in `potential`

Before checking all of the functions listed in `calledFunctions`, it is worth checking the `main()` function for `GPE` (GetElementPtr) instructions, which is used to access elements of a pointer. We check if these `GPE` instructions returns a `struct` type and access any of the pointers listed in `potential`, before confirming it as a dynamic AoS and putting it in the `confirmed` vector.

1. Look for `GPE` instructions
2. Check if its ResultElementType is a `struct`
3. If yes, check if the load operand is present in the `potential` vector
4. If yes, dynamic AoS confirmed, remove pointer from `potential` and store it in `confirmed`

Now it is time to check all functions in the `calledFunctions` vector and use the `potential` vector to see if any of these pointers are used in a `GEP` instruction that returns a `struct`. 

Note that steps A, B and D described above, as well as checks for static arrays, should be repeated when inspecting each function, since each function may have its own AoS data structures and may call other functions that have its own AoS data structures. Therefore, these four steps must be implemented recursively to inspect deeper levels of function calls.

For each function in `CalledFunctions` until the list is empty:

1. check for static arrays
2. check for potential AoS and store them in `potential`, if it doesn't exist 
3. check for function calls and add them to back of `calledFunctions`*
4. check for `GPE` instructions that operate on AoS on pointers stored in `potential`
5. remove all instances of this function call in list - has to match name and argument index

*Duplicate function calls are allowed since one function call may operate on a global, whilst another may take a parameter and operate on that instead.

### Pseudocode

```pseudo
vector<value> potential;
vector<value> usedAsArg;
vector<value> confirmed;
int detect = 0; //flag used to detect for storeinst

vector<pair<name,vector<int>> calledFunction

void detectStatic(Allocainst) - check staticAoS notes for this pseudocode

//after detecting a call instruction that is a malloc in a function, run this on next instruction (if flag = 1, perform this function, knowing that it follows a malloc)
void getPotential(Instruction)
{
    if store instruction
        get second operand of store
        save this operand in `potential` vector
    endif
    flag = 0
}

//if a call instruction (that is not a malloc or free) is detected, run this
void getCalledFunctions(CallInst)
{
    vector<int> indices;
    for each function call argument
        if any argument references a pointer stored in `potential`
            store index into args
            pop pointer value from `potential`
            store this pointer value in `usedAsArg` 
        endif
    endif
        store <function name, `indices`> in `calledFunctions` vector
    endif
}

//if a gpe inst is detected, run this
void checkGPE(gpe,Value,isArg)
{
    if getResultElementType of gpe is a struct
        get first operand (what is being loaded in)
        if operand is = Value
            dynamic AoS found
            if(isArg == false)
                pop Value from `potential` vector
            else
                pop Value from `usedAsArg` vector
            endif
            store Value in `confirmed` vector
        endif
    endif
}

//helper function to get the operands that store the passed-in arguments
vector<Value> getArgumentStores(Function* F, vector<int> indices)
{
    vector arguments;
    vector newStores;
    for each index
        get argument of function
        store in arguments
    endfor
    for each block in function
        for each instruction in function
            if storeinst found
                get first operand
                add this operand to newStores
            endif
            if size of newStores == size of indices
                break
        endfor
    endfor

    return newStores
}


///in main part of pass

vector globals = getGlobalList()
for each global variable
    store in `potential` vector
endfor

for loop - iterate through all functions in module
    int checkedMain = 0;
    if functionName == main
        for loop - iterate through all blocks in main
            for loop - iterate through all instructions in main
                if(flag = 1)
                    getPotential(instruction)
                if alloca inst found
                    call detectStaticAoS(allocainst)
                else if callinst found
                    if malloc
                        call getPotential()
                    else
                        call getCalledFunctions(callinst)
                else if gpe found
                    for every value in `potential`
                        call checkGPE(gpe,value,false)
                    endfor
                endif
            endfor
        endfor
        checkedMain = 1
    endif
    if checkedMain == 1
        break; //don't iterate other functions
    endif
endfor

//second loop to iterate through all called functions
while calledFunctions.size != 0
    searchFunc = first function name and vector indices pair in vector `calledFunctions`
    usedIndices = searchFunc.second
    vector<value> argStores
    for loop - iterate through all functions in module
        if current function name equal to searchFunc.first
            argStores = getArgumentStores(Function F, indices)
            for loop - iterate through all blocks in main
                for loop - iterate through all instructions in main
                    if(flag = 1)
                        getPotential(instruction)
                    if alloca inst found
                        call detectStaticAoS(allocainst)
                    else if callinst found
                        if malloc
                            call getPotential()
                        else
                            call getCalledFunctions(callinst)
                    else if gpe found
                        for every value in `potential` //check for use of global AoS
                            call checkGPE(gpe,value, false)
                        endfor
                        for every value in `argStores` //checks AoS that has been passed as argument
                            call checkGPE(gpe,value,true)
                endfor
            endfor
            //now remove current function name in `calledFunctions` vector
            for all elements in calledFunctions
                if element.first = currFunc.first AND element.second = currFunc.second
                    remove element
                endif
            endfor
        endif
    endfor
```

Dynamic AoS that have been accessed by GPE are only detected. Unused dynamic AoS are not detected.

## Representations

There are two representations of dynamic AoS data structures, both which are accessed/modified in the same way using `GEP` instructions.

### Malloc-ed pointers

#### With size as a literal:
##### <u>C Code:</u>

```C
struct nodeOne* arrayOne = (struct nodeOne*) malloc(100*sizeof(struct nodeOne)); 
```

##### <u>LLVM representation:</u>

Note that the size 400 is calculated by 100 * 4 (Number of elements * size of each int element)

Since the size of `struct nodeOne` is 4 bytes (32 bits)

```llvm
%struct.nodeOne = type { i32 }

%call = call noalias ptr @malloc(i64 noundef 400) #5 
store ptr %call, ptr %arrayOne, align 8

%arrayidx = getelementptr inbounds %struct.nodeOne, ptr %4, i64 %idxprom

```
#### With size as a variable:
##### <u>C Code:</u>

```C
n = 7080;
struct nodeTwo* arrayTwo = (struct nodeTwo*) malloc(n*sizeof(struct nodeTwo));
```
##### <u>LLVM representation:</u>
```llvm
%struct.nodeTwo = type { i32, i8, i8 }

%17 = load i32, ptr %n, align 4
%conv9 = sext i32 %17 to i64
%mul10 = mul i64 %conv9, 8
%call11 = call noalias ptr @malloc(i64 noundef %mul10) #5
store ptr %call11, ptr %arrayTwo, align 8
```

The second operand `8` in `%mul10` signifies the size of `struct nodeTwo` which is 6 bytes (4 + 1 + 1 = 32 + 4 + 4 = 40 bits) padded to 8 bytes

### Global pointers
##### <u>C Code:</u>

```C
struct nodeOne* globalOne;
```

##### <u>LLVM representation:</u>

```llvm
@globalOne = dso_local global ptr null, align 8
```