#ifndef DB_H
#define DB_H 1

#include <sqlite3.h>
#include <syslog.h>
#include <stdio.h>

#define DB_PATH "/log/mqtt_subscriber.db"

int db_connect();
int db_insert_msg(char *topic, char *msg);
int db_disconnect();

#endif