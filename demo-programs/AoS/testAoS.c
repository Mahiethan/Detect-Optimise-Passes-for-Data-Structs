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
struct nodeOne* globalSix;
struct nodeOne* globalSeven;


void pop()
{
    globalFive->c = 0.0; //AoS 5 - (%globalFive in @pop)
}

void populateTwo(struct nodeOne* ptr)
{
    ptr[10].b = 'c'; 
    pop();
}

void populateOne(struct nodeOne* ptr)
{
    struct nodeOne* tempOne = (struct nodeOne*) malloc(100*sizeof(struct nodeOne));
    tempOne[0].a = 10; //AoS 1 - (%tempOne in @populateOne)
    populateTwo(ptr); 
    globalSix[9].a = 100;
    // pop(); //gives error (any function call here) - out of memory? - not priority to fix, unless same error occurs elsewhere
}

int main()
{
    struct nodeOne* arrayOne = (struct nodeOne*) malloc(100*sizeof(struct nodeOne));
    struct nodeOne* arrayTwo = (struct nodeOne*) malloc(100*sizeof(struct nodeOne));
    globalSix = arrayOne;
    // globalSix[9].a = 100;

    struct nodeOne staticOne[1000];


    populateOne(arrayOne); //AoS 2 - (%arrayOne in @main)
    populateOne(arrayTwo); //AoS 3- (%arrayOne in @main)
    populateOne(globalSix); //AoS 4 - (%arrayTwo in @main)
    populateTwo(staticOne);

    // pop();
    
    return 0;
}