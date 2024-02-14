#include <stdio.h>
#include <stdlib.h>

#define aosSize 999999


struct nodeOneNew
{
    //reordering by increasing alignment size (8, 4, 1, 1)
    //takes up 16 bytes - 16 / 8 = only two CPU cycles needed to access
    //also less memory taken up by struct
    double b[aosSize];
    int a[aosSize];
    char c[aosSize];
    char d[aosSize];
    //// 2 byte padding
};

// void populateNodeOneNew(struct nodeOneNew array[], int size)
// void populateNodeOneNew(struct nodeOneNew* array, int size)
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

// void multNodeOneNew(struct nodeOneNew* array, int size)
// {
//     for(int i = 0; i < size; i++)
//     {
//         array[i].a = (array[i].b * array[i].a) / (100 / 5);
//         array[i].b = (array[i].a - array[i].b) * (100 / 5);
//     }
// }

// void multArrays(struct nodeOneNew* arrayOne, struct nodeOneNew* arrayTwo, int size)
// {
//     for(int j = 0; j < 70; j++)
//     {
//         for(int i = 0; i < size; i++)
//         {
//             arrayOne[i].a = arrayOne[i].a * arrayTwo[i].a;
//             arrayOne[i].b = arrayOne[i].b * arrayTwo[i].b;
//         }
//     }
// }

int main()
{
    // int n = 99999;
    int n = aosSize;
    int k;

    for(k = 0; k < 100; k++)
    {
     ////static
    //  struct nodeOneNew arrayOneNew[n];
     ////dynamic
    // struct nodeOneNew* arrayOneNew = (struct nodeOneNew*) malloc(n*sizeof(struct nodeOneNew));
    // struct nodeOneNew* arrayTwoNew = (struct nodeOneNew*) malloc(n*sizeof(struct nodeOneNew));

    // struct nodeOneNew arrayOneNew; //single static struct is faster but limited size

    struct nodeOneNew* arrayOneNew = (struct nodeOneNew*) malloc(sizeof(struct nodeOneNew)); //ptr is slow but allow greater size
    struct nodeOneNew* arrayTwoNew = (struct nodeOneNew*) malloc(sizeof(struct nodeOneNew)); //ptr is slow


    //  struct nodeOneNew* arrayOneNew = aligned_alloc(64,sizeof(struct nodeOneNew) * n); //no difference
    //  struct nodeOneNew* arrayTwoNew = aligned_alloc(64,sizeof(struct nodeOneNew) * n); //no difference

    //  populateNodeOneNew(arrayOneNew,n);
    // populateNodeOneNew(arrayTwoNew,n);

    //  multNodeOneNew(arrayOneNew,n);
    //  multArrays(arrayOneNew,arrayTwoNew,n);

    //  for(int i = 0; i < n; i++)
    // {
    //     arrayOneNew.a[i] = 17.0;
    //     arrayOneNew.b[i]= 10;
    //     arrayOneNew.c[i] = 'c';
    //     arrayOneNew.d[i] = 'd';
    //     //array[i].b = array[i].a + array[i].b;
    // }

      for(int i = 0; i < n; i++)
    {
        arrayOneNew->a[i] = 10;
        arrayOneNew->b[i] = 17.0;
        arrayOneNew->c[i] = 'c';
        arrayOneNew->d[i] = 'd';

        arrayTwoNew->a[i] = 10;
        arrayTwoNew->b[i] = 17.0;
        arrayTwoNew->c[i] = 'c';
        arrayTwoNew->d[i] = 'd';
        //array[i].b = array[i].a + array[i].b;
    }

     for(int i = 0; i < n; i++)
    {
        arrayOneNew->a[i] = (arrayOneNew->b[i]  * arrayOneNew->a[i]) / (100 / 5);
        arrayOneNew->b[i]  = (arrayOneNew->a[i] - arrayOneNew->b[i] ) * (100 / 5);
    }

    for(int j = 0; j < 70; j++)
    {
        for(int i = 0; i < n; i++)
        {
            arrayOneNew->a[i] = arrayOneNew->a[i] * arrayOneNew->a[i];
            arrayOneNew->b[i]  = arrayOneNew->b[i]  * arrayTwoNew->b[i];
        }
    }

    // if(i == 79)
    // {
    //     printf("Validity check\n");
    //     // for(int j = 5000; j < 5010; j++)
    //     // {
    //         printf("a: %d\n---\n",arrayOneNew.a[1]);
    //         printf("b: %f\n---\n",arrayOneNew.b[1]);
    //         printf("c: %c\n---\n",arrayOneNew.c[1]);
    //         printf("d: %c\n---\n",arrayOneNew.d[1]);
    //     //}
    // }

    if(k == 79)
    {
        printf("Validity check\n");
        // for(int j = 5000; j < 5010; j++)
        // {
            printf("a: %d\n---\n",arrayOneNew->a[1]);
            printf("b: %f\n---\n",arrayOneNew->b[1]);
            printf("c: %c\n---\n",arrayOneNew->c[1]);
            printf("d: %c\n---\n",arrayOneNew->d[1]);
        //}
    }

     free(arrayOneNew);
    free(arrayTwoNew);
    }

    return 0;
}