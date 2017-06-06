#include <stdio.h>

struct range {
    int lo;
    int hi;
};

void print_substr(const char* s, struct range* r)
{
    int i;
    for (i = r->lo; i < r->hi; i++)
        putchar(s[i]);
    putchar('\n');
}

int main()
{
    struct range r;
    r.lo = 1;
    r.hi = 8;
    print_substr("hello_world", &r);
    return 0;
}
