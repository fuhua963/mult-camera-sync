import time
import cv2
import numpy as np
import os
from config import *

class ThermalCamera:
    """红外相机控制类"""

    def __init__(self):
        """初始化红外相机"""
        self.cap = None
        self.is_connected = False
        self.frame_buffer = []
        self.save_path = None

    def connect(self):
        """连接红外相机"""
        self.cap = cv2.VideoCapture(THERMAL_CAMERA_IP)
        if self.cap.isOpened():
            self.is_connected = True
            print("红外相机连接成功")
        else:
            print("红外相机连接失败")
        return self.is_connected

    def configure(self):
        """配置红外相机参数"""
        if not self.is_connected:
            print("相机未连接")
            return False

        # 设置相机参数
        self.cap.set(cv2.CAP_PROP_FRAME_WIDTH, THERMAL_WIDTH)
        self.cap.set(cv2.CAP_PROP_FRAME_HEIGHT, THERMAL_HEIGHT)
        self.cap.set(cv2.CAP_PROP_FPS, THERMAL_FPS)
        print("红外相机参数配置完成")
        return True

    def start_acquisition(self, num_frames):
        """开始采集图像
        Args:
            num_frames (int): 采集的帧数
        """
        if not self.is_connected:
            print("相机未连接")
            return False

        self.frame_buffer.clear()
        for _ in range(num_frames):
            ret, frame = self.cap.read()
            if ret:
                self.frame_buffer.append(frame)
            else:
                print("图像采集失败")
                return False
        print(f"已采集 {len(self.frame_buffer)} 张图像")
        return True

    def save_images(self, base_path):
        """保存采集的图像
        Args:
            base_path (str): 保存路径
        """
        if not self.frame_buffer:
            print("没有可保存的图像")
            return

        timestamp = time.strftime("%Y_%m_%d_%H_%M_%S", time.localtime())
        self.save_path = os.path.join(base_path, timestamp, 'thermal')
        os.makedirs(self.save_path, exist_ok=True)

        for i, frame in enumerate(self.frame_buffer):
            file_path = os.path.join(self.save_path, f"thermal_{i:04d}.png")
            cv2.imwrite(file_path, frame)
        print(f"图像已保存至: {self.save_path}")

    def cleanup(self):
        """清理相机资源"""
        if self.cap:
            self.cap.release()
        print("红外相机资源已清理")
