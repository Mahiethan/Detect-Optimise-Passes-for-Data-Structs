#include <stdlib.h> //used for malloc()
#include <stdio.h> //for printf statements

#define SizeA 100345
#define SizeB 203459
#define SizeC 315243

//global AoSoA
struct StructureOne* gs2;

struct StructureOne //SoA (with static arrays)
{
    int a[SizeA]; //sizes predefined for static arrays
    int b[SizeB];
    char c[SizeC];
};

void populateStructure(struct StructureOne* soa, int size, int sizeA, int sizeB, int sizeC)
{
    for(int j = 0; j < size; j++)
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
}

void freeStructure(struct StructureOne* soa)
{
    free(soa);
}

void printStructure(struct StructureOne* soa, int size, int sizeA, int sizeB, int sizeC)
{
    for(int j = 0; j < size; j++)
    {
        printf("Accessing struct %d\n", j);
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
}

void populateAoSoA(struct StructureOne* a, int n)
{
    for(int i = 0; i < n; i++)
    {
        a[i].a[90] = 100;
    }
}



int main()
{
    int n = 1;
    struct StructureOne* s1 = (struct StructureOne*) malloc(10 * sizeof(struct StructureOne)); //AoSoA 1
    gs2 = (struct StructureOne*) calloc(n,sizeof(struct StructureOne)); //AoSoA 2

    //will be detected
    s1[0].a[2] = 100;
    printf("%d\n",s1[0].a[2]);

    populateAoSoA(gs2,n);

    freeStructure(s1);
    freeStructure(gs2);

    return 0;
}