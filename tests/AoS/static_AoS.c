#include <stdio.h>
#include <stdlib.h>


struct nodeOne
{
    int a;
    char b;
    double c;
};

struct nodeTwo
{
    float a;
    double b;
};

struct nodeOne globalOne[100];
struct nodeTwo globalTwo[100];
struct nodeOne globalThree[100];
struct nodeTwo globalFour[100];
struct nodeOne globalFive[100];

struct nodeOne invalidGlobal;

void populateNodeOne(struct nodeOne array[], int n)
{
    for(int i = 0; i < n; i++)
    {
        array[i].a = 22;
    }
}

void populateNodeTwo(struct nodeTwo* array, int n)
{
    for(int i = 0; i < n; i++)
    {
        array[i].a = 12.0f;
    }
}


void populateTwo();

void populateThree()
{
    int f = 100;
    struct nodeTwo arrayFour[f]; 
    // struct nodeTwo arrayFour[100]; 
    arrayFour[67].b = 90.23; //dynamic AoS 4 - DETECTED! (%vla)

    // populateTwo(); //SHOULD NOT RUN THIS - BEING CURRENTLY CALLED - SKIP!! - AVOID INFINITE RECURSION
}

void populateTwo()
{
    
    populateThree();
}


void populate()
{
    int f = 100;
    struct nodeOne arrayFour[f]; //not a static AoS - not used in a GEP instruction
    populateTwo();
}

void populateParam0(struct nodeOne* array, int n);
void populateParam1(struct nodeOne* array, int n);
void populateParam2(struct nodeOne* array, int n);
void populateParam3(struct nodeOne* array, int n);
void populateParam4(struct nodeOne* array, int n);


void populateParam0(struct nodeOne* array, int n)
{
    populateParam1(array,n);
}

void populateParam1(struct nodeOne* array, int n)
{
   if(n > 100)
   {
     n--;
    //  populateParam0(array,n); //even though this doesn't result in an infinite recursion, do not run this if has been already called - FIX!
   }
   else
   {
    populateParam2(array,n);
    globalFour[99].a = 99; //static AoS 5 - DETECTED
   }
}

void populateParam2(struct nodeOne* array, int n)
{
    struct nodeOne innerArrayOne[100];
    populateParam3(array,n);
    innerArrayOne[0].a = 99; //static AoS 6 - DETECTED
}

void populateParam3(struct nodeOne* array, int n)
{
    // //dynamic AoS 7 - DETECTED
    globalFive[1].a = 100;
    // populateNodeOne(globalFive,100);

    populateParam4(array,n);
}

void populateParam4(struct nodeOne* array, int n)
{
    populateNodeOne(array,n); //static AoS 8 - DETECTED
    // innerArrayOne[2].a = 99; //Won't work - this variable is out of scope
}

int main()
{
    int a = 100;
    struct nodeOne arrayOne[a]; 
    // struct nodeOne arrayOne[100]; 
    populateNodeOne(arrayOne,a); //static AoS 1 - DETECTED! (%vla)

    int b = 100;
    struct nodeTwo arrayTwo[b];
    // struct nodeTwo arrayTwo[100]; 
    arrayTwo[23].a = 1.02f; //static AoS 2 - DETECTED! (%vla1)

    int c = 100;
    int intArray[c]; //not an AoS
    intArray[55] = 23525;

    int e = 100;
    struct nodeOne arrayThree[e];
    // struct nodeOne arrayThree[100];

    globalThree[53].a = 99; //static AoS 3 - DETECTED! (%globalThree)

    populate(); //adds 4th static AoS: arrayFour (%vla in @populateThree)

    populateParam0(arrayThree,e); //adds three static AoS: globalFive (@globalFive), arrayThree (%array.addr in @populateNodeOne), innerArrayOne (%innerArrayOne) and globalFour (@globalFour)

    return 0;
}