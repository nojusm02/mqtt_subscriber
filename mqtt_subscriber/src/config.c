#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <uci.h>

#include "config.h"

const char *config_name = "mqtt_subscriber";
const char *email_config_name = "mqtt_subscriber_mail";

struct Topic *get_topics()
{

    struct uci_context *context = uci_alloc_context();
    struct uci_package *package;

    if (uci_load(context, config_name, &package) != UCI_OK)
    {
        uci_perror(context, "uci_load : topics");
        uci_free_context(context);
        return NULL;
    }

    int k = 0;
    struct Topic *topics;
    topics = malloc(sizeof(struct Topic) * 1);

    struct uci_element *i, *j;
    uci_foreach_element(&package->sections, i)
    {
        struct uci_section *section = uci_to_section(i);

        if (strcmp(section->type, "topic") == 0)
        {
            if (k >= 1)
            {
                topics = (struct Topic *)realloc(topics, sizeof(struct Topic) * (k + 1));
            }

            uci_foreach_element(&section->options, j)
            {
                struct uci_option *option = uci_to_option(j);
                char *option_name = option->e.name;

                if (strcmp(option_name, "topic") == 0)
                {
                    strcpy(topics[k].topic, option->v.string);
                }
                else if (strcmp(option_name, "qos") == 0)
                {
                    topics[k].qos = atoi(option->v.string);
                }
            }
            k++;
        }
    }

    uci_free_context(context);

    if (k == 0)
    {
        return NULL;
    }
    else
    {
        topics = (struct Topic *)realloc(topics, sizeof(struct Topic) * (k + 1));
        topics[k].qos = -1;
    }

    return topics;
}

struct Event *get_events()
{

    struct uci_context *context = uci_alloc_context();
    struct uci_package *package;

    if (uci_load(context, config_name, &package) != UCI_OK)
    {
        uci_perror(context, "uci_load : events");
        uci_free_context(context);
        return NULL;
    }

    int k = 0;
    struct Event *events;
    events = malloc(sizeof(struct Event) * 1);

    struct uci_element *i, *j;
    uci_foreach_element(&package->sections, i)
    {
        struct uci_section *section = uci_to_section(i);

        if (strcmp(section->type, "event") == 0)
        {
            if (k >= 1)
            {
                events = (struct Event *)realloc(events, sizeof(struct Event) * (k + 1));
            }

            uci_foreach_element(&section->options, j)
            {
                struct uci_option *option = uci_to_option(j);
                char *option_name = option->e.name;

                if (strcmp(option_name, "topic") == 0)
                {
                    strcpy(events[k].topic, option->v.string);
                }
                else if (strcmp(option_name, "parameter") == 0)
                {
                    strcpy(events[k].parameter, option->v.string);
                }
                else if (strcmp(option_name, "valuetype") == 0)
                {
                    events[k].value_type = atoi(option->v.string);
                }
                else if (strcmp(option_name, "comparisontype") == 0)
                {
                    events[k].comparison_type = atoi(option->v.string);
                }
                else if (strcmp(option_name, "comparisonvalue") == 0)
                {
                    strcpy(events[k].comparison_value, option->v.string);
                }
                else if (strcmp(option_name, "recipients") == 0)
                {
                    struct uci_element *element;
                    struct recipient_list *list = NULL;
                    uci_foreach_element(&option->v.list, element)
                    {
                        struct recipient_list *temp = malloc(sizeof(struct recipient_list));
                        strcpy(temp->email, element->name);
                        add_recipient(&list, temp);
                    }
                    events[k].recipients = list;
                }
            }
            k++;
        }
    }

    uci_free_context(context);

    if (k == 0)
    {
        return NULL;
    }
    else
    {
        events = (struct Event *)realloc(events, sizeof(struct Event) * (k + 1));
        events[k].value_type = -1;
    }

    return events;
}

struct Email *get_emails()
{

    struct uci_context *context = uci_alloc_context();
    struct uci_package *package;

    // It is also possible to load the config with uci_lookup_ptr(),
    // or from a file stream using uci_import()
    if (uci_load(context, email_config_name, &package) != UCI_OK)
    {
        uci_perror(context, "uci_load()");
        uci_free_context(context);
        return NULL;
    }

    int k = 0;
    struct Email *emails;
    emails = malloc(sizeof(struct Email) * 1);

    struct uci_element *i, *j; // Iteration variables
    uci_foreach_element(&package->sections, i)
    {
        struct uci_section *section = uci_to_section(i);

        if (strcmp(section->type, "email") == 0)
        {
            if (k >= 1)
            {
                emails = (struct Email *)realloc(emails, sizeof(struct Email) * (k + 1));
            }

            uci_foreach_element(&section->options, j)
            {
                struct uci_option *option = uci_to_option(j);
                char *option_name = option->e.name;

                if (strcmp(option_name, "smtp_ip") == 0)
                {
                    strcpy(emails[k].smtp_ip, option->v.string);
                }
                else if (strcmp(option_name, "smtp_port") == 0)
                {
                    emails[k].smtp_port = atoi(option->v.string);
                }
                else if (strcmp(option_name, "username") == 0)
                {
                    strcpy(emails[k].username, option->v.string);
                }
                else if (strcmp(option_name, "password") == 0)
                {
                    strcpy(emails[k].password, option->v.string);
                }
                else if (strcmp(option_name, "senderemail") == 0)
                {
                    strcpy(emails[k].sender_email, option->v.string);
                }
            }
            k++;
        }
    }

    uci_free_context(context);

    if (k == 0)
    {
        return NULL;
    }
    else
    {
        emails = (struct Email *)realloc(emails, sizeof(struct Email) * (k + 1));
    }

    return emails;
}