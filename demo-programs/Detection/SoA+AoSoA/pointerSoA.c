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
    double b;
    char c[98];
};

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
   populateTwo(s);
}

void populateZero(struct StructureOne* s)
{
   struct StructureOne* s4 = (struct StructureOne*) malloc(sizeof(struct StructureOne)); //SoA 4 (%s4 in @populateZero)
   populateStructure(s4,100,100,100); 
   populateOne(s);

}

void populateAos(struct StructureOne* aos, int size)
{
    for(int i = 0; i < size; i++)
    {
        aos[i].a[90] = 100;
    }
}

struct StructureOne* returnSoATwo()
{
    struct StructureOne* temp = (struct StructureOne*) malloc(sizeof(struct StructureOne)); //SoA 6
    temp->a[9] = 100;
    struct StructureOne* tempTwo = temp; //SoA 7
    return temp;
}

struct StructureOne* returnSoAOne()
{
    return returnSoATwo();
}

int recCountOne = 0;
int recCountTwo = 0;

void recursiveTwo();

void recursiveOne()
{
    struct StructureOne* recAoSoA = (struct StructureOne*) calloc(100,sizeof(struct StructureOne)); //AoSoA
    recAoSoA->c[9] = 'r';
    recursiveTwo();
}

void recursiveTwo()
{   
    if(recCountOne < 5)
    {
        recCountOne++;
        recursiveOne();
    }

    if(recCountTwo < 5)
    {
        struct StructureOne* recSoA = (struct StructureOne*) calloc(1,sizeof(struct StructureOne)); //SoA 9 (%recSoA at @recursiveTwo)
        recSoA->b[54] = 199;
        recCountTwo++;
        recursiveTwo();
    }

}

int main()
{
    //global struct pointer
    struct StructureOne* gs1 = (struct StructureOne*) malloc(sizeof(struct StructureOne)); //SoA 1 (%gs1)

    //AoSoA
    struct StructureOne* gs2;
    gs2 = (struct StructureOne*) malloc(100 * sizeof(struct StructureOne));

    populateAos(gs2,100);

    populateStructure(gs1,SizeA,SizeB,SizeC);

    //invalid
    struct test* wrong;
    wrong = (struct test*) malloc(sizeof(struct test));
    wrong->a = 10;

    struct StructureOne* gs3 = (struct StructureOne*) calloc(1,sizeof(struct StructureOne)); //SoA 2 (%gs3)

    gs3->a[100] = 10;

    struct StructureOne* gs4 = (struct StructureOne*) calloc(199,sizeof(struct StructureOne)); //AoSoA

    gs4[0].a[99] = 9;

    int* not = (int*) malloc(1500000); 

    struct StructureOne* gs5 = (struct StructureOne*) malloc(sizeof(struct StructureOne)); //SOA 3 (%gs5)
    populateZero(gs5);

    struct test* aos = (struct test*) malloc(10*sizeof(struct test)); //AoS
    aos[7].a = 0;
    aos[7].c[1] = 'f';
    printf("%c\n",aos[7].c[1]);

    struct StructureOne* rs1;
    rs1 = returnSoAOne(); //creates two SoAs (%temp and %temp2 in @returnSoATwo)

    printf("%d\n",rs1->a[9]); //SoA 8 (%rs1 at @main)

    recursiveOne(); //creates AoSoA

    freeStructure(gs1);
    freeStructure(gs2);
    freeStructure(gs3);
    freeStructure(gs4);
    freeStructure(gs5);

    return 0;
}