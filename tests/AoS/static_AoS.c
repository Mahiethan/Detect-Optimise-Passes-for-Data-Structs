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
    arrayFour[67].b = 90.23; //dynamic AoS 4 - DETECTED!
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
   }
}

void populateParam2(struct nodeOne* array, int n)
{
    struct nodeOne innerArrayOne[100];
    populateParam3(array,n);
    innerArrayOne[0].a = 99; //dynamic AoS 5 - DETECTED
}

void populateParam3(struct nodeOne* array, int n)
{
    // //dynamic AoS 6 - DETECTED
    globalFive[1].a = 100;
    // populateNodeOne(globalFive,100);

    populateParam4(array,n);
}

void populateParam4(struct nodeOne* array, int n)
{
    populateNodeOne(array,n); //dynamic AoS 7 - DETECTED
    // innerArrayOne[2].a = 99; //Won't work - this variable is out of scope
}

int main()
{
    int a = 100;
    struct nodeOne arrayOne[a]; 
    populateNodeOne(arrayOne,a); //dynamic AoS 1 - DETECTED!

    int b = 100;
    struct nodeTwo arrayTwo[b];
    arrayTwo[23].a = 1.02f; //dynamic AoS 2 - DETECTED!

    int c = 100;
    int intArray[c]; //not an AoS
    intArray[55] = 23525;

    int e = 100;
    struct nodeOne arrayThree[e];

    globalThree[53].a = 99; //dynamic AoS 3 - DETECTED!

    populate(); //adds two static AoS: arrayFour and globalFour

    populateParam0(arrayThree,e); //adds three static AoS: arrayThree, innerArrayOne and globalFive

    return 0;
}