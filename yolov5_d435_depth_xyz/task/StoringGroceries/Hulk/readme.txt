/*******************************************************/
/                                                       
/ pip install HulkRobot-0.0.2-py3-none-any.whl
/ 包名HulkRobot(pip uninstall HulkRobot)
/                                                       
/*******************************************************/
/                                                       
/ __init__中(一般位于/local/lib/python3.*/site-packages/HulkRobot/__init__.py)
/
/ def open_server(self):中self.__server_ip__修改成当前主机IP，如"192.168.10.102"
/
/ def scan(self):中__tcp_client.connect(('192.168.10.101', 8081))，中的IP为机器人
/ 的IP。使用TJArk_OPL连接时不用更改。
/                                                       
/*******************************************************/
/ 