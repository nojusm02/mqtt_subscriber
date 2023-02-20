#ifndef ARGSP_H
#define ARGSP_H 1
#include <argp.h>

struct arguments
{
    char *host;
    int port;
    char *username;
    char *password;
    char *cert_file_path;
    int daemonize;
};

error_t
parse_opt(int parameter, char *arg, struct argp_state *state);

struct argp argp;

int parse_args(int argc, char *argv[], struct arguments *args);

#endif