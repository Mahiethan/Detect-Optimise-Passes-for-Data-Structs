#include <stdio.h>
#include <stdlib.h>

struct nodeOneOld
{

    //// Hot fields - most commonly used fields
    int a;
    double b;
    int c;
    //// Cold fields - least used fields
    long long int d : 48;
    char e;
    double f[67];
    double g;
    double h;
    double i;
    float j;
};

struct nodeOneOld arrayOneOld[1000000];
struct nodeOneOld arrayTwoOld[1000000];

void populateNodeOneOld(int size)
{
    for(int i = 0; i < size; i++)
    {
        arrayOneOld[i].a = 1;
        arrayOneOld[i].b = 10.0;
        arrayOneOld[i].c = 9;
        arrayOneOld[i].d = 23.0;
        arrayOneOld[i].e = 'a';
        for(int j = 0; j < 67; j++)
            arrayOneOld[i].f[j] = 23.0;
        arrayOneOld[i].g = 23.0;

        arrayTwoOld[i].a = 1;
        arrayTwoOld[i].b = 10.0;
        arrayTwoOld[i].c = 9;
        // arrayTwoOld[i].d = 23.0;
        // arrayTwoOld[i].e = 'a';
        // for(int j = 0; j < 67; j++)
        //     arrayTwoOld[i].f[j] = 23.0;
        // arrayTwoOld[i].g = 23.0;
    }
}


void multNodeOneOld(int size)
{
    for(int j = 0; j < 100000; j++)
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
    for(int j = 0; j < 100000; j++)
    {
        for(int i = 0; i < size; i++)
        {
            arrayOneOld[i].a = arrayOneOld[i].a * arrayTwoOld[i].a;
            arrayOneOld[i].b = (arrayOneOld[i].b + arrayTwoOld[i].a) / 1.1;
        }
    }
}


int main(int argc, char *argv[])
{
   // int n = 99999;
   int n = atoi(argv[1]);
   
    ////static
    // struct nodeOneOld arrayOneOld[n];
    ////dynamic
    // struct nodeOneOld* arrayOneOld = (struct nodeOneOld*) malloc(n*sizeof(struct nodeOneOld));
    // arrayTwoOld = (struct nodeOneOld*) malloc(n*sizeof(struct nodeOneOld));

    //  struct nodeOneOld* arrayOneOld = aligned_alloc(64,sizeof(struct nodeOneOld) * n); //no difference
    // struct nodeOneOld* arrayTwoOld = aligned_alloc(64,sizeof(struct nodeOneOld) * n); //no difference

    // struct nodeOneOld arrayOneLocal[n];
    // struct nodeOneOld arrayTwoLocal[n];

    populateNodeOneOld(n);

    int i = (rand() % n);
    int j = (rand() % 67);

    arrayOneOld[i].a = 1;
    arrayOneOld[i].b = 10.0;
    arrayOneOld[i].c = 9;
    // arrayOneLocal[i].d = 23.0;
    // arrayOneLocal[i].e = 'a';
    // arrayOneLocal[i].f = 23.0; -
    // arrayOneLocal[i].g = 23.0;

    arrayTwoOld[i].a = 1;
    arrayTwoOld[i].b = 10.0;
    arrayTwoOld[i].c = 9;
    // arrayTwoLocal[i].d = 23.0;
    // arrayTwoLocal[i].e = 'a';
    // arrayTwoLocal[i].f = 23.0;
    // arrayTwoLocal[i].g = 23.0;

    multNodeOneOld(n);
    multArrays(n);

    //validity check
    printf("Validity check\n");

    printf("\na: ---\n%d",arrayOneOld[i].a);
    printf("\nb: ---\n%f",arrayOneOld[i].b);
    printf("\nc: ---\n%d",arrayOneOld[i].c);
    printf("\nd: ---\n%lld",arrayOneOld[i].d);
    printf("\ne: ---\n%d",arrayOneOld[i].e);
    // printf("\nf: ---\n%f",arrayOneOld[i].f[j]);
    printf("\ng: ---\n%f",arrayOneOld[i].g);
    printf("\nh: ---\n%f",arrayOneOld[i].h);
    printf("\ni: ---\n%f",arrayOneOld[i].i);
    // printf("\nj: ---\n%f\n",arrayOneOld[i].j[2]);


    return 0;
}