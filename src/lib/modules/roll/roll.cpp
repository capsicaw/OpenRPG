/*
roll - roll.cpp
Created on: Nov 10, 2016

License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
*/
#include <iostream>
#include <getopt.h>

#include "config.h"
#include "utils.h"
#include "die.h"

using namespace std;

static void print_version_flag() {
    fputs("roll (openrpg) " VERSION " - " COPYRIGHT "\n"
          "License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>\n"
          "This is free software: you are free to change and redistribute it.\n"
          "There is NO WARRANTY, to the extent permitted by law.\n\n",
          stdout);
    exit(EXIT_SUCCESS);
}

static void print_help_flag() {
    fputs("roll (openrpg) " VERSION " - " COPYRIGHT "\n"
          "License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>\n"
          "This is free software: you are free to change and redistribute it.\n"
          "There is NO WARRANTY, to the extent permitted by law.\n\n"
          "Usage: roll [options] XdY [+|-] AdB [+|-] N [...]\n"
                "\t-h --help                   Print this help screen\n"
                "\t-p --positive               Allow only positive results\n"
                "\t-v --version                Print version info\n"
                "\t-V --verbose                Verbose program output\n"
          "\n"
          "Long options may not be passed with a single dash.\n"
          "Report bugs to: <https://github.com/incomingstick/OpenRPG/issues>\n"
          "OpenRPG home page: <https://github.com/incomingstick/OpenRPG>\n"
          "General help using GNU software: <http://www.gnu.org/gethelp/>\n"
          "See 'man name-generator' for more information [TODO add man pages].\n",
          stdout);
    exit(EXIT_SUCCESS);
}

/* Option parser - parse_args(argc, argv)
    This function parses all cla's passed to argv. */
int parse_args(int argc, char* argv[], int* out) {
    int status = EXIT_SUCCESS;


    /* getopt_long stores the option and option index here */
    int opt, opt_ind;

    /* disables getopt printing to now be handled in '?' case */
    opterr = 0;

    /* these are the long cla's and their corresponding chars */
    static struct option long_opts[] = {
        {"help",    no_argument,        0,  'h'},
        {"positive",no_argument,        0,  'p'},
        {"version", no_argument,        0,  'v'},
        {"verbose", no_argument,        0,  'V'},
        {0,         required_argument,  0,   0},
        /* NULL row to terminate struct */
        {0,         0,                  0,   0}
    };

    while ((opt = getopt_long(argc, argv, "hpvV",
                               long_opts, &opt_ind)) != EOF) {
        string cmd("");

        switch (opt) {
        /* -h --help */
        case 'h':
            print_help_flag();
            break;

        /* -p --positive */
        case 'p':
            POS_FLAG = true;
            break;

        /* -v --version */
        case 'v':
            print_version_flag();
            break;

        /* -V --verbose */
        case 'V':
            VB_FLAG = true;
            verbose("verbose flag is set");
            QUIET_FLAG = false;
            break;
        
        /* parsing error */
        case '?':
            fprintf(stderr, "Error: unknown arguement %s\n", argv[optind]);
            print_help_flag();
            break;
        
        /* if we get here something very bad happened */
        default:
            status = verbose("Aborting...", EXIT_FAILURE);
        }
    }

    string expression;

    argc -= optind;
    argv += optind;         
  
    /* build expression string to parse */
    while(argc > 0) {

        expression += *argv;

        argc--;
        argv++;
    }

    if (expression_size > 0) {
    
        yy_scan_string(expression);

        yyparse();

    } else {
        printf("Error: No expression provided\n");
        print_help_flag();
    }

    return status;
}

int main(int argc, char* argv[]) {
    int out = 0;

    int status = verbose("parse_args completed", parse_args(argc, argv, &out));

    cout << out << endl;

	return verbose("exiting with status "+to_string(status), status);
}