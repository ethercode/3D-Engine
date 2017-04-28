typedef void (*VOID_VOID)(void);


void test(void)
{
}

void test2(void)
{
}

VOID_VOID GetFunction(int num)
{
    if (num == 1)
    {
        return test;
    }
    if (num == 2)
    {
        return test2;
    }
    return 0;
}

void main()
{
    VOID_VOID   f   = GetFunction(0);
    f();
}