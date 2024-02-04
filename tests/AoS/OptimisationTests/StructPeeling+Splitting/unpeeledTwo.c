#include <stdio.h>
#include <stdlib.h>

// #pragma pack(1)


struct nodeOneOld
{

    //// Hot fields - most commonly used fields
    int a;
    double b;
    int c;
    //// Cold fields - least used fields
    long long int d : 48;
    char e;
    double f;
    double g;
    double h;
    double i;
    float j;
};

struct nodeOneOld arrayOneOld[999999];
// struct nodeOneOld arrayOneOldCold[999999]; //duplicate global arrays do not cause issues
struct nodeOneOld arrayTwoOld[999999];
// struct nodeOneOld* arrayTwoOld;

// void populateNodeOneOld(struct nodeOneOld array[], int size)
void populateNodeOneOld(int size)
{
    for(int i = 0; i < size; i++)
    {
        arrayOneOld[i].a = 1;
        arrayOneOld[i].b = 10.0;
        arrayOneOld[i].c = 9;
        arrayOneOld[i].d = 23.0;
        arrayOneOld[i].e = 'a';
        arrayOneOld[i].f = 23.0;
        arrayOneOld[i].g = 23.0;

        arrayTwoOld[i].a = 1;
        arrayTwoOld[i].b = 10.0;
        arrayTwoOld[i].c = 9;
        arrayTwoOld[i].d = 23.0;
        arrayTwoOld[i].e = 'a';
        arrayTwoOld[i].f = 23.0;
        arrayTwoOld[i].g = 23.0;
        //array[i].b = array[i].a + array[i].b;
    }
}

// void populateNodeOneLocal(struct nodeOneOld local[], int size)
// {
//     for(int i = 0; i < size; i++)
//     {
//         arrayOneOld[i].a = 1;
//         arrayOneOld[i].b = 10.0;
//         arrayOneOld[i].c = 9;
//         arrayOneOld[i].d = 23.0;
//         arrayOneOld[i].e = 'a';
//         arrayOneOld[i].f = 23.0;
//         arrayOneOld[i].g = 23.0;

//         arrayTwoOld[i].a = 1;
//         arrayTwoOld[i].b = 10.0;
//         arrayTwoOld[i].c = 9;
//         arrayTwoOld[i].d = 23.0;
//         arrayTwoOld[i].e = 'a';
//         arrayTwoOld[i].f = 23.0;
//         arrayTwoOld[i].g = 23.0;
//         //array[i].b = array[i].a + array[i].b;
//     }
// }

void multNodeOneOld(int size)
{
    for(int j = 0; j < 30; j++)
    {
        for(int i = 0; i < size; i++)
        {
            arrayOneOld[i].a = (arrayOneOld[i].b * arrayOneOld[i].a) + (100 / 5);
            arrayOneOld[i].b = (arrayOneOld[i].a / arrayOneOld[i].c) - (99 * 3);
        }
    }
}

void multArrays(int size)
{
    for(int j = 0; j < 30; j++)
    {
        for(int i = 0; i < size; i++)
        {
            arrayOneOld[i].a = arrayOneOld[i].a * arrayTwoOld[i].a;
            arrayOneOld[i].b = arrayOneOld[i].b * arrayTwoOld[i].b;
        }
    }
}

void checkIndex(double e)
{
    printf("Yes\n");
}

int main()
{
   // int n = 99999;
   int n = 999999;
   int i;
   for(i = 0; i < 100; i++)
    {
    ////static
    // struct nodeOneOld arrayOneOld[n];
    ////dynamic
    // struct nodeOneOld* arrayOneOld = (struct nodeOneOld*) malloc(n*sizeof(struct nodeOneOld));
    // arrayTwoOld = (struct nodeOneOld*) malloc(n*sizeof(struct nodeOneOld));

    //  struct nodeOneOld* arrayOneOld = aligned_alloc(64,sizeof(struct nodeOneOld) * n); //no difference
    // struct nodeOneOld* arrayTwoOld = aligned_alloc(64,sizeof(struct nodeOneOld) * n); //no difference

    struct nodeOneOld arrayOneLocal[100];
    struct nodeOneOld arrayTwoLocal[100];

    populateNodeOneOld(n);

    arrayOneLocal[i].a = 1;
    arrayOneLocal[i].b = 10.0;
    arrayOneLocal[i].c = 9;
    // arrayOneLocal[i].d = 23.0;
    // arrayOneLocal[i].e = 'a';
    // arrayOneLocal[i].f = 23.0;
    // arrayOneLocal[i].g = 23.0;

    arrayTwoLocal[i].a = 1;
    arrayTwoLocal[i].b = 10.0;
    arrayTwoLocal[i].c = 9;
    // arrayTwoLocal[i].d = 23.0;
    // arrayTwoLocal[i].e = 'a';
    // arrayTwoLocal[i].f = 23.0;
    // arrayTwoLocal[i].g = 23.0;

     multNodeOneOld(n);
     multArrays(n);

      //validity check
      if(i == 77)
     {
        printf("Validity check\n");

        //required for printing global AoS variables until fix is found
        // struct nodeOneOld* arrayOneTemp = (struct nodeOneOld*) malloc(999999*sizeof(struct nodeOneOld));
        // arrayOneTemp = arrayOneOld;        // for(int j = 5000; j < 5010; j++)
        // {
            // FILE *fptr = fopen("sample.txt", "w"); 
            struct nodeOneOld* result = arrayOneOld;
            printf("%d\n---\n",result[5000].a);
            // printf("%d\n---\n",result[5000].a);
            //  printf("%d\n---\n",result[5000].a);
            // printf("%d\n---\n",result[5000].a);
            //  printf("%d\n---\n",result[5000].a);
            // printf("%d\n---\n",result[5000].a);
            //  printf("%d\n---\n",result[5000].a);
            //  printf("%d\n---\n",result[5000].a);
            //  printf("%d\n---\n",result[5000].a);
            //  printf("%d\n---\n",result[5000].a);
            //  printf("%d\n---\n",result[5000].a);
            //  printf("%d\n---\n",result[5000].a);
            //  printf("%d\n---\n",result[5000].a);
            //  printf("%d\n---\n",result[5000].a);
            //  printf("%d\n---\n",result[5000].a);
            //  printf("%d\n---\n",result[5000].a);
            //  printf("%d\n---\n",result[5000].a);
            //  printf("%d\n---\n",result[5000].a);
            //  printf("%d\n---\n",result[5000].a);
            //  printf("%d\n---\n",result[5000].a);
            //  printf("%d\n---\n",result[5000].a);
            //  printf("%d\n---\n",result[5000].a);
            //  printf("%d\n---\n",result[5000].a);
            //  printf("%d\n---\n",result[5000].a);
            //  printf("%d\n---\n",result[5000].a);

             arrayOneLocal[67].d = 90;
             printf("%lld\n",arrayOneLocal[67].d);


            //GEP index now updating
            // fprintf(fptr,"%d\n---\n",arrayOneOld[5000].a);
            // printf("%d\n---\n",arrayOneOld[5000].a);
            // printf("%d\n---\n",result[5000].a);
            printf("\na: ---\n%d",arrayOneOld[5000].a);
            printf("\nb: ---\n%f",arrayOneOld[5000].b);
            printf("\nc: ---\n%d",arrayOneOld[5000].c);
            printf("\nd: ---\n%lld",arrayOneOld[5000].d);
            printf("\ne: ---\n%d",arrayOneOld[5000].e);
            printf("\nf: ---\n%f",arrayOneOld[5000].f);
            printf("\ng: ---\n%f",arrayOneOld[5000].g);
            printf("\nh: ---\n%f",arrayOneOld[5000].h);
            printf("\ni: ---\n%f",arrayOneOld[5000].i);
            printf("\nj: ---\n%f\n",arrayOneOld[5000].j);

            //GIVES ERROR - cannot re-use a ptr like the one above - can caused undefined behaviour (may work or may not work) because the field being accessed may now have a nodeOneCold struct
            //MAYBE IT DOES WORK NOW ?!!
            // result = arrayOneLocal;
            // printf("%f\n---\n",result[43].g);
            // // // printf("%f\n---\n",resultTwo[43].f);
            // result = arrayTwoLocal;
            // printf("%f\n---\n",result[43].g);

            // checkIndex(result[96].d);
            // printf("%f\n---\n",result[43].g);
            //
            //// BUT THIS WORKS WHEN REUSING ptr result - BECAUSE both fields belong in the hot struct
            // result = arrayOneLocal;
            // printf("%d\n---\n",result[43].a);
            // result = arrayTwoLocal;
            // printf("%f\n---\n",result[43].g);

            //WORKS THIS WAY
            // printf("%d\n---\n",arrayOneLocal[43].a);
            // printf("%f\n---\n",arrayOneLocal[43].f);
            //OR
            // struct nodeOneOld* resultTwo = arrayOneLocal;
            // printf("%d\n---\n",resultTwo[43].a);
            // printf("%d\n---\n",resultTwo[43].a);
            // printf("%d\n---\n",resultTwo[43].a);
            // printf("%d\n---\n",resultTwo[43].a);
            // printf("%d\n---\n",resultTwo[43].a);
            // printf("%d\n---\n",resultTwo[43].a);
            // printf("%d\n---\n",resultTwo[43].a);
            // struct nodeOneOld* resultThree = arrayTwoLocal;
            // // printf("%d\n---\n",resultTwo[43].a);
            // printf("%f\n---\n",resultTwo[43].f);


        //}
     }

    
    //  free(arrayOneOld); 
    //  free(arrayTwoOld);
    }


    return 0;
}