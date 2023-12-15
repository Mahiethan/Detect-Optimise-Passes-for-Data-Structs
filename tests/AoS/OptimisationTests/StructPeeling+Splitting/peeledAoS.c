#include <stdio.h>
#include <stdlib.h>

// #pragma pack(1)

//Applying structure peeling
struct nodeOneCold
{
    //// Cold fields - least used fields
    double d;
    char e;
    double f;
    double g;
    double h;
    double i;
    float j;
};

struct nodeOneHot
{
    //// Hot fields - most commonly used fields
    int a;
    double b;
    int c;
};

struct nodeOneHot arrayOneHot[999999];
//new array to store cold structure
struct nodeOneCold arrayOneCold[999999];

struct nodeOneHot arrayTwoHot[999999];
//new array to store cold structure
struct nodeOneCold arrayTwoCold[999999];

//adding new argument for nodeOneCold struct
void populateNodeOneOld(struct nodeOneHot* array, struct nodeOneCold* sub, int size)
{
    for(int i = 0; i < size; i++)
    {
        array[i].a = 1;
        array[i].b = 10.0;
        array[i].c = 9;
        // array[i].cold = (struct nodeOneCold*) malloc(sizeof(struct nodeOneCold));
        sub[i].d = 23.0;
        sub[i].e = 'a';
        sub[i].f = 23.0;
        sub[i].g = 23.0;  
    }
}

void multNodeOneOld(struct nodeOneHot* array, int size)
{
    for(int j = 0; j < 30; j++) //the greater the limit, the more effective the optimisation is since the affinity of a and b is very high (see notes)
    {
        for(int i = 0; i < size; i++)
        {
            array[i].a = (array[i].b * array[i].a) + (100 / 5);
            array[i].b = (array[i].a / array[i].c) - (99 * 3);
        }
    }
}

void multArrays(struct nodeOneHot* arrayOne, struct nodeOneHot* arrayTwo, int size)
{
    for(int j = 0; j < 30; j++)
    {
        for(int i = 0; i < size; i++)
        {
            arrayOne[i].a = arrayOne[i].a * arrayTwo[i].a;
            arrayOne[i].b = arrayOne[i].b * arrayTwo[i].b;
        }
    }
}

// void freeAoS(struct nodeOneHot* array, int size)
// {
//     for(int i = 0; i < size; i++)
//         free(array[i].cold);
//     free(array);
// }

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
    // struct nodeOneHot* arrayOneHot = (struct nodeOneHot*) malloc(n*sizeof(struct nodeOneHot));
    // //create one for nodeOneCold struct too
    // struct nodeOneCold* arrayOneCold = (struct nodeOneCold*) malloc(n*sizeof(struct nodeOneCold));

    // struct nodeOneHot* arrayTwoHot = (struct nodeOneHot*) malloc(n*sizeof(struct nodeOneHot));
    // struct nodeOneCold* arrayTwoCold = (struct nodeOneCold*) malloc(n*sizeof(struct nodeOneCold));

    //  struct nodeOneOld* arrayOneOld = aligned_alloc(64,sizeof(struct nodeOneOld) * n); //no difference
    // struct nodeOneOld* arrayTwoOld = aligned_alloc(64,sizeof(struct nodeOneOld) * n); //no difference

     populateNodeOneOld(arrayOneHot,arrayOneCold,n);
     populateNodeOneOld(arrayTwoHot,arrayOneCold,n);

     multNodeOneOld(arrayOneHot,n);
     multArrays(arrayOneHot,arrayTwoHot,n);

     //validity check
      if(i == 77)
     {
        printf("Validity check\n");

        //required for printing global AoS variables until fix is found
        struct nodeOneHot* arrayOneTemp = (struct nodeOneHot*) malloc(999999*sizeof(struct nodeOneHot));
        arrayOneTemp = arrayOneHot;

        // for(int j = 5000; j < 5010; j++)
        // {
            printf("%d\n---\n",arrayOneTemp[50].a);
            printf("%f\n---\n",arrayOneTemp[50].b);
        //}
        
     }

    // freeAoS(arrayOneHot,n); 
    // free(arrayOneHot);
    // free(arrayOneCold); 
    // // freeAoS(arrayTwoHot,n); 
    // free(arrayTwoHot);
    // free(arrayTwoCold); 
    }


    return 0;
}