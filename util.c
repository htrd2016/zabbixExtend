#include <stdio.h>
#include "util.h"

MYSQL *mysql = 0;
int connect_db(const char* host, int nPort, const char* szDatabase, const char* user, const char* passwd)
{
    printf("connect_db ..... \n");

    mysql = mysql_init(NULL);

    if (mysql == 0)
    {
        fprintf(stderr, "%s\n", mysql_error(mysql));
        return -1;
    }

    if (mysql_real_connect(mysql, host, user, passwd, szDatabase, nPort, NULL, 0) == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(mysql));
        mysql_close(mysql);
        return -1;
    }

    printf("connect mysql sucess!\n");
    return (0);
}

void disconnect_db(void)
{
    if (mysql)
        mysql_close(mysql);

    printf("disconnected_db\n");
}

int get_current_day_str(char **out)
{
  time_t timep;
  struct tm *p;
  time(&timep); /*获得time_t结构的时间，UTC时间*/
  p = gmtime(&timep); /*转换为struct tm结构的UTC时间*/
  //sprintf((char*)(*out), "%d-%02d-%02d", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday);
  sprintf((char*)(*out), "%d-%02d-%02d %02d:%02d:%02d", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday,
         p->tm_hour, p->tm_min, p->tm_sec);
  printf("%s", *out);
  return 0;  
}
