

#include "uri_compare.h"


bool charEq(char c1, char c2, bool sensitive)
{
    if (c1 == c2)
        return true;
    if (sensitive)        
    {
        char tmpc1 = c1;
        char tmpc2 = c2;
        if (c1 > 'Z')
            tmpc1 = c1 - 32;
        if (c2 > 'Z')
            tmpc2 = c2 - 32;
        if (tmpc1 == tmpc2)
            return true;
        
    }
    return false;
}
bool compareURIs(char *uri1, char *uri2)
{
    int colonFound = 0;
    int slashFound = 0;
    
    while (*uri1 && *uri2)
    {
        // compare first part
        // e.g. http://
        if (!charEq(*uri1, *uri2, true))
        {
            return false;
        }
        if (*uri1 == ':')
            colonFound++;
        uri1++;
        uri2++;
        if (colonFound == 1)
            break;
    }

    if (colonFound != 1)
        return false;
    // check for host / port
    // //foo:80
    const char colon = ':';
    const char slash = '/';
    int port1 = 80;
    int port2 = 80;
    memchr(str, colon, strlen(uri1));
    memchr(str, colon, strlen(uri1));
    if (!charEq(*uri1, *uri2, true));
        return false;
    if (*uri1 == '/')
        slashFound++;
    uri1++;
    uri2++;
    if (slashFound == 3)
        break;
    if (!strstr(uri1, ":"))
        port1 = atoi("60");
    if (!strstr(uri2, ":"))
        port2 = atoi("60");
        // compare ports
    return true;
    
    // case sensitive check rest 
}