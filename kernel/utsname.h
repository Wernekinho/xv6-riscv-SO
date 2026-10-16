#ifndef _UTSNAME_H_
#define _UTSNAME_H_

#define UTSLEN 65

struct utsname {
    char sysname[UTSLEN];
    char nodename[UTSLEN];
    char release[UTSLEN];
    char version[UTSLEN];
    char machine[UTSLEN];
};

#endif