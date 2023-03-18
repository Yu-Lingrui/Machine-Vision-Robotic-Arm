#!/usr/bin/env python
# coding=UTF-8

import argparse

import torch.backends.cudnn as cudnn

import _init_path

from src.yolov5.utils.datasets import *
from src.yolov5.utils.utils import *

from setting.settings import getconfig
sys.path.append("/home/ylr/pepper2022/src/yolov5/")


# 对检测器进行了封装
import _init_path
import argparse
import sys,os
import torch
import numpy
import torch.backends.cudnn as cudnn
from src.yolov5.utils.datasets import *
from src.yolov5.utils.utils import *

gpsr_names = ["potato chips","sprite","coke","grape juice","apple","orange","orange juice","chocolate drink","sausages","pringles","crackers","noodles","milk"]

class GPSRDetector():
    def __init__(self,weights='/home/tjark/pepper2022/src/yolov5/weights/best_s.pt',gpsr=True):
        self.gpsr = gpsr
        ### 定义权重文件路径，都是相对路径
        if not os.path.exists(weights):
            print("There is no yolo weights in{}".format(weights))
            return 0
        else:
            # 默认权重文件采用yolov5s，使用视频作为输入
            self.imgsz = 640 # inference size (pixels)
            self.conf_thres = 0.1 # object confidence threshold
            self.iou_thres = 0.2 # IOU threshold for NMS
            self.device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
            # 单张图片检测
            with torch.no_grad():
                # Initialize
                self.half = self.device.type != 'cpu'  # half precision only supported on CUDA
                print(weights)
                self.model = torch.load(weights, map_location=self.device)['model'].float()  # load to FP32
                
                self.model.to(self.device).eval()
                if self.half:
                    self.model.half()  # to FP16
                # 载入物品名称
                self.names = self.model.module.names if hasattr(self.model, 'module') else self.model.names

    def detect(self,img):
        """ 视频流载入读取 """
        with torch.no_grad():
            # Run inference
            t0 = time.time()
            im0 = img.copy()
            # img = torch.zeros((1, 3, self.imgsz, self.imgsz), device=self.device)  # init img
            img = letterbox(img, new_shape=self.imgsz)[0]
            img = img[:, :, ::-1].transpose(2, 0, 1)  # Convert BGR to RGB, to 3x416x416
            # cv2.imshow('test',img)
            img = np.ascontiguousarray(img)
            img = torch.from_numpy(img).to(self.device)
            img = img.half() if self.half else img.float()  # uint8 to fp16/32
            img = img / 255.0  # 0 - 255 to 0.0 - 1.0
            # img = torch.from_numpy(img).to(self.device)
            # img /= 255.0  # 0 - 255 to 0.0 - 1.0
            if img.ndimension() == 3:
                img = img.unsqueeze(0)

            # Inference
            t1 = torch_utils.time_synchronized()
            pred = self.model(img, augment=False)[0]

            # Apply NMS
            pred = non_max_suppression(pred, self.conf_thres, self.iou_thres, classes=False, agnostic=False)
            t2 = torch_utils.time_synchronized()

            # Process detections
            current_detections = []
            for i, det in enumerate(pred):  # detections per image
                gn = torch.tensor(im0.shape)[[1, 0, 1, 0]]  #  normalization gain whwh
                if det is not None and len(det):
                    # Rescale boxes from img_size to im0 size
                    det[:, :4] = scale_coords(img.shape[2:], det[:, :4], im0.shape).round()

                    # Write results
                    for result_pack in reversed(det):
                        xyxy=result_pack[:4]
                        conf = result_pack[4]
                        cls = result_pack[5]
                        xywh = (xyxy2xywh(xyxy.clone().detach().view(1, 4))).view(-1).tolist()  # normalized xywh
                        # if self.gpsr:
                        label = gpsr_names[int(cls)]
                        # else:
                        #     label = self.names[int(cls)]
                        
                        detection = YoloResult(xyxy, xywh, label, conf)
                        current_detections.append(detection)
                            
        return current_detections

class YoloDetector():
    def __init__(self, weights='/home/ylr/pepper2022/src/yolov5/weights/yolov5s.pt'):
        # 默认权重文件采用yolov5s，使用视频作为输入
        self.imgsz = 640 # inference size (pixels)
        self.conf_thres = 0.1 # object confidence threshold
        self.iou_thres = 0.2 # IOU threshold for NMS
        self.device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
        # 单张图片检测
        with torch.no_grad():
            # Initialize
            self.half = self.device.type != 'cpu'  # half precision only supported on CUDA
            print(weights)
            self.model = torch.load(weights, map_location=self.device)['model'].float()  # load to FP32
            
            self.model.to(self.device).eval()
            if self.half:
                self.model.half()  # to FP16
            # 载入物品名称
            self.names = self.model.module.names if hasattr(self.model, 'module') else self.model.names
            
            # 初始化一个图片模板
            # self.img = torch.zeros((1, 3, self.imgsz, self.imgsz), device=self.device)  # init img
            # _ = self.model(self.img.half() if half else self.img) if self.device.type != 'cpu' else None  # run once
            
    def detect(self, img):
        """ 视频流载入读取 """
        with torch.no_grad():
            # Run inference
            t0 = time.time()
            try:
                im0 = img.copy()
            except Exception as e:
                return 
            # img = torch.zeros((1, 3, self.imgsz, self.imgsz), device=self.device)  # init img
            img = letterbox(img, new_shape=self.imgsz)[0]
            img = img[:, :, ::-1].transpose(2, 0, 1)  # Convert BGR to RGB, to 3x416x416
            # cv2.imshow('test',img)
            img = np.ascontiguousarray(img)
            img = torch.from_numpy(img).to(self.device)
            img = img.half() if self.half else img.float()  # uint8 to fp16/32
            img = img / 255.0  # 0 - 255 to 0.0 - 1.0
            # img = torch.from_numpy(img).to(self.device)
            # img /= 255.0  # 0 - 255 to 0.0 - 1.0
            if img.ndimension() == 3:
                img = img.unsqueeze(0)

            # Inference
            t1 = torch_utils.time_synchronized()
            pred = self.model(img, augment=False)[0]

            # Apply NMS
            pred = non_max_suppression(pred, self.conf_thres, self.iou_thres, classes=False, agnostic=False)
            t2 = torch_utils.time_synchronized()

            # Process detections
            current_detections = []
            for i, det in enumerate(pred):  # detections per image
                gn = torch.tensor(im0.shape)[[1, 0, 1, 0]]  #  normalization gain whwh
                if det is not None and len(det):
                    # Rescale boxes from img_size to im0 size
                    det[:, :4] = scale_coords(img.shape[2:], det[:, :4], im0.shape).round()

                    # Write results
                    for *xyxy, conf, cls in det:
                        _xyxy = (torch.tensor(xyxy).view(1, 4)).view(-1).tolist()
                        xywh = (xyxy2xywh(torch.tensor(xyxy).view(1, 4))).view(-1).tolist()  # normalized xywh
                        label = self.names[int(cls)]
                        detection = YoloResult(_xyxy, xywh, label, conf)
                        current_detections.append(detection)
                            
        return current_detections


class YoloResult():
    def __init__(self, xyxy, xywh, label, confidence):
        self.xyxy = xyxy  # xy1 top-left, xy2 bottom-right
        self.xywh = xywh  # x center, y center, width, height
        self.label = label
        self.confidence = confidence
        self.color = [97, 255, 0]
        self.center = [int(xywh[0]), int(xywh[1])]
        self.bbox = [int(xyxy[0]), int(xyxy[1]),int(xyxy[2]-xyxy[0]),int(xyxy[3]-xyxy[1])]
    
    def draw(self, img, color=[97, 255, 0]):
        plot_one_box(self.xyxy, img, label=self.label, color=self.color, line_thickness=2)

# detector = YoloDetector()
# detections = detector.detect(img)


if __name__ == '__main__':
    # 用电脑端测试检测效果
    video_path = '0'
    vid = cv2.VideoCapture(0)
    i = 0
    # 初始化yolo模型
    # detector = YoloDetector(weights='/home/ylr/AGV/pepper2022/src/yolov5/weights/yolov5s.pt')
    detector = GPSRDetector(weights='/home/ylr/projects/pepper2022src/yolov5/weights/best_s.pt')
    while True:
        i += 1
        t0 = time.time()

        return_val, img = vid.read()
        # # im0 = img.copy()
        # 检测
        detections = detector.detect(img) # 每帧检测结果 返回一个YoloResult类
        for det in detections:
            if det.label == 'person':
                print("person")
                det.draw(img)
        
        cv2.imshow("p", img)
        if cv2.waitKey(1) == ord('q'):  # q to quit
            cv2.destroyAllWindows()
            raise StopIteration

        print('Done. (%.3fs)' % (time.time() - t0))
        
    cv2.destroyAllWindows()   
