#include <stdio.h>
#include <stdlib.h>

struct nodeOne* globalOne;
struct nodeTwo* globalTwo;
struct nodeOne* globalThree;
struct nodeTwo* globalFour;
struct nodeOne* globalFive;
struct nodeOne* globalSix;

struct nodeOne invalidGlobal;


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

void populateNodeOne(struct nodeOne* array, int n)
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
    globalFour = (struct nodeTwo*) calloc(1000,sizeof(struct nodeTwo)); //(POTENTIAL: @globalFour at @populateThree)
    // populateTwo(); //SHOULD NOT RUN THIS - BEING CURRENTLY CALLED - SKIP!! - AVOID INFINITE RECURSION
}

void populateTwo()
{
    globalFour[67].b = 90.23; //dynamic AoS 5 - DETECTED!
    populateThree();
}


void populate()
{
    populateNodeOne(globalThree,100); //dynamic AoS 4 - DETECTED! (@globalThree at @populateNodeOne)
    int f = 100;
    globalFour = (struct nodeTwo*) calloc(f,sizeof(struct nodeTwo)); 
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
    //  populateParam0(array,n); //even though this doesn't result in an infinite recursion, do not run this if has been already called - FIX! - use a stack in recursion and see if this function is in it
   }
   else
   {
    populateParam2(array,n);
   }
}

void populateParam2(struct nodeOne* array, int n)
{
    struct nodeOne* innerArrayOne = (struct nodeOne*) malloc(100*sizeof(struct nodeOne));
    populateParam3(array,n);
    innerArrayOne[0].a = 99; //dynamic AoS 7 - DETECTED


}

void populateParam3(struct nodeOne* array, int n)
{
    populateParam4(array,n);
    globalFive = (struct nodeOne*) malloc(100*sizeof(struct nodeOne)); //to be populated in main()

}

void populateParam4(struct nodeOne* array, int n)
{
    populateNodeOne(array,n); //dynamic AoS 5 - DETECTED (%array.addr at @populateNodeOne)
    // innerArrayOne[2].a = 99; //Won't work - this variable is out of scope
    struct nodeTwo* arrayTwo = (struct nodeTwo*) calloc(100,sizeof(struct nodeTwo)); //dynamic AoS with duplicate name -  differentiated by origin function name
    arrayTwo[23].a = 1.02f; //dynamic AoS 6 - DETECTED! (%arrayTwo at @populateParam4)

}

void populateGlobal(int n, struct nodeOne* array)
{
    array = (struct nodeOne*) malloc(n*sizeof(struct nodeOne));
    populateNodeOne(array,n); //dynamic AoS 9 - DETECTED! (%array.addr at @populateNodeOne)
}

int main()
{
    int a = 100;
    struct nodeOne* arrayOne = (struct nodeOne*) malloc(a*sizeof(struct nodeOne));
    populateNodeOne(arrayOne,a); //dynamic AoS 1 - DETECTED! (%arrayOne in @populateNodeOne)

    int b = 100;
    struct nodeTwo* arrayTwo = (struct nodeTwo*) calloc(b,sizeof(struct nodeTwo));
    arrayTwo[23].a = 1.02f; //dynamic AoS 2 - DETECTED! (%arrayTwo in @main)

    // arrayTwo = (struct nodeTwo*) malloc(sizeof(struct nodeTwo)*b);
    // printf("%f\n",arrayTwo[23].a);
    
    int c = 100;
    globalOne = (struct nodeOne*) malloc(c*sizeof(struct nodeOne));
    globalOne[4].b = 'v'; //dynamic AoS 3 - DETECTED! 

    // printf("%c\n",globalOne[4].b);

    int* intArray = (int*) calloc(c,sizeof(int)); //not an AoS (POTENTIAL: %intArray in @main)
    intArray[55] = 23525;

    globalOne = (struct nodeOne*) malloc(c*sizeof(struct nodeOne)); //NOTE!!! - this clears the AoS - remove from confirmed and add back to potential - DONE

    // // printf("%c\n",globalOne[4].b); //no longer returns 'v', it is empty

    // (POTENTIAL: %globalOne in @main)

    // //dynamicAoSCount reduced by 1

    int d = 100;
    globalTwo = (struct nodeTwo*) calloc(d,sizeof(struct nodeTwo));
    populateNodeTwo(globalTwo,d); //dynamic AoS 3 - DETECTED! (%globalTwo in @populateNodeTwo)

    int e = 100;
    struct nodeOne* arrayThree = (struct nodeOne*) malloc(e*sizeof(struct nodeOne)); 

    int f = 100;
    // struct nodeOne* arrayPotential = (struct nodeOne*) malloc(f*sizeof(struct nodeOne));   // (POTENTIAL: %arrayPotential in @main)

    globalThree = (struct nodeOne*) malloc(e*sizeof(struct nodeOne));

    populate(); //adds globalThree (@globalThree at @populateNodeOne)

    populateParam0(arrayThree,e); //adds two dynamic AoS: innerArrayOne and arrayThree

    //dynamic AoS 8 - DETECTED
    //uncommenting any one of the below will enable detection of this AoS

    // globalFive[1].a = 100; //@globalFive at @main
    // printf("%d\n",globalFive[1].a); //@globalFive at @main
    // populateNodeOne(globalFive,100); //@globalFive at @populateNodeOne
    printf("%d\n",globalFive[0].a); //@globalFive at @main

     populateGlobal(d,globalSix); //adds @globalSix as %array.addr

    return 0;
}