# coding=UTF-8

import _init_path
from yolov5 import *
import pyrealsense2 as rs
import numpy as np
import cv2

if __name__ == '__main__':
    # 读取camera device
    video_path = '0'
    vid = cv2.VideoCapture(0)
    i = 0
    # 初始化yolo模型
    detector = YoloDetector()

    # realsense init
    # Create a pipeline
    pipeline = rs.pipeline()

    # Create a config and configure the pipeline to stream
    #  different resolutions of color and depth streams
    config = rs.config()

    # Get device product line for setting a supporting resolution
    pipeline_wrapper = rs.pipeline_wrapper(pipeline)
    pipeline_profile = config.resolve(pipeline_wrapper)
    device = pipeline_profile.get_device()
    device_product_line = str(device.get_info(rs.camera_info.product_line))

    config.enable_stream(rs.stream.depth, 640, 480, rs.format.z16, 30)  # 使能深度相机

    # 使能rgb相机
    if device_product_line == 'L500':
        config.enable_stream(rs.stream.color, 960, 540, rs.format.bgr8, 30)
    else:
        config.enable_stream(rs.stream.color, 640, 480, rs.format.bgr8, 30)

    # Start streaming
    profile = pipeline.start(config)

    # Getting the depth sensor's depth scale (see rs-align example for explanation)
    depth_sensor = profile.get_device().first_depth_sensor()
    depth_scale = depth_sensor.get_depth_scale()
    print("Depth Scale is: " , depth_scale)

    # 设置省略掉的深度区域范围
    # We will be removing the background of objects more than clipping_distance_in_meters meters away
    clipping_distance_in_meters = 1 #1 meter（1米之外省略）
    clipping_distance = clipping_distance_in_meters / depth_scale

    # Create an align object
    # rs.align allows us to perform alignment of depth frames to others frames
    # The "align_to" is the stream type to which we plan to align depth frames.
    align_to = rs.stream.color
    align = rs.align(align_to)

    while True:
        # Wait for a coherent pair of frames: depth and color
        # Get frameset of color and depth
        frames = pipeline.wait_for_frames()
        # frames.get_depth_frame() is a 640x360 depth image

        # Align the depth frame to color frame
        aligned_frames = align.process(frames)

        # Get aligned frames 这里得到的rgb图和深度图已经完成匹配了
        aligned_depth_frame = aligned_frames.get_depth_frame() # aligned_depth_frame is a 640x480 depth image
        color_frame = aligned_frames.get_color_frame()

        # Validate that both frames are valid
        if not aligned_depth_frame or not color_frame:
            continue

        # 获取内参
        # depth_profile = aligned_depth_frame.get_profile()
        color_profile = color_frame.get_profile()
        cvsprofile = rs.video_stream_profile(color_profile)
        # dvsprofile = rs.video_stream_profile(depth_profile)

        color_intrin = cvsprofile.get_intrinsics()
        ppx = color_intrin.ppx
        ppy = color_intrin.ppy
        fx = color_intrin.fx
        fy = color_intrin.fy

        # Convert images to numpy arrays
        # 转换为numpy数据格式
        depth_image = np.asanyarray(aligned_depth_frame.get_data())
        img = np.asanyarray(color_frame.get_data())

        i += 1
        t0 = time.time()

        # 检测
        detections = detector.detect(img) # 每帧检测结果 返回一个YoloResult类
        for det in detections:
            # if det.label == 'person':
            #    print("person")
            # det.draw(img)
            if det.label == 'person':
                dist = aligned_depth_frame.get_distance(det.center[0], det.center[1])
                z = int(dist * 100)
                if dist > 0:
                    det.draw(img)
                    x_true = (det.center[0] - ppx) * dist / fx
                    y_true = (det.center[1] - ppy) * dist / fy
                    x = int(x_true * 100)
                    y = int(y_true * 100)
                    print(det.label, x, y, z)
                    cv2.putText(img, str([x, y, z]), (det.center[0], det.center[1]), cv2.FONT_HERSHEY_SIMPLEX, 0.5, [225, 255, 255], 2)
            
        cv2.imshow("p", img)
        if cv2.waitKey(1) == ord('q'):  # q to quit
            cv2.destroyAllWindows()
            raise StopIteration

        # print('Done. (%.3fs)' % (time.time() - t0))
        
    cv2.destroyAllWindows()  
