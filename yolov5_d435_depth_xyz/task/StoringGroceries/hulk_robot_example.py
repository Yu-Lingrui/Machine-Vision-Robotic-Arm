import Hulk as hk
import time

hk.robot_init()
## 初始化机器人

# hk.set_io(16)
# ## 16 = 10000，用于供电
# time.sleep(1)
# hk.get_IO_input()
# hk.get_IO_output()

# hk.set_io(20)
# ## 20 = 10100，正压
# time.sleep(5)
# hk.get_IO_output()

# hk.set_io(24)
# ## 24 = 11000，负压
# time.sleep(5)
# hk.get_IO_output()

# hk.set_io(20)
# ## 20 = 10100，正压
# time.sleep(5)
# hk.get_IO_output()

# hk.set_io(24)
# ## 24 = 11000，负压
# time.sleep(5)
# hk.get_IO_output()

# hk.set_io(20)
# ## 20 = 10100，正压
# time.sleep(5)
# hk.get_IO_output()

# hk.set_io(24)
# ## 24 = 11000，负压
# time.sleep(5)
# hk.get_IO_output()

# x, y, z, r, speed, acc = [600, 0, 100, -142, 3, 3]
# x = 600 超限
# hk.move(hk.MoveType.MOVL, x, y, z, r, speed, acc)
# # MoveType.MOVL移动x,y,z；MoveType.MOVJ移动1，2，3，4轴
# # 超限后会回到home
# hk.wait('robot_move_finished_flag')
for i in range(2):
    x, y, z, r, speed, acc = [400, 0, 200, -142, 3, 3]
    # hk.move(hk.MoveType.MOVL, x, y, z, r, speed, acc)
    hk.move(0,199,231,121,-42,8,3)
    print('发送')
    hk.wait('robot_move_finished_flag')
    print('完成啦')

    x, y, z, r, speed, acc = [200, 0, 100, -142, 3, 3]
    # hk.move(hk.MoveType.MOVL, x, y, z, r, speed, acc)
    print('回家啦')
    hk.go_home()
    hk.wait('robot_move_finished_flag')
    print('到家啦')
# hk.set_io(0)
# time.sleep(5)
# hk.get_IO_input()


hk.server_close()
## 关闭服务器
