#ifndef CONFIG_H
#define CONFIG_H 1

#include <stdio.h>
#include <string.h>
#include <uci.h>
#include "recipient_list.h"

struct Topic
{
    char topic[100];
    int qos;
};

struct Event
{
    char topic[100];
    char parameter[255];
    int value_type;
    int comparison_type;
    char comparison_value[255];
    struct recipient_list *recipients;
};

struct Email
{
    char smtp_ip[255];
    int smtp_port;
    char username[255];
    char password[255];
    char sender_email[255];
};

struct Topic *get_topics();
struct Event *get_events();
struct Email *get_emails();

#endif