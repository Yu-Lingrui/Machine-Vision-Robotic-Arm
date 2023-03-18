# coding=UTF-8

import _init_path
from yolov5 import *
import Hulk as hk
import pyrealsense2 as rs
import numpy as np
import cv2


class SeenGrab():
    label_list = []
    new_x = 0
    new_y = 0
    new_z = 0
    count = 0
    list_x = []
    list_y = []
    list_z = []
    find_success_flag = 0

    # list_label = []

    # if __name__ == '__main__':
    def calxyz(self, label):
        if label == "drinks":
            self.label_list = ["grape juice", "orange juice"]
        elif label == 'food':
            self.label_list = ['noodles', 'sausages']
        elif label == 'fruits':
            self.label_list = ['apple', 'orange']
        elif label == 'snacks':
            self.label_list = ['pringles', 'crackers', 'potato chips']
        else:
            self.label_list = [label]
        x = 0
        y = 0
        z = 0
        self.count = 0
        self.find_success_flag = 0
        # 读取camera device
        video_path = '0'
        vid = cv2.VideoCapture(0)
        # 初始化yolo模型
        # detector = GPSRDetector()
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

        config.enable_stream(rs.stream.depth, 640, 480, rs.format.z16,
                             30)  # 使能深度相机

        # 使能rgb相机
        if device_product_line == 'L500':
            config.enable_stream(rs.stream.color, 960, 540, rs.format.bgr8, 30)
        else:
            config.enable_stream(rs.stream.color, 640, 480, rs.format.bgr8, 30)

        # Start streaming
        profile = pipeline.start(config)

        # Hulk robot init
        # hk.robot_init()

        # Getting the depth sensor's depth scale (see rs-align example for explanation)
        depth_sensor = profile.get_device().first_depth_sensor()
        depth_scale = depth_sensor.get_depth_scale()
        print("Depth Scale is: ", depth_scale)

        # 设置省略掉的深度区域范围
        # We will be removing the background of objects more than clipping_distance_in_meters meters away
        clipping_distance_in_meters = 1  # 1 meter（1米之外省略）
        clipping_distance = clipping_distance_in_meters / depth_scale

        # Create an align object
        # rs.align allows us to perform alignment of depth frames to others frames
        # The "align_to" is the stream type to which we plan to align depth frames.
        align_to = rs.stream.color
        align = rs.align(align_to)

        t0 = time.time()

        while True:
            # Wait for a coherent pair of frames: depth and color
            # Get frameset of color and depth
            frames = pipeline.wait_for_frames()
            # frames.get_depth_frame() is a 640x360 depth image

            # Align the depth frame to color frame
            aligned_frames = align.process(frames)

            # Get aligned frames 这里得到的rgb图和深度图已经完成匹配了
            aligned_depth_frame = aligned_frames.get_depth_frame(
            )  # aligned_depth_frame is a 640x480 depth image
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
            # 检测
            detections = detector.detect(img)  # 每帧检测结果 返回一个YoloResult类
            for det in detections:
                # if det.label == 'person':
                #    print("person")
                # det.draw(img)
                if det.label in self.label_list:
                    dist = aligned_depth_frame.get_distance(
                        det.center[0], det.center[1])
                    z = int(dist * 1000)
                    if dist > 0 and dist < 1.0:
                        det.draw(img)
                        x_true = (det.center[0] - ppx) * dist / fx
                        y_true = (det.center[1] - ppy) * dist / fy
                        x = int(x_true * 1000)
                        y = int(y_true * 1000)
                        rx, ry, rz = hk.get_end_xyz()
                        distance_camera_to_end = 100
                        self.new_x = z + int(rx)
                        self.new_y = -x + int(ry)
                        self.new_z = -y + int(rz) + 100
                        if self.new_x > 500:
                            self.new_x = 500
                        print('target: ', self.new_x, self.new_y, self.new_z)
                        self.list_x.append(self.new_x)
                        self.list_y.append(self.new_y)
                        self.list_z.append(self.new_z)
                        # self.list_label.append(det.label)
                        self.count = self.count + 1
                        print(det.label, x, y, z)
            #             cv2.putText(img, str([x, y,
            #                                   z]), (det.center[0], det.center[1]),
            #                         cv2.FONT_HERSHEY_SIMPLEX, 0.5, [225, 255, 255],
            #                         2)
            # cv2.imshow("p", img)
            # k = cv2.waitKey(1)
            # if k == ord('q'):  # q to quit
            #     cv2.destroyAllWindows()
            #     raise StopIteration
            # elif k == ord('s'):
                # rx, ry, rz = hk.get_end_xyz()
                # distance_camera_to_end = 100
                # new_x = z + int(rx)
                # new_y = -x + int(ry)
                # new_z = -y + int(rz) + 100
                # print(new_x,new_y,new_z)
                # hk.move(0, (z + rx), (-x + ry), (-y + rz + 100), 0, 10, 3)
                #
                # time.sleep(2)
                # hk.move(0, new_x, new_y, new_z, -142, 8, 2)
                # hk.wait('robot_move_finished_flag')
                # hk.set_io(20)
                # time.sleep(2)
            #     print("s")
            # elif k == ord('h'):
            #     hk.go_home()
            #     print('go home')
            # elif k==ord('g'):
            #     hk.set_io(20)
            # elif k==ord('l'):
            #     hk.set_io(24)
            # print('Done. (%.3fs)' % (time.time() - t0))
            if self.count >= 3:
                self.count = 0
                self.find_success_flag = 1
                cv2.destroyAllWindows()
                break
            elif abs(time.time() - t0) >= 3:
                self.find_success_flag = 0
                break

        # hk.server_close()
        pipeline.stop()
        return self.list_x, self.list_y, self.list_z, self.find_success_flag
        # cv2.destroyAllWindows()


if __name__ == "__main__":
    find_object_points_list = [[200, 0, 50], [200, -70, 50], [200, 70, 50],
                               [200, 0, 180], [200, -70, 180], [200, 70, 180]]

    hk.robot_init()
    find_object_count = 1
    while True:
        sng = SeenGrab()
        x, y, z, find_object_flag = sng.calxyz('orange')
        if not find_object_flag:
            if find_object_count == 6:
                find_object_count = 1
                new_x = 300
                new_y = 0
                new_z = 100
                hk.go_home()
                hk.wait('robot_move_finished_flag')
                print("Cannot find ")
                break
            hk.move(
                0,
                find_object_points_list[find_object_count][0],
                find_object_points_list[find_object_count][1],
                find_object_points_list[find_object_count][2],
                -142, 15, 10)
            hk.wait('robot_move_finished_flag')
            find_object_count += 1
        elif find_object_flag:
            new_x = np.mean(x)
            new_y = np.mean(y)
            new_z = np.mean(z)
            if new_x**2 + new_y**2 + new_z**2 > 525**2:
                print("超限...")
                new_x = 300
                new_y = 0
                new_z = 100
            print("found", new_x, new_y, new_z)
            break
    hk.set_io(24)
    hk.move(0, new_x, new_y, new_z, -142, 8, 8)
    hk.wait('robot_move_finished_flag')
    hk.set_io(20)
    time.sleep(3)
    nowz = 100
    nowx, nowy, nowz = hk.get_end_xyz()
    hk.move(0, 200, 0, nowz, -142, 15, 8)
    hk.wait('robot_move_finished_flag')
    hk.go_home()
    hk.wait('robot_move_finished_flag')
    hk.server_close()
    print('Take finished')
    cv2.destroyAllWindows()
