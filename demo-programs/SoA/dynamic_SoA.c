/*

THIS IS NOT AN SOA BY DEFINITION - STRUCT SHOULD CONTAIN ONLY ARRAYS, NOT POINTERS
POINTERS ADD OVERHEADS WHEN ACCESSING AND DYNAMICALLY ALLOCATING

*/


// #include <stdlib.h> //used for malloc()
// #include <stdio.h> //for printf statements

// #define SizeA 100000
// #define SizeB 200000
// #define SizeC 300000

// //global struct pointer
// struct StructureOne* g1;

// struct StructureOne //SoA (with dynamic arrays in heap)
// {
//     int* a;
//     int* b;
//     char* c;
// };

// void populateStructure(struct StructureOne* soa, int sizeA, int sizeB, int sizeC)
// {
//     int i;
//     for(i  = 0; i < sizeA; i++)
//     {
//         soa->a[i] = i;
//     }
//     for(i  = 0; i < sizeB; i++)
//     {
//         soa->b[i] = i;
//     }
//     for(i  = 0; i < sizeC; i++)
//     {
//         soa->c[i] = i;
//     }
// }

// struct StructureOne* createStructureOne()
// {
//     struct StructureOne* s1  = (struct StructureOne*) malloc(sizeof(struct StructureOne));
//     s1->a = (int *) malloc(SizeA * sizeof(int));
//     s1->b = (int *) malloc(SizeB * sizeof(int));
//     s1->c = (char *) malloc(SizeC * sizeof(char));
//     return s1;

// }

// void freeStructure(struct StructureOne* soa)
// {
//     free(soa->a);
//     free(soa->b);
//     free(soa->c);
//     free(soa);
// }

// void printStructure(struct StructureOne* soa, int sizeA, int sizeB, int sizeC)
// {
//     int i;
//     printf("Array a: \n");
//     for(i  = 0; i < sizeA; i++)
//     {
//         printf("Index %d: %d\n",i,soa->a[i]);
//     }
//     printf("Array b: \n");
//     for(i  = 0; i < sizeB; i++)
//     {
//         printf("Index %d: %d\n",i,soa->b[i]);
//     }
//     printf("Array c: \n");
//     for(i  = 0; i < sizeC; i++)
//     {
//         printf("Index %d: %c\n",i,soa->c[i]);
//     }
// }

// int main()
// {
//     struct StructureOne* s1  = createStructureOne();
//     // populateStructure(s1,SizeA,SizeB,SizeC);
//     // printStructure(s1,SizeA,SizeB,SizeC);
//     g1 = (struct StructureOne*) malloc(sizeof(struct StructureOne));

//     freeStructure(s1);
//     return 0;
// }