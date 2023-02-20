#include "argsp.h"
#include <string.h>
#include <stdlib.h>

const char *argp_program_version =
    "1.0.0";

char doc[] =
    "MQTT subcriber";

struct argp_option options[] = {
    {"host", 'h', "host", 0, "host adress"},
    {"port", 'p', "port", 0, "port number"},
    {"user", 'u', "username", 0, "ussername"},
    {"pass", 'P', "password", 0, "password"},
    {"cafile", 'c', "certificate", 0, "certificate file"},
    {"daemonize", 'd', 0, 0, "Make a daemon process"},
    {0}};

struct argp argp = {options, parse_opt, NULL, doc};

error_t
parse_opt(int parameter, char *arg, struct argp_state *state)
{

    struct arguments *arguments = state->input;

    switch (parameter)
    {
    case 'h':
        arguments->host = arg;
        break;

    case 'p':
        arguments->port = atoi(arg);
        break;

    case 'u':
        arguments->username = arg;
        break;

    case 'P':
        arguments->password = arg;
        break;

    case 'c':
        arguments->cert_file_path = arg;
        break;
    case 'd':
        arguments->daemonize = 1;
        break;
    default:
        return ARGP_ERR_UNKNOWN;
    }

    return 0;
}

int parse_args(int argc, char *argv[], struct arguments *args)
{

    int rc = argp_parse(&argp, argc, argv, ARGP_NO_EXIT, 0, args);

    const char *empty = "-";

    if (!strcmp(args->host, empty) || args->port == 0)
        rc = 1;

    return rc;
}