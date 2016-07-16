#ifndef UTIL_H
#define UTIL_H

#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
extern MYSQL *mysql;

int connect_db(const char* host, int nPort, const char* szDatabase, const char* user, const char* passwd);
void disconnect_db(void);

#endif // UTIL_H

