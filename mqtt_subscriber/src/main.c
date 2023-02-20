#include <mosquitto.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sqlite3.h>
#include <signal.h>
#include <json-c/json.h>
#include <syslog.h>

#include "db.h"
#include "become_daemon.h"
#include "argsp.h"
#include "config.h"
#include "email.h"

volatile int daemonize = 1;

struct Topic *tp;
struct Event *events;
struct Email *email;

void sig_handler(int signum)
{
    daemonize = 0;
    syslog(LOG_NOTICE, "Terminating program\n");
}

void execute_topic_events(const char *topic, const char *parameter, const char *value, enum json_type value_type)
{
    int k = 0;
    while (events[k].value_type != -1 && events != NULL)
    {
        char header[1000];
        sprintf(header, "Event topic: %s\nEvent parameter: %s\n", events[k].topic, events[k].parameter);
        if (strcmp(topic, events[k].topic) == 0 && strcmp(parameter, events[k].parameter) == 0)
        {
            char msg[MSG_SIZE];
            sprintf(msg, "Event topic: %s\n Event parameter %s\n", events[k].topic, events[k].parameter);

            if (events[k].value_type == 2 && value_type == json_type_string)
            {
                // ==
                if (events[k].comparison_type == 1)
                {
                    if (strcmp(value, events[k].comparison_value) == 0)
                    {
                        sprintf(msg, "%sEVENT SYMBOL: == STRINGS\nEvent value: %s\nReceived value: %s", header, events[k].comparison_value, value);
                        printf(msg);
                        send_email(msg, events[k].recipients, email);
                        db_insert_msg(events[k].topic, msg);
                    }
                }
                // !=
                else if (events[k].comparison_type == 2)
                {
                    if (strcmp(value, events[k].comparison_value) != 0)
                    {
                        sprintf(msg, "%sEVENT SYMBOL: != STRINGS\nEvent value: %s\nReceived value: %s", header, events[k].comparison_value, value);
                        printf(msg);
                        send_email(msg, events[k].recipients, email);
                        db_insert_msg(events[k].topic, msg);
                    }
                }
            }

            else if (events[k].value_type == 1 && (value_type == json_type_int || value_type == json_type_double))
            {
                double event_value_converted = atof(events[k].comparison_value);
                double received_value_converted = atof(value);

                // ==
                if (events[k].comparison_type == 1)
                {
                    if (received_value_converted == event_value_converted)
                    {
                        sprintf(msg, "%sEVENT SYMBOL: ==\nEvent value: %f\nReceived value: %f", header, event_value_converted, received_value_converted);
                        printf(msg);
                        send_email(msg, events[k].recipients, email);
                        db_insert_msg(events[k].topic, msg);
                    }
                }
                // !=
                else if (events[k].comparison_type == 2)
                {
                    if (received_value_converted != event_value_converted)
                    {

                        sprintf(msg, "%sEVENT SYMBOL: !=\nEvent value: %f\nReceived value: %f", header, event_value_converted, received_value_converted);
                        printf(msg);
                        send_email(msg, events[k].recipients, email);
                        db_insert_msg(events[k].topic, msg);
                    }
                }
                // >
                else if (events[k].comparison_type == 3)
                {
                    if (received_value_converted > event_value_converted)
                    {
                        sprintf(msg, "%sEVENT SYMBOL: >\nEvent value: %f\nReceived value: %f", header, event_value_converted, received_value_converted);
                        printf(msg);
                        send_email(msg, events[k].recipients, email);
                        db_insert_msg(events[k].topic, msg);
                    }
                }

                // >=
                else if (events[k].comparison_type == 4)
                {
                    if (received_value_converted >= event_value_converted)
                    {
                        sprintf(msg, "%sEVENT SYMBOL: >= \nEvent value: %f\nReceived value: %f", header, event_value_converted, received_value_converted);
                        printf(msg);
                        send_email(msg, events[k].recipients, email);
                        db_insert_msg(events[k].topic, msg);
                    }
                }

                // <
                else if (events[k].comparison_type == 5)
                {
                    if (received_value_converted < event_value_converted)
                    {
                        sprintf(msg, "%sEVENT SYMBOL: < \nEvent value: %f\nReceived value: %f", header, event_value_converted, received_value_converted);
                        printf(msg);
                        send_email(msg, events[k].recipients, email);
                        db_insert_msg(events[k].topic, msg);
                    }
                }

                // <=
                else if (events[k].comparison_type == 6)
                {
                    if (received_value_converted <= event_value_converted)
                    {
                        sprintf(msg, "%sEVENT SYMBOL: <=\nEvent value: %f\nReceived value: %f", header, event_value_converted, received_value_converted);
                        printf(msg);
                        send_email(msg, events[k].recipients, email);
                        db_insert_msg(events[k].topic, msg);
                    }
                }
            }
        }
        k++;
    }
}

int check_for_topic(const char *topic)
{
    int k = 0;
    while (tp[k].qos != -1 && tp != NULL)
    {
        if (strcmp(topic, tp[k].topic) == 0)
        {
            return k;
        }
        k++;
    }
    return -1;
}

void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
{
    int topic_exists = check_for_topic(msg->topic);

    printf("\nTopic: %s\nQoS: %d\nMessage: %s\n\n", msg->topic, tp[topic_exists].qos, (char *)msg->payload);

    if (topic_exists != -1)
    {

        struct json_object *jobj;

        jobj = json_tokener_parse((char *)msg->payload);

        const char *parameter = json_object_get_string(json_object_object_get(jobj, "parameter"));
        const char *value = json_object_get_string(json_object_object_get(jobj, "value"));
        enum json_type value_type = json_object_get_type(json_object_object_get(jobj, "value"));

        if (parameter == NULL || value == NULL)
        {
            fprintf(stderr, "ERROR: parameter or value is NULL\n");
            return;
        }

        printf("\nparameter: %s\n", parameter);
        printf("value: %s\n", value);
        printf("value_type: %d\n", value_type);

        execute_topic_events(msg->topic, parameter, value, value_type);
        printf("\n");
    }
}

int main(int argc, char *argv[])
{

    struct mosquitto *mosq;

    signal(SIGINT, sig_handler);

    struct arguments options = {"-", 0, "-", "-", "-"};

    int rc = 0;
    int keep = 100;

    openlog(NULL, LOG_PID, LOG_USER);

    rc = db_connect();
    if (rc)
    {
        syslog(LOG_ERR, "Failed to connect to the database\n");
        goto EXIT;
    }

    rc = parse_args(argc, argv, &options);
    if (rc)
    {
        syslog(LOG_ERR, "Failed to parse arguments\n");
        goto EXIT;
    }
    else
    {
        syslog(LOG_INFO, "Sucessfuly parsed arguments\n");
    }

    tp = get_topics();
    events = get_events();
    email = get_emails();

    mosquitto_lib_init();

    mosq = mosquitto_new(NULL, 1, NULL);

    if (mosq == NULL)
    {
        syslog(LOG_ERR, "ERROR: Out of memory or invalid parameters\n");
        goto EXIT;
    }

    rc = mosquitto_username_pw_set(mosq, options.username, options.password);
    if (rc != MOSQ_ERR_SUCCESS)
    {
        syslog(LOG_ERR, "ERROR: %s\n", mosquitto_strerror(rc));
        goto EXIT;
    }

    if (strcmp(options.cert_file_path, "-") != 0)
    {
        rc = mosquitto_tls_set(mosq, options.cert_file_path, NULL, NULL, NULL, NULL);
        if (rc != MOSQ_ERR_SUCCESS)
        {
            fprintf(stderr, "ERROR: Can not configure SSL\n");
            goto EXIT;
        }
    }

    rc = mosquitto_connect(mosq, options.host, options.port, keep);
    if (rc != MOSQ_ERR_SUCCESS)
    {
        syslog(LOG_ERR, "ERROR: %s\n", mosquitto_strerror(rc));
        goto EXIT;
    }
    else
    {
        syslog(LOG_INFO, "SUCCESS: Connected to broker\n");
    }

    if (tp != NULL)
    {
        int k = 0;
        while (tp[k].qos != -1)
        {
            rc = mosquitto_subscribe(mosq, NULL, tp[k].topic, tp[k].qos);
            if (rc != MOSQ_ERR_SUCCESS)
            {
                syslog(LOG_ERR, "ERROR: Can not subscribe topic (%s)\n", tp[k].topic);
            }
            else
            {
                syslog(LOG_INFO, "SUCCESS: subscribed to topic (%s)\n", tp[k].topic);
            }
            k++;
        }
    }
    else
    {
        syslog(LOG_ERR, "ERROR: No topics!\n");
    }

    mosquitto_message_callback_set(mosq, on_message);
    syslog(LOG_INFO, "Message callback set !");

    rc = mosquitto_loop_start(mosq);
    if (rc != MOSQ_ERR_SUCCESS)
    {
        syslog(LOG_ERR, "ERROR: %s\n", mosquitto_strerror(rc));
        goto EXIT;
    }
    else
    {
        syslog(LOG_INFO, "SUCESS: MOSQUITTO loop started");
    }
    if (options.daemonize == 1)
    {
        rc = become_daemon(2);
        if (rc != 0)
        {
            syslog(LOG_ERR, "ERROR : failed to deamonize");
            goto EXIT;
        }
    }

    while (daemonize)
    {
    }

EXIT:
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    db_disconnect();
    syslog(LOG_INFO, "Closing logs");
    closelog();

    return rc;
}