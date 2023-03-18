from HulkRobot import *
import time

eventManager = EventManager()
hulk = HulkRobot(eventManager)
bConnect = False
robot_IO_input = 0
robot_IO_output = 0
robot_move_finished_flag = 0
robot_disconnected_flag = 0
robot_online_finished_flag = 0
robot_car_x = 200
robot_car_y = 0
robot_car_z = 100


def new_connection(event):
    print(event.dict["ClientID"], ' connected...')
    global bConnect
    bConnect = True
    thread = Thread(target=get_coord)
    thread.setDaemon(True)
    thread.start()


def disconnection(event):
    print(event.dict["ClientID"], ' disconnected...')
    global bConnect
    bConnect = False
    global robot_disconnected_flag
    robot_disconnected_flag = 1


def get_coord():
    while True:
        # print("coord thread running")
        hulk.request_coord()
        time.sleep(0.3)
        if not bConnect:
            break


def get_robot_data(event):
    if 'Basic' in event.dict["type"]:
        global robot_IO_output, robot_IO_input
        global robot_car_x, robot_car_y, robot_car_z
        robot_IO_output = event.dict["content"][9]
        robot_IO_input = event.dict["content"][10]
        robot_car_x = event.dict["content"][0]
        robot_car_y = event.dict["content"][2]
        robot_car_z = event.dict["content"][4]
        # print('receive: ', event.dict["type"], 'data:', event.dict["content"])
    else:
        print('receive: ', event.dict["type"], 'data:', event.dict["content"])


def get_robot_xyz():
    print("当前末端坐标：", robot_car_x, robot_car_y, robot_car_z)
    return robot_car_x, robot_car_y, robot_car_z


def get_IO_input():
    print(robot_IO_input)
    return robot_IO_input


def get_IO_output():
    print(robot_IO_output)
    return robot_IO_output


def get_robot_finished(event):
    if 'Movefinish' in event.dict["type"]:
        global robot_move_finished_flag
        robot_move_finished_flag = 1


def get_robot_set_online_finished(event):
    if 'Status' in event.dict["type"] and event.dict["content"][1] == '1':
        global robot_online_finished_flag
        robot_online_finished_flag = 1


def checkif_robot_error(event):
    if 'Error' in event.dict["type"] and event.dict["content"][0] == '1':
        print('！！！！！超限！！！！！')
        hulk.reset()
        print('！！！！正在归位！！！！')
        robot_go_home()


def robot_go_home():
    hulk.move(0, 200, 0, 30, -142, 6, 3)


def wait(flag):
    global robot_disconnected_flag
    global robot_move_finished_flag
    global robot_online_finished_flag
    if flag == 'robot_move_finished_flag':
        while True:
            time.sleep(0.05)
            if robot_move_finished_flag == 1:
                robot_move_finished_flag = 0
                break
    elif flag == 'robot_disconnected_flag':
        while True:
            time.sleep(0.05)
            if robot_disconnected_flag == 1:
                robot_disconnected_flag = 0
                break
    elif flag == 'scan_robot_successed':
        while True:
            time.sleep(0.05)
            if hulk.bServerOpen and len(hulk.g_conn_pool) > 0:
                break
    elif flag == 'robot_online_finished_flag':
        while True:
            time.sleep(0.05)
            if robot_online_finished_flag == 1:
                robot_online_finished_flag = 0
                break


def robot_init():
    print("--------------------------")
    eventManager.AddEventListener(EVENT_CONNECTED, new_connection)
    eventManager.AddEventListener(EVENT_DISCONNECTED, disconnection)
    eventManager.AddEventListener(EVENT_ROBOT, get_robot_data)
    eventManager.AddEventListener(EVENT_ROBOT, get_robot_finished)
    eventManager.AddEventListener(EVENT_ROBOT, get_robot_set_online_finished)
    eventManager.AddEventListener(EVENT_ROBOT, checkif_robot_error)
    eventManager.Start()
    print("--------------------------")
    hulk.set_encryption(True)
    if hulk.open_server():
        print(hulk.__server_ip__)
    else:
        eventManager.Stop()
        exit()
    hulk.scan()
    print("--------------------------")
    wait('scan_robot_successed')
    hulk.online(1)
    wait('robot_online_finished_flag')


def server_close():
    hulk.close_server()
    wait('robot_disconnected_flag')
    eventManager.Stop()
    print("--------------------------")


if __name__ == '__main__':
    robot_init()
    i = 2
    while i > 0:
        print(i)
        x, y, z, r, speed, acc = [400, 0, 100, 0, 10, 10]
        hulk.move(MoveType.MOVL, x, y, z, r, speed, acc)
        wait('robot_move_finished_flag')
        x, y, z, r, speed, acc = [400, 0, 200, 0, 10, 10]
        hulk.move(MoveType.MOVL, x, y, z, r, speed, acc)
        wait('robot_move_finished_flag')
        x, y, z, r, speed, acc = [200, 0, 100, 0, 10, 10]
        hulk.move(MoveType.MOVL, x, y, z, r, speed, acc)
        wait('robot_move_finished_flag')
        i -= 1
    server_close()
