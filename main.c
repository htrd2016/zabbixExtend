#include <stdio.h>
#include <unistd.h>
#include "util.h"

extern MYSQL *mysql;

extern int zabbix_send(int count, char *key, char *server_ip, char *host_name);
extern int query_count(char **data, char *sql);
extern char *get_sql(int nIndex, char *current_day, char **out);

int main(int argc, char *argv[])
{
  char out[1024];
  char mysql_db[512];
  char mysql_ip[50];
  int mysql_port;
  char mysql_user_name[512];
  char mysql_user_pass[512];
  char server_ip[128];
  char host_name[128];
  int time_interval;

  char current_day[512];
  char *pcurrent_day = current_day;
  char sql[1024];
  char *psql = sql;
  int count = 0;
 
  if (argc<9)
  {
     printf("<path><mysql db name><mysql server ip><mysql srever port><mysql user name><mysql password><zbbix hostname in zabbix_agent.conf><server ip><send time interval(sencond)>\n");
     return -1;
  }

  strcpy(mysql_db, argv[1]);
  strcpy(mysql_ip, argv[2]);
  mysql_port = atoi(argv[3]);
  strcpy(mysql_user_name, argv[4]);
  strcpy(mysql_user_pass, argv[5]);
  strcpy(host_name, argv[6]);
  strcpy(server_ip, argv[7]);
  time_interval = atoi(argv[8]);
  
  printf("%s %s %d %s %s %s %d", mysql_db, mysql_ip, mysql_port, mysql_user_name, mysql_user_pass, host_name, time_interval);

  if (0!=connect_db(mysql_ip, mysql_port, mysql_db, mysql_user_name, mysql_user_pass))
  {
        printf("connect mysql faild: server=%s,port=%d,username=%s,database=%s\n",
            mysql_ip, mysql_port, mysql_user_name, mysql_db);
        return -1;
  }
  get_current_day_str((char**)(&pcurrent_day));

  for (;;)
  {
    //sprintf(data, "zabbix_sender   -z localhost   -s \"Zabbix server\"   -k \"test.timestamp\"  -o %d\n", random()%100);

    printf("\n-----------------------------\n");

    get_sql(0, current_day, &psql);
    count = query_count((char**)(&out), psql);
    zabbix_send(count, "htrd.key.task0", server_ip, host_name);

    get_sql(1, current_day, &psql);
    count = query_count((char**)(&out), psql);
    zabbix_send(count, "htrd.key.task1", server_ip, host_name);
 
    get_sql(2, current_day, &psql);
    count = query_count((char**)(&out), psql);
    zabbix_send(count,"htrd.key.task2", server_ip, host_name);
 
    get_sql(3, current_day, &psql);
    count = query_count((char**)(&out), psql);
    zabbix_send(count, "htrd.key.task3", server_ip, host_name);
   
    get_sql(4, current_day, &psql);
    count = query_count((char**)(&out), psql);
    zabbix_send(count, "htrd.key.task4", server_ip, host_name);
 
    //sprintf(sql, "select count(*) from k_flowrecord where STime >= '%s';", current_day);
    sprintf(sql, "SELECT COUNT(*) FROM k_flowrecord kf JOIN k_scheduler ks on kf.ID=ks.FlowRecordID where kf.STime >= '%s' AND ks.State in (1,2,4);",  current_day);
    count = query_count((char**)(&out), sql);
    zabbix_send(count, "htrd.key.task5", server_ip, host_name);
    
    sleep(time_interval);
  }
  disconnect_db();
  return 0;
}

char *get_sql(int nIndex, char *current_day, char **out)
{
  sprintf((*out), "SELECT count(*) from k_scheduler WHERE State=%d and CONCAT(FDate,' ',FTime)>='%s';", nIndex, current_day);
  return *out;
}

int zabbix_send(int count, char *key, char *server_ip, char *host_name)
{
  char data[1024];
  sprintf(data, "zabbix_sender   -z %s   -s \"%s\"   -k \"%s\"  -o %d\n", server_ip, host_name, key, count);
  printf("%s", data);
  int ret = system(data);  //调用shell命令 ls -l
  printf("ret = %d\n",ret);
  return 0;
}

int query_count(char **data, char *sql)
{
  MYSQL_RES *res_ptr;
  int num_row;
  MYSQL_ROW mysql_row;
  int nRet = 0;
  
  printf("%s\n", sql);
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


