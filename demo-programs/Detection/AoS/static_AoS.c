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

int recursiveCount = 0;

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


void populateTwo(struct nodeTwo* ptr);

void populateThree()
{
    int f = 100;
    struct nodeTwo arrayFour[f]; 
    arrayFour[67].b = 90.23; //dynamic AoS 4 - DETECTED! (%vla)
    struct nodeTwo* recursive = (struct nodeTwo*) calloc(100,sizeof(struct nodeTwo));

    if(recursiveCount != 5)
    {
        recursiveCount++;
        populateTwo(recursive); //AoS 9 - (%recursive in @populateThree)
    }
}

void populateTwo(struct nodeTwo* ptr)
{
    if(ptr != NULL)
        ptr[100].a = 7.0;

    populateThree();
}


void populate()
{
    int f = 100;
    struct nodeOne arrayFour[f]; //not a static AoS - not used in a GEP instruction
    populateTwo(NULL);
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

    populateParam4(array,n);
}

void populateParam4(struct nodeOne* array, int n)
{
    populateNodeOne(array,n); //static AoS 8 - DETECTED
}

int main()
{
    int a = 100;
    struct nodeOne arrayOne[a]; 
    populateNodeOne(arrayOne,a); //static AoS 1 - DETECTED! (%vla)

    int b = 100;
    struct nodeTwo arrayTwo[b];
    arrayTwo[23].a = 1.02f; //static AoS 2 - DETECTED! (%vla1)

    int c = 100;
    int intArray[c]; //not an AoS
    intArray[55] = 23525;

    int e = 100;
    struct nodeOne arrayThree[e];

    globalThree[53].a = 99; //static AoS 3 - DETECTED! (%globalThree)

    populate(); //adds 4th static AoS: arrayFour (%vla in @populateThree)

    populateParam0(arrayThree,e); //adds four static AoS: globalFive (@globalFive), arrayThree (%vla5 in @main), innerArrayOne (%innerArrayOne) and globalFour (@globalFour)

    return 0;
}