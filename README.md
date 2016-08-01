# zabbixExtend

zabbixExtend是读取server端mysql数据库，查询server端未执行、执行成功、执行失败、正在执行、执行异常和已经执行的流程数目然后通过zabbix-sender发送给zabbix-server。

1.进入mysqlInstaller目录执行sh ./installMySql.sh安装mysql库文件。

2.以下命令写入run.sh文件

export LD_LIBRARY_PATH=/usr/lib/:LD_LIBRARY_PATH

./zabbixExtend "db" "192.168.XXX.XXX" 3306 "root" "pass" "SERVER-OLD" "localhost" 120 "2016-07-29 17:30:00" > /dev/null 2>&1 &

其中zabbixExtend是当前工程生成的可执行程序，db为要查询的数据库名，192.168.XXX.XXX为数据库所在的ip地址，3306为mysql端口号，root为mysql用户名，pass为mysql密码，SERVER-OLD为zabbix客户端主机名，localhost为zabbix-server地址，120为通过zabbix-sender发送数据的时间间隔，2016-07-29 17:30:00为查询的起始时间。

3.zabbix-server 展示接收到的数据

(1)打开zabbix web界面，登录。

(2)依次点击“配置”->“主机”。

(3)在列表中找到名字为“SERVER-OLD”的行，点击“监控项”链接。

(4)点击右上角“创建监控项”按钮。

(5)在“名称”输入名字“HTRD Task Not executed yet”。(根据实际情况命名)

(6)“类型”下拉列表选择“Zabbix 采集器”。

(7)“键值”输入框输入“htrd.key.task0”。(该值与zabbix-sender的参数“-k”对应)。

(8)点击“添加”，添加监控项。

(9)点击列表上面的“图形”链接，进入图形编辑界面。

(10)点击右上角“创建图形”按钮。

(11)“名称”输入名字“HTRD Task Not executed yet”。(根据实际情况命名)

(12)点击“监控项”后面的“添加”链接，勾选“HTRD Task Not executed yet”前的复选框，点击左下角的“选择”按钮。

(13)点击“添加”按钮。完成图形的添加。

3.通过命令行执行sh ./run.sh启动进程。
备注：运行环境为centos7,需要在该运行脚本的机器上安装zabbix-sender(sudo yum install zabbix-sender)。
