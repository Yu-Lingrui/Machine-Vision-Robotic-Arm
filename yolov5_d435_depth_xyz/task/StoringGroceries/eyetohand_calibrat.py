import cv2
import numpy as np
from pyexpat.model import XML_CTYPE_CHOICE
from HulkRobot import *
from ObjectDectect_sift import *
from cam_sdk.camera import Camera

def on_EVENT_LBUTTONDOWN(event, x, y, flags, param):# 点击获取相应像素点位置
    if event == cv2.EVENT_LBUTTONDOWN:
        xy = "%d,%d" % (x, y)
        cv2.circle(img, (x, y), 3, (255, 0, 0), thickness = 3)
        cv2.putText(img, xy, (x, y), cv2.FONT_HERSHEY_PLAIN,
                    5.0, (0,0,0), thickness = 5)
        cv2.imshow("image_",img)
    return 

def calibrat(img):
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    blur = cv2.GaussianBlur(gray, (9, 9), 0)
    ret, thresh = cv2.threshold(blur, 120, 255, cv2.THRESH_BINARY)
    flags = cv2.CALIB_CB_NORMALIZE_IMAGE | cv2.CALIB_CB_EXHAUSTIVE | cv2.CALIB_CB_ACCURACY
    retval, corners = cv2.findChessboardCornersSB(thresh, (3, 3), flags=flags)# 测取摄像机坐标系下棋盘格内角点位置
    drawn = cv2.drawChessboardCorners(img, (3, 3), corners, retval)
    try:
        if corners == None:
            print('未成功获取内角点,请调试棋盘格')#查看是否成功获取内角点
    except:
        print('成功获取内角点')
    cv2.namedWindow("img", 0)
    cv2.resizeWindow("img", 640, 480)
    cv2.imshow('img',drawn)
    cv2.waitKey(0)

    img_coordinate = np.array(corners ,dtype=np.float32)

    #输入角点和边点的机器人xy坐标(需要设置r坐标为-102)
    x1, y1 = input('请输入角点x，y坐标，用空格隔开').split()
    x2, y2 = input('请输入边点x，y坐标，用空格隔开').split()

    x1, y1, x2, y2 = float(x1), float(y1), float(x2), float(y2)

    obj_coor = obj_coordinate(x1, y1, x2, y2)


    mat_,inlier = cv2.estimateAffine2D(img_coordinate,  obj_coor)#实现标定
    np.savetxt("mat_.txt", mat_)#记录mat_
        

    return mat_


def obj_coordinate(x1, y1, x2, y2): #通过输入坐标获取九点位置机器人坐标
    ori_x = x2 - x1  
    ori_y = y2 - y1
    obj_coordinate = []
    obj_coordinate = np.array([
        [x1, y1],
        [x1 + ori_x, y1 + ori_y],
        [x1 + 2*ori_x, y1 + 2*ori_y],
        [x1 + ori_y, y1 - ori_x],
        [x1 + ori_y + ori_x, y1 - ori_x + ori_y],
        [x1 + ori_y + 2*ori_x, y1 - ori_x + 2*ori_y],
        [x1 + 2*ori_y, y1 - 2*ori_x],
        [x1 + 2*ori_y + ori_x, y1 - 2*ori_x + ori_y],
        [x1 + 2*ori_y + 2*ori_x, y1 - 2*ori_x + 2*ori_y]
        ],dtype=np.float32)

    return obj_coordinate


if __name__ == '__main__':
    
    cam = Camera()
    cam.open(0)  # 打开摄像头
    im = cam.read()
    #cv2.imshow('im', im)
    cv2.imwrite("./img/img.jpg", im)
    cam.close()  # 关闭摄像头
    img = cv2.imread('./img/img.jpg')
    mat_ = calibrat(img)
    #用另外点检验标定是否准确 
    # print(mat_)

    cv2.namedWindow("image_", cv2.WINDOW_FREERATIO)
    cv2.setMouseCallback("image_",on_EVENT_LBUTTONDOWN) # 点击获取相应像素点位置
    cv2.imshow("image_",img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()


    mat_=mat_.astype(np.float)
    np.savetxt("mat_.txt", mat_,fmt='%f',delimiter=',')
    check_x, check_y = input('请输入检验点像素坐标x,y，用空格隔开').split()
    check_x, check_y = float(check_x), float(check_y)
    print('t_rx= ',(mat_[0,0] * check_x) + mat_[0,1] * check_y + mat_[0,2])
    print('t_ry= ',(mat_[1,0] * check_x) + mat_[1,1] * check_y + mat_[1,2])