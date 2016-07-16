#include <stdio.h>
#include "util.h"

extern MYSQL *mysql;

int query_count(char **data, char *sql);
int main(int argc, char *argv[])
{
  char out[1024];
  char mysql_db[512];
  char mysql_ip[50];
  int mysql_port;
  char mysql_user_name[512];
  char mysql_user_pass[512];

  char json[1024];

  if(argc<6)
  {
     printf("<path><mysql db name><mysql server ip><mysql srever port><mysql user name><mysql password>\n");
     return -1;
  }

  strcpy(mysql_db, argv[1]);
  strcpy(mysql_ip, argv[2]);
  mysql_port = atoi(argv[3]);
  strcpy(mysql_user_name, argv[4]);
  strcpy(mysql_user_pass, argv[5]);
  

  strcpy(mysql_db, "asdb");
  strcpy(mysql_ip, "192.168.1.253");
  mysql_port = atoi("3306");
  strcpy(mysql_user_name, "root");
  strcpy(mysql_user_pass, "");

  //printf("%s %s %d %s %s", mysql_db, mysql_ip, mysql_port, mysql_user_name, mysql_user_pass);

  if(0!=connect_db(mysql_ip, mysql_port, mysql_db, mysql_user_name, mysql_user_pass))
  {
        printf("connect mysql faild: server=%s,port=%d,username=%s,database=%s\n",
            mysql_ip, mysql_port, mysql_user_name, mysql_db);
        return -1;
  }

  int count0 = query_count((char**)(&out), "SELECT count(1) from k_scheduler WHERE State=0;");
  //printf("count0=%d\n", count0);

  int count1 = query_count((char**)(&out), "SELECT count(1) from k_scheduler WHERE State=1;");
  //printf("count1=%d\n", count1);

  int count2 = query_count((char**)(&out), "SELECT count(1) from k_scheduler WHERE State=2;");
  //printf("count2=%d\n", count2);

  int count3 = query_count((char**)(&out), "SELECT count(1) from k_scheduler WHERE State=3;");
  //printf("count3=%d\n", count3);
  
  int count4 = query_count((char**)(&out), "SELECT count(1) from k_scheduler WHERE State=4;");
  //printf("count4=%d\n", count4);

  sprintf(json, "{\n \"data\":[\n{ \"{#PORT}\":\"%d\", \"{#PROTO}\":\"TASK_UNDO\" },\n{ \"{#PORT}\":\"%d\", \"{#PROTO}\":\"TASK_SUCESSED\" }, \n{ \"{#PORT}\":\"%d\", \"{#PROTO}\":\"TASK_FAILD\" }, \n{ \"{#PORT}\":\"%d\", \"{#PROTO}\":\"TASK_DOING\" },\n{ \"{#PORT}\":\"%d\", \"{#PROTO}\":\"TASK_UNNORMAL\" }\n ] \n}",count0, count1,count2,count3,count4);

  puts(json);
  fflush(stdout);
  disconnect_db();
  return 0;
}


int query_count(char **data, char *sql)
{
  MYSQL_RES *res_ptr;
  int num_row;
  MYSQL_ROW mysql_row;
  int nRet = 0;

  int res = mysql_query(mysql, sql);
  if (res)
  {
     fprintf(stderr, "SELECT error: %s\n", mysql_error(mysql));
     return -1;
  }
  else
  {
     res_ptr = mysql_store_result(mysql);
     if (res_ptr)
     {
       num_row = mysql_num_rows(res_ptr);
       //printf("count=%d\n", num_row);
       if(num_row<=0)
       {
         return -1;
       }
       mysql_row = mysql_fetch_row(res_ptr);
       //printf("%s\n", mysql_row[0]);
       nRet = atoi(mysql_row[0]);

       if (mysql_errno(mysql))
       {
         fprintf(stderr, "Retrive error: %s\n", mysql_error(mysql));
       }
       mysql_free_result(res_ptr);
    }
  }
  return nRet;
}

