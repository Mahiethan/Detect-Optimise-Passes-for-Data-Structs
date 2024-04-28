#include <stdlib.h> //used for malloc()
#include <stdio.h> //for printf statements

#define SizeA 1000
#define SizeB 2000
#define SizeC 3000

struct StructureOne //SoA (with static arrays)
{
    int a[SizeA]; //sizes predefined for static arrays
    int b[SizeB];
    char c[SizeC];
};

struct StructureTwo //NOT a SoA
{
    int a[SizeA]; //sizes predefined for static arrays
    int b[SizeB];
    char c;
};

struct StructureOne populateStructure(struct StructureOne soa, int sizeA, int sizeB, int sizeC)
{
    int i;
    for(i  = 0; i < sizeA; i++)
    {
        soa.a[i] = i;
    }
    for(i  = 0; i < sizeB; i++)
    {
        soa.b[i] = i;
    }
    for(i  = 0; i < sizeC; i++)
    {
        soa.c[i] = i;
    }

    return soa;
}

struct StructureOne* createStructureOne()
{
    struct StructureOne* s1  = (struct StructureOne*) malloc(sizeof(struct StructureOne));
    return s1;

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

struct StructureOne populateZero(struct StructureOne s)
{
   struct StructureOne s4; //SoA 3 (%s1 in @main)
   s4 = populateStructure(s4,100,100,100); 
   return populateStructure(s,100,100,100); 
}

int main()
{
    //structs
    struct StructureOne s1; //SoA 1 (%s1 in @main)
    struct StructureTwo s2;

    struct StructureTwo wrong;
    wrong.a[10] = 10;

    struct StructureOne s3; //SoA 2 (%s3 in @main)

    struct StructureOne s4[100]; //AoS

    //if a store instruction is found from accessing a identifed struct, this is an SoA
    s1.a[0] = 100;
    s1.a[21] = 100;
    s1.c[7] = 'b';
    s4[9].a[0] = 10;
    printf("%d\n", s1.a[0]);
    printf("%d\n", s3.a[0]);

    return 0;
}