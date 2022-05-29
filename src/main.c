#include "uri_compare.h"
int main(int argc, char** argv)
{
    printf("return val %d\n", compareURIs("http://foo:80/bar" ,"http://FOO/bar"));
    return 0;
}