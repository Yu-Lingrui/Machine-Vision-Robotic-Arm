import Hulk.hulk_robot_init

robot_init = Hulk.hulk_robot_init.robot_init
# 初始化

server_close = Hulk.hulk_robot_init.server_close
# 关闭服务器

move = Hulk.hulk_robot_init.hulk.move
# 移动 x, y, z, r, speed, acc

wait = Hulk.hulk_robot_init.wait
# 等待下位机返回相应函数, 'robot_move_finished_flag'(移动结束), 阻塞

MoveType = Hulk.hulk_robot_init.MoveType
# 移动方式MOVL，直线运动

set_io = Hulk.hulk_robot_init.hulk.set_io
# 设置IO，5位，int('0b00000',2)，0低电平，1高电平，最高位对应最左侧输出

get_IO_input = Hulk.hulk_robot_init.get_IO_input
# 获取IO_input，int('0b00000',2)

get_IO_output = Hulk.hulk_robot_init.get_IO_output
# 获取IO_output，int('0b00000',2)

go_home = Hulk.hulk_robot_init.robot_go_home
# 回到HOME

get_end_xyz = Hulk.hulk_robot_init.get_robot_xyz
# 获取末端x，y，z
# rx, ry, rz = hk.get_end_xyz()
# distance_camera_to_end = 100
# hk.move(0, z + rx, -x + ry, -y + rz + distance_camera_to_end, 0, 10, 3)