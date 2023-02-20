#ifndef EMAIL_H
#define EMAIL_H 1

#define MSG_SIZE 10000

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include "config.h"

int send_email(char *message, struct recipient_list *recipients, struct Email *email);

#endif