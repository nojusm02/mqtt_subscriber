#ifndef RECIPIENT_LIST_H
#define RECIPIENT_LIST 1

#include <stdio.h>
#include <stdlib.h>

struct recipient_list
{
    char email[100];
    struct recipient_list *next;
};

void add_recipient(struct recipient_list **head, struct recipient_list *mail);
void destroy_recipient_list(struct recipient_list *list);

#endif