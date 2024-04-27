struct nodeOne
{
    int a;
};

int main()
{
    struct nodeOne test;
    struct nodeOne testTwo[1]; //potential AoS - not populated/used

    return 0;
}