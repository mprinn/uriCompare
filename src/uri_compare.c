#include "uri_compare.h"

const char colon = ':';
const char slash = '/';

static bool charEq(char c1, char c2, bool caseInsensitive)
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


static bool compareScheme(char *uri1, char *uri2, size_t *schemeLen)
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

static bool comparePorts(char *uri1, char *uri2)
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

static bool compareAuthority(char *uri1, char *uri2, size_t *authLen1, size_t *authLen2)
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
    
    *authLen1 = len1;
    *authLen2 = len2;
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



    ret = true;
fail:    
    free(auth1);
    free(auth2);
    return ret;
}

static char convertHex(char *in)
{
    return (char)strtol(in, NULL, 16);
}

static void escapeHex(char **in)
{
    
    size_t len = strlen(*in);
    char hex[2], *str;
    str = *in;
    for (int i = 0; i < len; ++i)
    {
        if (str[i] == '%')
        {
            memcpy(hex, &str[i + 1], 2);

            str[i] = convertHex(hex);
            
            for (int j = i + 1; j < len - 1; ++j)
            {
                str[j] = str[j + 2];
            }
            len -= 2;
        }
    }
}

bool compareURIs(char *uri1, char *uri2)
{
    size_t compLen = 0;
    size_t compLen2 = 0;
    
    
    escapeHex(&uri1);
    escapeHex(&uri2);

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
    if (!compareAuthority(uri1, uri2, &compLen, &compLen2))
    {
        return false;
    }

    uri1 += compLen;
    uri2 += compLen2;

    // case sensitive check rest 
    compLen = strlen(uri1);
    compLen2 = strlen(uri1);

    if (compLen != compLen2)
        return false;
    for (int i = 0; i < compLen; ++i)
    {
        if (!charEq(uri1[i], uri2[i], false))
        {
           return false;
        }
    }
    return true;
}