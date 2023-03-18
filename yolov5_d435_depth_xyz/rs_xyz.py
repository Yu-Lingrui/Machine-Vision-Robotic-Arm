import pyrealsense2 as rs
import numpy as np
import cv2

pc = rs.pointcloud()
points = rs.points()

pipeline = rs.pipeline()  # 创建一个管道
config = rs.config()  # Create a config并配置要流​​式传输的管道。
config.enable_stream(rs.stream.depth, 640, 480, rs.format.z16, 15)
config.enable_stream(rs.stream.color, 640, 480, rs.format.bgr8, 15)
# 使用选定的流参数显式启用设备流

# Start streaming 开启流
pipe_profile = pipeline.start(config)

align_to = rs.stream.color
align = rs.align(align_to)  # 设置为其他类型的流,意思是我们允许深度流与其他流对齐
print(type(align))
cap = cv2.VideoCapture(0)

object_x = 300  # 修改成检测目标的中心点即可(视频中心：640*480)
object_y = 250

while True:

    frames = pipeline.wait_for_frames()  # 等待开启通道
    # ret, frame = cap.read()  # ret 读取到图片为True 未读到图片为Falst
    # frame = cv2.flip(frame, 1)
    aligned_frames = align.process(frames)  # 将深度框和颜色框对齐
    depth_frame = aligned_frames.get_depth_frame()  # 获得对齐后的帧数深度数据(图)
    color_frame = aligned_frames.get_color_frame()  # 获得对齐后的帧数颜色数据(图)
    img_color = np.asanyarray(color_frame.get_data())  # 把颜色图像像素转化为数组
    img_depth = np.asanyarray(depth_frame.get_data())  # 把深度图像像素转化为数组
    # img_color2 = cv2.cvtColor(img_color, cv2.COLOR_BGR2GRAY)
    # Intrinsics & Extrinsics
    depth_intrin = depth_frame.profile.as_video_stream_profile().intrinsics # 获取内参
    color_intrin = color_frame.profile.as_video_stream_profile().intrinsics
    depth_to_color_extrin = depth_frame.profile.get_extrinsics_to(color_frame.profile)

    # 获取深度传感器的深度标尺
    depth_sensor = pipe_profile.get_device().first_depth_sensor()
    depth_scale = depth_sensor.get_depth_scale()

    # 由深度到颜色
    depth_pixel = [240, 320]  # Random pixel
    depth_point = rs.rs2_deproject_pixel_to_point(depth_intrin, depth_pixel, depth_scale)

    color_point = rs.rs2_transform_point_to_point(depth_to_color_extrin, depth_point)
    color_pixel = rs.rs2_project_point_to_pixel(color_intrin, color_point)
    print('depth: ', color_point)  #3d中的点坐标:xyz
    print('depth: ', color_pixel)  #坐标转像素验证

    pc.map_to(color_frame)
    points = pc.calculate(depth_frame)
    vtx = np.asanyarray(points.get_vertices())  # points.get_vertices() 检索点云的顶点
    tex = np.asanyarray(points.get_texture_coordinates())
    i = 640 * 200 + 200
    print('xyz: ', [np.float(vtx[i][0]), np.float(vtx[i][1]), np.float(vtx[i][2])])
    print("Distance/mm:", img_depth[object_x, object_y])
    cv2.circle(img_color, (object_x, object_y), 8, [255, 0, 255], thickness=-1)

    cv2.putText(img_color, "Distance/mm:" + str(img_depth[object_x, object_y]), (40, 40), cv2.FONT_HERSHEY_SIMPLEX, 1.2,
                [255, 0, 255])
    cv2.putText(img_color, "X:" + str(np.float(vtx[i][0])), (80, 80), cv2.FONT_HERSHEY_SIMPLEX, 1, [255, 0, 255])
    cv2.putText(img_color, "Y:" + str(np.float(vtx[i][1])), (80, 120), cv2.FONT_HERSHEY_SIMPLEX, 1, [255, 0, 255])
    cv2.putText(img_color, "Z:" + str(np.float(vtx[i][2])), (80, 160), cv2.FONT_HERSHEY_SIMPLEX, 1, [255, 0, 255])
    cv2.imshow('rgb_frame', img_color)
    cv2.imshow("depth_frame", img_depth)

    key = cv2.waitKey(1)
    if key == 27:
        cv2.destroyAllWindows()
        break

cv2.waitKey(0)
cv2.destroyAllWindows()
pipeline.stop()