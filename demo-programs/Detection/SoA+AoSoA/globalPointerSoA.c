#include <stdlib.h> //used for malloc()
#include <stdio.h> //for printf statements

#define SizeA 100000
#define SizeB 200000
#define SizeC 300000

//global struct pointer
struct StructureOne* gs1;

struct StructureOne //SoA (with static arrays)
{
    int a[SizeA]; //sizes predefined for static arrays
    int b[SizeB];
    char c[SizeC];
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

int main()
{
    gs1 = (struct StructureOne*) malloc(sizeof(struct StructureOne));

    //if a store instruction is found from accessing a identifed struct inside function calls, this is an SoA
    populateStructure(gs1,SizeA,SizeB,SizeC);

    freeStructure(gs1);

    return 0;
}