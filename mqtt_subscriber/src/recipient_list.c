#include "recipient_list.h"

void add_recipient(struct recipient_list **head, struct recipient_list *mail)
{
    mail->next = NULL;
    struct recipient_list *temp = *head;
    if (temp == NULL)
    {
        *head = mail;
        return;
    }
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = mail;
}

void destroy_recipient_list(struct recipient_list *list)
{
    struct recipient_list *to_delete = list;
    while (list != NULL)
    {
        list = list->next;
        if (to_delete != NULL)
        {
            to_delete->next = NULL;
            free(to_delete);
        }
        to_delete = NULL;
        to_delete = list;
    }
}