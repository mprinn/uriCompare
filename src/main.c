#include "uri_compare.h"
int main(int argc, char** argv)
{
    printf("return val %d\n", compareURIs("Http:%2F%2Ffoo:80/bare" ,"http://FOO/bar%65"));
    return 0;
}