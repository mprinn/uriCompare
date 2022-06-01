

#include "uri_compare.h"


bool charEq(char c1, char c2, bool caseInsensitive)
{
    if (c1 == c2)
        return true;
    if (caseInsensitive)        
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

const char colon = ':';
const char slash = '/';

bool compareScheme(char *uri1, char *uri2, size_t *schemeLen)
{
    size_t sLen1 = 0, sLen2 = 0;
    char *tmp;
    char scheme1[MAX_SCHEME_LEN];
    char scheme2[MAX_SCHEME_LEN];

    // Get scheme 1
    tmp = memchr(uri1, colon, strlen(uri1));
    sLen1 = tmp - uri1;
    tmp = memchr(uri2, colon, strlen(uri2));
    sLen2 = tmp - uri2;
    if (sLen1 != sLen2)
        return false;

    strncpy(scheme1, uri1, sLen1);
    strncpy(scheme2, uri2, sLen2);

    
    for(int i = 0; i < sLen1; ++i)
    {
        // compare scheme
        // e.g. http
        if (!charEq(scheme1[i], scheme2[i], true))
        {
            return false;
        }
    }
    *schemeLen = sLen1;

    return true;
}

bool comparePorts(char *uri1, char *uri2)
{
    int port1 = 80;
    int port2 = 80;
    char *tmp = NULL;
    tmp = strstr(uri1, ":"); 
    if (tmp)
        port1 = atoi(++tmp);
    tmp = strstr(uri2, ":"); 
    if (tmp)
        port2 = atoi(++tmp);
    if (port1 != port2)
        return false;
    return true;
}

bool compareAuthority(char *uri1, char *uri2, size_t *authLen)
{
    size_t len1 = 0, len2 = 0;
    bool ret = false;
    char *tmp;
    char *auth1;
    char *auth2;
    tmp = memchr(uri1, slash, strlen(uri1));
    len1 = tmp - uri1;

    tmp = memchr(uri2, slash, strlen(uri2));
    len2 = tmp - uri2;

    auth1 = (char *)calloc(len1 + 1, 1);
    auth2 = (char *)calloc(len2 + 1, 1);

    strncpy(auth1, uri1, len1);
    strncpy(auth2, uri2, len2);

    if (!comparePorts(auth1, auth2))
    {
        goto fail;
    }
    

    tmp = memchr(uri1, colon, strlen(uri1));
    if (tmp)
    {
        len1 = (tmp - uri1);
    }
    tmp = memchr(uri2, colon, strlen(uri2));
    if (tmp)
    {
        len2 = (tmp - uri2);
    }
    if (len1 != len2)
    {
        goto fail;
    }
    for(int i = 0; i < len1; ++i)
    {
        // compare authority
        // e.g. foo.org
        if (!charEq(auth1[i], auth2[i], true))
        {
           goto fail;
        }
    }
    *authLen = len1;


    ret = true;
fail:    
    free(auth1);
    free(auth2);
    return ret;
}

bool compareURIs(char *uri1, char *uri2)
{
    size_t compLen = 0;
    
    
    if (!compareScheme(uri1, uri2, &compLen))
    {
        return false;
    }

    uri1 += compLen + 1;
    uri2 += compLen + 1;

    // check for //
    if (((uri1[0] != uri2[0]) || (uri1[1] != uri2[1]) || uri1[0] != '/'))
    {return false;}
    uri1 +=2;
    uri2 +=2;
    // check for host / port
    // //foo:80
    if (!compareAuthority(uri1, uri2, &compLen))
    {
        return false;
    }

    
    
    // case sensitive check rest 
    return true;
}