from camera_inf import *
import time
import os
import numpy as np
from datetime import datetime
from ctypes import *
from PIL import Image

class CameraStar:
    def __init__(self):
        """初始化相机参数"""
        self.handle = 0
        self.isConnect = False
        self.iplist = []
        self.sframe = []
        
        # 图像相关参数
        self.imgsize = [HEIGHT, WIDTH]
        self.gray = (c_uint16 * WIDTH * HEIGHT)()
        self.buf = (c_uint8 * 1024)()
        self.rgb = (c_uint8 * 3 * WIDTH * HEIGHT)()
        
        # IP地址数组初始化
        self.ipaddr = []
        for i in range(32):
            ip_addr = T_IPADDR()
            self.ipaddr.append(ip_addr)
        self.ip_addr_array = (T_IPADDR * 32)(*self.ipaddr)
        
        # 创建保存目录
        self.grabdir = './grab'
        if not os.path.exists(self.grabdir):
            os.makedirs(self.grabdir)
            
        # 初始化SDK
        sdk_init()
        sdk_setIPAddrArray(self.ip_addr_array)
        
        # 初始化帧处理
        self.glbFrame = Frame()
        self.sizeFrame = 32 + WIDTH * HEIGHT * 2
        
        # 设置回调函数
        VIDEOCALLBACKFUNC = CFUNCTYPE(c_int, c_void_p, c_void_p)
        self.callback = VIDEOCALLBACKFUNC(self.frame_callback)

    def frame_callback(self, frame, this):
        """帧数据回调处理"""
        bytebuf = string_at(frame, self.sizeFrame)
        memmove(addressof(self.glbFrame), bytebuf, self.sizeFrame)
        self.sframe = self.glbFrame
        return 0
        
    def connect_camera(self, ip_address, port=None):
        """连接相机"""
        if port is None:
            str_iplist = ip_address.split('.')
            port = 30005 + 100 * int(str_iplist[3])
            
        ip = T_IPADDR()
        str_ip_as_bytes = str.encode(ip_address)
        
        for i in range(len(str_ip_as_bytes)):
            ip.IPAddr[i] = str_ip_as_bytes[i]
        ip.DataPort = port
        ip.isValid = 1
        
        sdk_creat_connect(self.handle, ip, self.callback, None)
        time.sleep(1)
        
        self.isConnect = sdk_isconnect(self.handle)
        return self.isConnect

    def set_temp_segment(self, index):
        """设置温度段"""
        if self.isConnect:
            sdk_tempseg_sel(self.handle, index)
            
    def calibration(self):
        """快门补偿"""
        if self.isConnect:
            sdk_calibration(self.handle)

    def grab_image(self):
        """抓取图像并保存灰度图和RGB图"""
        if not self.isConnect:
            print("相机未连接")
            return False
            
        curtime = datetime.now().strftime('%Y-%m-%d %H.%M.%S.%f')[:-3]
        gray_path = f'{self.grabdir}/gray_{self.handle}_{curtime}.jpg'
        rgb_path = f'{self.grabdir}/rgb_{self.handle}_{curtime}.jpg'
        
        if hasattr(self, 'sframe') and self.sframe:
            # 转换为灰度图
            sdk_frame2gray(byref(self.sframe), byref(self.gray))
            
            # 保存灰度图
            gray_array = np.frombuffer(self.gray, dtype=np.uint16)
            gray_array = gray_array.reshape((HEIGHT, WIDTH))
            gray_array = ((gray_array - gray_array.min()) * (255.0 / (gray_array.max() - gray_array.min()))).astype(np.uint8)
            gray_img = Image.fromarray(gray_array)
            gray_img.save(gray_path)
            print(f"灰度图已保存至: {gray_path}")
            
            # 转换为RGB并保存
            sdk_gray2rgb(byref(self.gray), byref(self.rgb), self.imgsize[1], self.imgsize[0], 0, 1)
            rgb_pathbytes = str.encode(rgb_path)
            sdk_saveframe2jpg(rgb_pathbytes, self.sframe, self.rgb)
            print(f"RGB图像已保存至: {rgb_path}")
            
            return True
        return False
        
    def close(self):
        """关闭相机连接"""
        if self.isConnect:
            sdk_stop(self.handle)
        sdk_quit()

def main():
    camera = CameraStar()
    
    ip = "192.168.1.11"  # 替换为实际的相机IP
    if camera.connect_camera(ip):
        print("相机连接成功")
        
        # 设置温度段(0:常温段, 1:中温段, 2:高温段)
        camera.set_temp_segment(0)
        
        # 执行快门补偿
        camera.calibration()
        
        # 抓取图像
        camera.grab_image()
        
        # 关闭相机
        camera.close()
    else:
        print("相机连接失败")

if __name__ == "__main__":
    main()