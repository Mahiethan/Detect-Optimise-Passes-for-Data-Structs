#include <stdio.h>
#include <stdlib.h>

struct nodeOneNew
{
    //reordering by increasing alignment size (8, 4, 1, 1)
    //takes up 16 bytes - 16 / 8 = only two CPU cycles needed to access
    //also less memory taken up by struct
    double b;
    int a;
    char c;
    char d;
    //// 2 byte padding
};

// void populateNodeOneNew(struct nodeOneNew array[], int size)
void populateNodeOneNew(struct nodeOneNew* array, int size)
{
    for(int i = 0; i < size; i++)
    {
        array[i].a = 17.0;
        array[i].b = 10;
        array[i].c = 'c';
        array[i].d = 'd';
        //array[i].b = array[i].a + array[i].b;
    }
}

void multNodeOneNew(struct nodeOneNew* array, int size)
{
    for(int i = 0; i < size; i++)
    {
        array[i].a = (array[i].b * array[i].a) / (100 / 5);
        array[i].b = (array[i].a - array[i].b) * (100 / 5);
    }
}

void multArrays(struct nodeOneNew* arrayOne, struct nodeOneNew* arrayTwo, int size)
{
    for(int j = 0; j < 70; j++)
    {
        for(int i = 0; i < size; i++)
        {
            arrayOne[i].a = arrayOne[i].a * arrayTwo[i].a;
            arrayOne[i].b = arrayOne[i].b * arrayTwo[i].b;
        }
    }
}

int main()
{
    // int n = 99999;
    int n = 999999;
    int i;

    for(i = 0; i < 100; i++)
    {
     ////static
    //  struct nodeOneNew arrayOneNew[n];
     ////dynamic
    struct nodeOneNew* arrayOneNew = (struct nodeOneNew*) malloc(n*sizeof(struct nodeOneNew));
    struct nodeOneNew* arrayTwoNew = (struct nodeOneNew*) malloc(n*sizeof(struct nodeOneNew));

    //  struct nodeOneNew* arrayOneNew = aligned_alloc(64,sizeof(struct nodeOneNew) * n); //no difference
    //  struct nodeOneNew* arrayTwoNew = aligned_alloc(64,sizeof(struct nodeOneNew) * n); //no difference



     populateNodeOneNew(arrayOneNew,n);
    populateNodeOneNew(arrayTwoNew,n);

     multNodeOneNew(arrayOneNew,n);
     multArrays(arrayOneNew,arrayTwoNew,n);

    if(i == 79)
    {
        printf("Validity check\n");
        // for(int j = 5000; j < 5010; j++)
        // {
            printf("a: %d\n---\n",arrayOneNew[1].a);
            printf("b: %f\n---\n",arrayOneNew[1].b);
            printf("c: %c\n---\n",arrayOneNew[1].c);
            printf("d: %c\n---\n",arrayOneNew[1].d);
        //}
    }

     free(arrayOneNew);
    free(arrayTwoNew);
    }

    return 0;
}