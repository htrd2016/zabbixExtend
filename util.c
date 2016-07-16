#include "util.h"

MYSQL *mysql = 0;
int connect_db(const char* host, int nPort, const char* szDatabase, const char* user, const char* passwd)
{
    //printf("connect_db ..... \n");

    mysql = mysql_init(NULL);

    if (mysql == 0)
    {
        fprintf(stderr, "%s\n", mysql_error(mysql));
        //exit(2000);
        return -1;
    }

    if (mysql_real_connect(mysql, host, user, passwd, szDatabase, nPort, NULL, 0) == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(mysql));
        mysql_close(mysql);
        return -1;
    }

    //printf("OK!\n");
    return (0);
}

void disconnect_db(void)
{
    if (mysql)
        mysql_close(mysql);

    //printf("\ndisconnected_db\n");
}
