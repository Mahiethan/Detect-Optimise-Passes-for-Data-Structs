#include <stdio.h>
#include <stdlib.h>

struct nodeOneOld
{
    char e;
    double g;
    int c;
    long long int d : 48;
    double f[2];
    double h;
    double i;
    int a;
    double b;
    float j;
};

struct nodeOneOld arrayOne[1000000];

void populateNode(struct nodeOneOld* array, int size)
{
    // for(int j = 0; j < 100000; j++)
    // {
        for(int i = 0; i < size; i++)
        {
            array[i].a = 1;
            array[i].b = 10.0;
            array[i].c = 9;
            array[i].d = 23.0;
            array[i].e = 'a';
            for(int j = 0; j < 2; j++)
                array[i].f[j] = 23.0;
            array[i].g = 23.0;
            array[i].h = 456.3234;
        }

        for(int i = 0; i < size; i++)
        {
            arrayOne[i].a = 1;
            arrayOne[i].b = 10.0;
            arrayOne[i].c = 9;
            arrayOne[i].d = 23.0;
            arrayOne[i].e = 'a';
            for(int j = 0; j < 2; j++)
                array[i].f[j] = 23.0;
            arrayOne[i].g = 23.0;
            arrayOne[i].h = 456.3234;
        }
    // }
}

/*
    Field reordering is not effective for large struct, when only a few structs are being accessed frequently

    No performance gains seen or in worst case, performance loss is seen (especially if top fields of structs are accessed frequently and this is then reordered)

    Mention this in report

*/

// void multNodeOneOld(int size)
// {
//     for(int j = 0; j < 100000; j++)
//     {
//         for(int i = 0; i < size; i++)
//         {
//             arrayOneOld[i].a = (arrayOneOld[i].b * arrayOneOld[i].a) + (100 / 5);
//             arrayOneOld[i].b = (arrayOneOld[i].a / arrayOneOld[i].c) - (99 * 3);
//         }
//     }
// }

// void multArrays(int size, struct nodeOneOld* arrayTwo)
// {
//     for(int j = 0; j < 100000; j++)
//     {
//         for(int i = 0; i < size; i++)
//         {
//             arrayOneOld[i].a = arrayOneOld[i].a * arrayTwo[i].a;
//             arrayOneOld[i].b = (arrayOneOld[i].b + arrayTwo[i].a) / 1.1;
//         }
//     }
// }

// void multNode(int size, struct nodeOneOld* array)
// {
//     for(int j = 0; j < 100000; j++)
//     {
//         for(int i = 0; i < size; i++)
//         {
//             array[i].b = (array[i].f[0] / array[i].b) - (99 * 3);
//             // array[i].i = (array[i].f[1] / array[i].c) - (99 * 3);
//         }
//     }
// }

// void multArrays(int size, struct nodeOneOld* array)
// {
//     for(int j = 0; j < 100000; j++)
//     {
//         for(int i = 0; i < size; i++)
//         {
//             array[i].i = (array[i].f[0]) * array[i].i;
//         }
//     }
// }

void calculationsOne(int size)
{
    for(int j = 0; j < 100000; j++)
    {
        for(int i = 0; i < size; i++)
        {
            arrayOne[i].a += 1;
            arrayOne[i].b += 0.001;
            arrayOne[i].c += 1;
            arrayOne[i].d += 1;
            for(int j = 0; j < 2; j++)
                arrayOne[i].f[j] += 0.001;
            arrayOne[i].g += 0.001;
            arrayOne[i].h += 0.001;
        }
    }
}

void calculationsTwo(int size, struct nodeOneOld* array)
{
    for(int j = 0; j < 100000; j++)
    {
        for(int i = 0; i < size; i++)
        {
            array[i].a += arrayOne[i].a;
            array[i].b += arrayOne[i].b;
            array[i].c += arrayOne[i].c;
            array[i].d += arrayOne[i].d;
            for(int j = 0; j < 2; j++)
                array[i].f[j] += arrayOne[i].f[j];
            array[i].g += arrayOne[i].g;
            array[i].h += arrayOne[i].h;
        }
    }
}

int main(int argc, char *argv[])
{
   int n = atoi(argv[1]);
   
    struct nodeOneOld* arrayTwoOld = (struct nodeOneOld*) malloc(n * sizeof(struct nodeOneOld));
    
    populateNode(arrayTwoOld,n);

    calculationsOne(n);
    calculationsTwo(n,arrayTwoOld);

    int i = (rand() % n);
    int j = (rand() % 67);

    // arrayOneOld[i].a = 1;
    // arrayOneOld[i].b = 10.0;
    // arrayOneOld[i].c = 9;

    // arrayTwoOld[i].a = 1;
    // arrayTwoOld[i].b = 10.0;
    // arrayTwoOld[i].c = 9;

    // multNodeOneOld(n);
    // multArrays(n, arrayTwoOld);

    //validity check
    printf("Validity check\n");

    printf("\na: ---\n%d",arrayTwoOld[i].a);
    printf("\nb: ---\n%f",arrayTwoOld[i].b);
    printf("\nc: ---\n%d",arrayTwoOld[i].c);
    printf("\nd: ---\n%lld",arrayTwoOld[i].d);
    printf("\ne: ---\n%d",arrayTwoOld[i].e);

    printf("\ng: ---\n%f",arrayTwoOld[i].g);
    printf("\nh: ---\n%f",arrayTwoOld[i].h);
    printf("\ni: ---\n%f",arrayTwoOld[i].i);


    return 0;
}