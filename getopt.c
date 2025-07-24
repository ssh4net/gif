#include "getopt.h"
#include <string.h>
#include <stdio.h>

char *optarg = NULL;
int optind = 1;
int opterr = 1;
int optopt = '?';

int getopt(int argc, char *const argv[], const char *optstring)
{
    static int optpos = 1;
    const char *arg;
    (void)argc;

    if (optind >= argc || !argv[optind] || argv[optind][0] != '-' || !argv[optind][1])
        return -1;

    if (strcmp(argv[optind], "--") == 0) {
        optind++;
        return -1;
    }

    arg = argv[optind] + optpos;
    optopt = *arg;
    
    if (optopt == ':' || (optopt != ':' && !(optstring = strchr(optstring, optopt)))) {
        if (opterr)
            fprintf(stderr, "%s: invalid option -- '%c'\n", argv[0], optopt);
        
        if (!arg[1]) {
            optind++;
            optpos = 1;
        } else
            optpos++;
            
        return '?';
    }

    if (optstring[1] == ':') {
        if (arg[1]) {
            optarg = (char *)(arg + 1);
            optind++;
            optpos = 1;
            return optopt;
        }
        
        if (argc <= ++optind) {
            if (opterr)
                fprintf(stderr, "%s: option requires an argument -- '%c'\n", argv[0], optopt);
                
            optpos = 1;
            return optstring[2] == ':' ? ':' : '?';
        }
        
        optarg = (char *)argv[optind++];
    } else {
        if (!arg[1]) {
            optind++;
            optpos = 1;
        } else
            optpos++;
            
        optarg = NULL;
    }

    return optopt;
} 