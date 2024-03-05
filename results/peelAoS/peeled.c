// #include <stdio.h>
// #include <stdlib.h>

// struct nodeOne //Total size: 16 bytes (saved 8 bytes)
// {
//     /////////////////////// Word 1
//     double b; // 8 bytes
//     /////////////////////// Word 2
//     int a; // 4 bytes
//     char c; // 1 byte
//     char d; // 1 byte
//     // 2 bytes padding
//     ///////////////////////
// };

// void populateNodeOne(struct nodeOne* array, int size)
// {
//     for(int i = 0; i < size; i++)
//     {
//         array[i].a = 17.0;
//         array[i].b = 10;
//         array[i].c = 'c';
//         array[i].d = 'd';
//         //array[i].b = array[i].a + array[i].b;
//     }
// }

// void multNodeOne(struct nodeOne* array, int size)
// {
//     for(int j = 0; j < 100000; j++)
//     {
//         float random = (float)rand()/(float)(RAND_MAX/5.0); //generate same 'random' number for each run
//         for(int i = 0; i < size; i++)
//         {
//             array[i].a = (array[i].b + 0.2 / random);
//             array[i].b = (array[i].a + (random * 0.2)) / 0.2;
//         }
//     }
// }

// void multNodeOneArrays(struct nodeOne* arrayOne, struct nodeOne* arrayTwo, int size)
// {
//     for(int j = 0; j < 100000; j++)
//     {
//         for(int i = 0; i < size; i++)
//         {
//             arrayOne[i].a = arrayOne[i].a * (arrayTwo[i].b * 0.1);
//             arrayOne[i].b = arrayOne[i].b * (arrayTwo[i].b * 0.1);
//         }
//     }
// }

// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// struct nodeTwo //Total size: 24 bytes (saved 8 bytes)
// {
//     /////////////////////// Word 1
//     double a; // 8 bytes
//     /////////////////////// Word 2
//     long long int b : 48; // 64-bit integer with bit-cast to 6 bytes (48 bits)  
//     char e; // 1 byte
//     /////////////////////// Word 3
//     int c; // 4 bytes
//     int d; // 4 bytes
//     /////////////////////// Word 4
// };

// void populateNodeTwo(struct nodeTwo* array, int size)
// {
//     for(int i = 0; i < size; i++)
//     {
//         array[i].a = 3.2;
//         array[i].b = 1000;
//         array[i].c = 77;
//         array[i].d = 10;
//         array[i].e = 'd';
//     }
// }

// void multNodeTwo(struct nodeTwo* array, int size)
// {
//     for(int j = 0; j < 100000; j++)
//     {
//         float random = (float)rand()/(float)(RAND_MAX/5.0); //generate same 'random' number for each run
//         for(int i = 0; i < size; i++)
//         {
//             array[i].a = (array[i].b + 0.2 / random);
//             array[i].b = (array[i].a + (random * 0.2)) / 0.2;
//         }
//     }
// }

// void multNodeTwoArrays(struct nodeTwo* arrayOne, struct nodeTwo* arrayTwo, int size)
// {
//     for(int j = 0; j < 100000; j++)
//     {
//         char randomletter = 'A' + (random() % 26);
//         for(int i = 0; i < size; i++)
//         {
//             arrayOne[i].b = arrayOne[i].b * arrayTwo[i].b;
//             arrayOne[i].e = randomletter;
//         }
//     }
// }

// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// struct nodeThree //Total size: 400 bytes (saved 16 bytes)
// {
//     /////////////////////// Word 1
//     // ...
//     /////////////////////// Word 48
//     float d[96]; //388 bytes (through word lines 1-48)
//     /////////////////////// Word 49
//     double b; // 8 bytes
//     /////////////////////// Word 50
//     int a; // 4 bytes
//     int c; // 4 bytes
//     ///////////////////////

// };

// void populateNodeThree(struct nodeThree* array, int size)
// {
//     for(int i = 0; i < size; i++)
//     {
//         array[i].a = 54;
//         array[i].b = 10.01;
//         array[i].c = 77;
//         float random = (float)rand()/(float)(RAND_MAX/5.0); //generate same 'random' number for each run
//         for(int j = 0; j < 96; j++)
//           array[i].d[j] = random;
//     }
// }

// void multNodeThree(struct nodeThree* array, int size)
// {
//     for(int j = 0; j < 100000; j++)
//     {
//         float random = (float)rand()/(float)(RAND_MAX/5.0); //generate same 'random' number for each run
//         for(int i = 0; i < size; i++)
//         {
//             array[i].b = (array[i].b + 0.2 / random);
//             array[i].c = (array[i].b + (random * 0.2)) / 0.2;
//         }
//     }
// }

// void multNodeThreeArrays(struct nodeThree* arrayOne, struct nodeThree* arrayTwo, int size)
// {
//     for(int i = 0; i < size; i++)
//     {
//         for(int k = 0; k < 96; k++)
//         {
//             arrayOne[i].d[k] += arrayTwo[i].d[k];
//         }
//     }
// }

// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// struct nodeFour // Total size: 496 bytes (no difference in size)
// {
//     /////////////////////// Word 1
//     // ...
//     /////////////////////// Word 55
//     float b[111]; // 444 bytes (through word lines 1-55)
//     char c[4]; // 4 bytes
//     /////////////////////// Word 56
//     // ...
//     /////////////////////// Word 61
//     int a[11]; // 44 bytes (through word lines 56-61)
//     int d[1]; // 4 bytes
//     ///////////////////////
// };

// void populateNodeFour(struct nodeFour* array, int size)
// {
//     for(int j = 0; j < size; j++)
//     {
//         int i;
//         for(i = 0; i < 11; i++)
//         {
//             array[j].a[i] = i;
//         }
//         for(i = 0; i < 111; i++)
//         {
//             array[j].b[i] = i;
//         }
//         for(i = 0; i < 4; i++)
//         {
//             array[j].c[i] = 'g';
//         }
//         array[j].d[0] = i;
//     }
// }

// void multNodeFourArrays(struct nodeFour* arrayOne, struct nodeFour* arrayTwo, int size)
// {
//     for(int j = 0; j < size; j++)
//     {   
//         int i;
//          for(i = 0; i < 11; i++)
//         {
//             arrayOne[j].a[i] *= arrayTwo[j].a[i];
//         }
//         for(i = 0; i < 111; i++)
//         {
//             arrayOne[j].b[i] *= arrayTwo[j].a[i];
//         }
//         arrayOne[j].d[0] *= arrayTwo[j].a[i];   
//     }
// }


// int main(int argc, char *argv[])
// {
//     int n = atoi(argv[1]); //size of all AoS - change this for testing
//     int randomIndex = rand() % n;
//     // int randomIteration = rand() % 100000;

//     // for(int loop = 0; loop < 10000; loop++)
//     // {
//     // normal Struct

//         struct nodeOne* arrayOne = (struct nodeOne*) calloc(n,sizeof(struct nodeOne));
//         struct nodeOne* arrayTwo = (struct nodeOne*) malloc(n*sizeof(struct nodeOne));

//         populateNodeOne(arrayOne,n);
//         populateNodeOne(arrayTwo,n);

//         multNodeOne(arrayOne,n);
//         multNodeOneArrays(arrayOne,arrayTwo,n);

//         // if(loop == randomIteration)
//         // {
//             printf("Validity check\n");
//             printf("a: %d\n---\n",arrayOne[randomIndex].a);
//             printf("b: %f\n---\n",arrayOne[randomIndex].b);
//             printf("c: %c\n---\n",arrayOne[randomIndex].c);
//             printf("d: %c\n---\n",arrayOne[randomIndex].d);
//         // }

//         free(arrayOne); 
//         free(arrayTwo);
        
//         // struct containing bitfield

//         struct nodeTwo* arrayThree = (struct nodeTwo*) malloc(n*sizeof(struct nodeTwo));
//         struct nodeTwo* arrayFour = (struct nodeTwo*) malloc(n*sizeof(struct nodeTwo));

//         populateNodeTwo(arrayThree,n);
//         populateNodeTwo(arrayFour,n);

//         multNodeTwo(arrayThree,n);
//         multNodeTwoArrays(arrayThree,arrayFour,n);

//         // if(loop == randomIteration)
//         // {
//             printf("Validity check\n");

//             printf("a: %f\n---\n",arrayThree[randomIndex].a);
//             printf("b: %lld\n---\n",arrayThree[randomIndex].b);
//             printf("c: %d\n---\n",arrayThree[randomIndex].c);
//             printf("d: %d\n---\n",arrayThree[randomIndex].d);
//             printf("e: %c\n---\n",arrayThree[randomIndex].e);
//         // }

//         free(arrayThree); 
//         free(arrayFour);

//         //struct containing arrays

//         struct nodeThree* arrayFive = (struct nodeThree*) calloc(n,sizeof(struct nodeThree));
//         struct nodeThree* arraySix = (struct nodeThree*) malloc(n*sizeof(struct nodeThree));

//         populateNodeThree(arrayFive,n);
//         populateNodeThree(arraySix,n);

//         multNodeThree(arrayFive,n);
//         multNodeThreeArrays(arrayFive,arraySix,n);

//         // if(loop == randomIteration)
//         // {

//             printf("Validity check\n");

//             int randomDIndex = rand() % 97;

//             printf("a: %c\n---\n",arrayFive[randomIndex].a);
//             printf("b: %f\n---\n",arrayFive[randomIndex].b);
//             printf("c: %d\n---\n",arrayFive[randomIndex].c);
//             printf("d: %f\n---\n",arrayFive[randomIndex].d[randomDIndex]);

//             free(arrayFive); 
//             free(arraySix);
//         // }

//         //AoSoA

//         struct nodeFour* arraySeven = (struct nodeFour*) calloc(n,sizeof(struct nodeFour));
//         struct nodeFour* arrayEight = (struct nodeFour*) malloc(n*sizeof(struct nodeFour));

//         populateNodeFour(arraySeven,n);
//         populateNodeFour(arrayEight,n);

//         multNodeFourArrays(arraySeven,arrayEight,n);

//         // if(loop == randomIteration)
//         // {
//             printf("Validity check\n");

//             printf("a: %d\n---\n",arraySeven[randomIndex].a[3]);
//             printf("b: %f\n---\n",arraySeven[randomIndex].b[57]);
//             printf("c: %c\n---\n",arraySeven[randomIndex].c[1]);
//             printf("d: %d\n---\n",arraySeven[randomIndex].d[0]);
//         // }

//         free(arraySeven); 
//         free(arrayEight);
//     //}
//     return 0;
// }