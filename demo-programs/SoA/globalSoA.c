#include <stdlib.h> //used for malloc()
#include <stdio.h> //for printf statements

#define SizeA 100000
#define SizeB 200000
#define SizeC 300000

struct StructureOne //SoA (with static arrays)
{
    int a[SizeA]; //sizes predefined for static arrays
    int b[SizeB];
    char c[SizeC];
};

struct test 
{
    int a; 
    int b;
    char c[SizeC];
};

struct StructureOne globalOne;
struct test globalTwo;
struct StructureOne globalThree[100];
struct StructureOne* globalFour;
struct StructureOne* globalFive;
struct test* globalSix;


void populateStructure(struct StructureOne* soa, int sizeA, int sizeB, int sizeC)
{
    int i;
    for(i  = 0; i < sizeA; i++)
    {
        soa->a[i] = i;
    }
    for(i  = 0; i < sizeB; i++)
    {
        soa->b[i] = i;
    }
    for(i  = 0; i < sizeC; i++)
    {
        soa->c[i] = i;
    }
}

void freeStructure(struct StructureOne* soa)
{
    //stack arrays automatically freed
    // free(soa->a);
    // free(soa->b);
    // free(soa->c);
    free(soa);
}

void printStructure(struct StructureOne* soa, int sizeA, int sizeB, int sizeC)
{
    int i;
    printf("Array a: \n");
    for(i  = 0; i < sizeA; i++)
    {
        printf("Index %d: %d\n",i,soa->a[i]);
    }
    printf("Array b: \n");
    for(i  = 0; i < sizeB; i++)
    {
        printf("Index %d: %d\n",i,soa->b[i]);
    }
    printf("Array c: \n");
    for(i  = 0; i < sizeC; i++)
    {
        printf("Index %d: %c\n",i,soa->c[i]);
    }
}

void populateTwo(struct StructureOne* s)
{
   struct StructureOne* s5 = (struct StructureOne*) malloc(sizeof(struct StructureOne)); //SoA 5 (%s5 in @populateTwo)
   populateStructure(s,200,200,200);
   populateStructure(s5,200,200,200);

}

void populateOne(struct StructureOne* s)
{
   struct StructureOne staticStruct; //SoA 4 (%staticStruct in @populateOne)
   staticStruct.a[99] = 1000;
   populateTwo(s);
}

void populateZero(struct StructureOne* s)
{
   struct StructureOne* s4 = (struct StructureOne*) malloc(sizeof(struct StructureOne)); //SoA 3 (%s4 in @populateZero)
   populateStructure(s4,100,100,100); 
   populateOne(s);

}

void populateAoS(struct StructureOne* s)
{
    s[97].a[45] = 100;
    s[23].a[45] = 100;
}

int main()
{
    globalOne.c[300] = 99; //SoA 1 (%globalOne)

    globalTwo.c[300] = 9; //NOT an SoA

    globalThree[9].a[100] = 9; //NOT an SoA (AoSoA)

    struct test aos[987]; //AoS

    aos[67].a = 10000;

    globalFour = (struct StructureOne*) calloc(100,sizeof(globalOne)); //NOT an SoA (AoSoA)

    // populateZero(globalFour); //gives error when uncommented

    // populateStructure(globalFour,100,100,100); //since no struct is being accessed in this func, we cannot determine if this is an AoS, also produces a duplicate (ignore bug)

    populateAoS(globalFour); //now detected as AoS

    globalFive = (struct StructureOne*) calloc(1,sizeof(globalOne));

    populateZero(globalFive); //SoA 2 (%globalFive)

    

    return 0;
}