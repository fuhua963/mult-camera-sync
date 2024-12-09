import PySpin
import numpy as np
import cv2 as cv
import os
from config import *

class FlirCamera:
    """FLIR相机控制类"""
    
    def __init__(self):
        """初始化FLIR相机"""
        self.system = None
        self.cam_list = None

    def initialize(self):
        """初始化相机系统"""
        self.system = PySpin.System.GetInstance()
        version = self.system.GetLibraryVersion()
        print(f'FLIR SDK版本: {version.major}.{version.minor}.{version.type}.{version.build}')
        self.cam_list = self.system.GetCameras()
        num_cameras = self.cam_list.GetSize()
        print(f'检测到 {num_cameras} 个相机')
        
        if num_cameras == 0:
            self.cleanup()
            return False
        if self.system:
            return True
        else:
            return False

    def config_camera(self, nodemap):
        """配置相机参数"""
        try:
            # 配置像素格式
            self._set_pixel_format(nodemap)
            
            # 配置ROI
            self._set_roi(nodemap)
            
            # 配置曝光
            self._set_exposure(nodemap)
            
            # 配置增益
            self._set_gain(nodemap)
            
            # 配置白平衡
            self._set_white_balance(nodemap)
            
            # 配置吞吐量
            self._set_throughput(nodemap)
            
            # 配置触发模式
            self._set_trigger(nodemap)
            
            # 配置采集模式
            self._set_acquisition_mode(nodemap)
            
            # 配置帧率
            self._set_framerate(nodemap)
            
            # 配置数据块模式
            self._enable_chunk_data(nodemap)
            
            return True
            
        except PySpin.SpinnakerException as ex:
            print(f'相机配置错误: {ex}')
            return False

    def _set_pixel_format(self, nodemap):
        """设置像素格式为BayerRG8"""
        try:
            node_pixel_format = PySpin.CEnumerationPtr(nodemap.GetNode('PixelFormat'))
            if not PySpin.IsAvailable(node_pixel_format) or not PySpin.IsWritable(node_pixel_format):
                print('像素格式节点不可用')
                return

            # 设置为BayerRG8格式
            node_pixel_format_bayer = node_pixel_format.GetEntryByName('BayerRG8')
            if PySpin.IsReadable(node_pixel_format_bayer):
                pixel_format_bayer = node_pixel_format_bayer.GetValue()
                node_pixel_format.SetIntValue(pixel_format_bayer)
            else:
                print('BayerRG8格式不可用')
        except PySpin.SpinnakerException as ex:
            print(f'设置像素格式错误: {ex}')

    def _set_roi(self, nodemap):
        """设置ROI参数"""
        try:
            # 设置宽度
            node_width = PySpin.CIntegerPtr(nodemap.GetNode('Width'))
            if PySpin.IsAvailable(node_width) and PySpin.IsWritable(node_width):
                node_width.SetValue(FLIR_WIDTH)

            # 设置高度
            node_height = PySpin.CIntegerPtr(nodemap.GetNode('Height'))
            if PySpin.IsAvailable(node_height) and PySpin.IsWritable(node_height):
                node_height.SetValue(FLIR_HEIGHT)

            # 设置X偏移
            node_offset_x = PySpin.CIntegerPtr(nodemap.GetNode('OffsetX'))
            if PySpin.IsAvailable(node_offset_x) and PySpin.IsWritable(node_offset_x):
                node_offset_x.SetValue(FLIR_OFFSET_X)

            # 设置Y偏移
            node_offset_y = PySpin.CIntegerPtr(nodemap.GetNode('OffsetY'))
            if PySpin.IsAvailable(node_offset_y) and PySpin.IsWritable(node_offset_y):
                node_offset_y.SetValue(FLIR_OFFSET_Y)

        except PySpin.SpinnakerException as ex:
            print(f'设置ROI错误: {ex}')

    def _set_exposure(self, nodemap):
        """设置曝光参数"""
        try:
            if FLIR_AUTO_EXPOSURE:
                self._set_auto_exposure(nodemap)
            else:
                self._set_manual_exposure(nodemap)
        except PySpin.SpinnakerException as ex:
            print(f'设置曝光参数错误: {ex}')

    def _set_auto_exposure(self, nodemap):
        """设置自动曝光"""
        try:
            # 设置自动曝光上限
            node_exposure_time_upper_limit = PySpin.CFloatPtr(nodemap.GetNode('AutoExposureExposureTimeUpperLimit'))
            if PySpin.IsAvailable(node_exposure_time_upper_limit) and PySpin.IsWritable(node_exposure_time_upper_limit):
                node_exposure_time_upper_limit.SetValue(5000000)

            # 启用自动曝光
            node_exposure_auto = PySpin.CEnumerationPtr(nodemap.GetNode('ExposureAuto'))
            if not PySpin.IsAvailable(node_exposure_auto) or not PySpin.IsWritable(node_exposure_auto):
                return

            entry_exposure_auto_continuous = node_exposure_auto.GetEntryByName('Continuous')
            if PySpin.IsReadable(entry_exposure_auto_continuous):
                exposure_auto_continuous = entry_exposure_auto_continuous.GetValue()
                node_exposure_auto.SetIntValue(exposure_auto_continuous)
            else:
                print('自动曝光模式不可用')

        except PySpin.SpinnakerException as ex:
            print(f'设置自动曝光错误: {ex}')

    def _set_manual_exposure(self, nodemap):
        """设置手动曝光"""
        try:
            # 关闭自动曝光
            node_exposure_auto = PySpin.CEnumerationPtr(nodemap.GetNode('ExposureAuto'))
            if PySpin.IsAvailable(node_exposure_auto) and PySpin.IsWritable(node_exposure_auto):
                entry_exposure_auto_off = node_exposure_auto.GetEntryByName('Off')
                if PySpin.IsReadable(entry_exposure_auto_off):
                    exposure_auto_off = entry_exposure_auto_off.GetValue()
                    node_exposure_auto.SetIntValue(exposure_auto_off)

            # 设置曝光模式为Timed
            node_exposure_mode = PySpin.CEnumerationPtr(nodemap.GetNode('ExposureMode'))
            if PySpin.IsAvailable(node_exposure_mode) and PySpin.IsWritable(node_exposure_mode):
                entry_exposure_mode_timed = node_exposure_mode.GetEntryByName('Timed')
                if PySpin.IsReadable(entry_exposure_mode_timed):
                    exposure_mode_timed = entry_exposure_mode_timed.GetValue()
                    node_exposure_mode.SetIntValue(exposure_mode_timed)

            # 设置曝光时间
            node_exposure_time = PySpin.CFloatPtr(nodemap.GetNode('ExposureTime'))
            if PySpin.IsAvailable(node_exposure_time) and PySpin.IsWritable(node_exposure_time):
                node_exposure_time.SetValue(FLIR_EXPOSURE_TIME)
        except PySpin.SpinnakerException as ex:
            print(f'设置手动曝光错误: {ex}')

    def _set_gain(self, nodemap):
        """设置增益"""
        try:
            # 关闭自动增益
            node_gain_auto = PySpin.CEnumerationPtr(nodemap.GetNode('GainAuto'))
            if PySpin.IsAvailable(node_gain_auto) and PySpin.IsWritable(node_gain_auto):
                entry_gain_auto_off = node_gain_auto.GetEntryByName('Off')
                if PySpin.IsReadable(entry_gain_auto_off):
                    gain_auto_off = entry_gain_auto_off.GetValue()
                    node_gain_auto.SetIntValue(gain_auto_off)

        except PySpin.SpinnakerException as ex:
            print(f'设置增益错误: {ex}')

    def _set_white_balance(self, nodemap):
        """设置白平衡"""
        try:
            # 关闭自动白平衡
            node_balance_white_auto = PySpin.CEnumerationPtr(nodemap.GetNode('BalanceWhiteAuto'))
            if PySpin.IsAvailable(node_balance_white_auto) and PySpin.IsWritable(node_balance_white_auto):
                entry_balance_white_auto_off = node_balance_white_auto.GetEntryByName('Off')
                if PySpin.IsReadable(entry_balance_white_auto_off):
                    balance_white_auto_off = entry_balance_white_auto_off.GetValue()
                    node_balance_white_auto.SetIntValue(balance_white_auto_off)
            # 设置白平衡值
            # node_balance_ratio = PySpin.CFloatPtr(nodemap.GetNode('BalanceRatio'))
            # if PySpin.IsAvailable(node_balance_ratio) and PySpin.IsWritable(node_balance_ratio):
            #     node_balance_ratio.SetValue(FLIR_BALANCE_WHITE)
            #     print(f'白平衡值已设置为: {FLIR_BALANCE_WHITE}')

        except PySpin.SpinnakerException as ex:
            print(f'设置白平衡错误: {ex}')

    def _set_throughput(self, nodemap):
        """设置吞吐量"""
        try:
            node_device_link_throughput_limit = PySpin.CIntegerPtr(nodemap.GetNode('DeviceLinkThroughputLimit'))
            if PySpin.IsAvailable(node_device_link_throughput_limit) and PySpin.IsWritable(node_device_link_throughput_limit):
                node_device_link_throughput_limit.SetValue(43000000)

        except PySpin.SpinnakerException as ex:
            print(f'设置吞吐量错误: {ex}')

    def _set_trigger(self, nodemap):
        """设置触发模式"""
        try:
            # 设置触发模式
            node_trigger_mode = PySpin.CEnumerationPtr(nodemap.GetNode('TriggerMode'))
            if PySpin.IsAvailable(node_trigger_mode) and PySpin.IsWritable(node_trigger_mode):
                node_trigger_mode.SetIntValue(PySpin.TriggerMode_On if FLIR_EX_TRIGGER else PySpin.TriggerMode_Off)

            if FLIR_EX_TRIGGER:
                # 设置触发源
                node_trigger_source = PySpin.CEnumerationPtr(nodemap.GetNode('TriggerSource'))
                entry_trigger_source_line3 = node_trigger_source.GetEntryByName('Line3')
                if not PySpin.IsAvailable(entry_trigger_source_line3) or not PySpin.IsReadable(entry_trigger_source_line3):
                    print('\nUnable to enter Trigger Source Line3. Aborting...\n')
                    return False
                trigger_source_line3 = entry_trigger_source_line3.GetValue()
                node_trigger_source.SetIntValue(trigger_source_line3)
                
                node_trigger_activation = PySpin.CEnumerationPtr(nodemap.GetNode('TriggerActivation'))
                if not PySpin.IsAvailable(node_trigger_activation) or not PySpin.IsWritable(node_trigger_activation):
                    print('\nUnable to set Trigger Activation (enumeration retrieval). Aborting...\n')
                    return False
                entry_trigger_activation_risingedge = node_trigger_activation.GetEntryByName('RisingEdge')
                if not PySpin.IsAvailable(entry_trigger_activation_risingedge) or not PySpin.IsReadable(entry_trigger_activation_risingedge):
                    print('\nUnable to enter Trigger Activation Rising Edge. Aborting...\n')
                    return False
                trigger_activation_risingedge = entry_trigger_activation_risingedge.GetValue()
                node_trigger_activation.SetIntValue(trigger_activation_risingedge)

                node_trigger_overlap = PySpin.CEnumerationPtr(nodemap.GetNode('TriggerOverlap'))
                if not PySpin.IsAvailable(node_trigger_overlap) or not PySpin.IsWritable(node_trigger_overlap):
                    print('\nUnable to set Trigger Overlap (enumeration retrieval). Aborting...\n')
                    return False
                node_trigger_overlap.SetIntValue(PySpin.TriggerOverlap_ReadOut)
            else:
                node_trigger_mode_off = node_trigger_mode.GetEntryByName('Off')
                if PySpin.IsReadable(node_trigger_mode_off):
                    node_trigger_mode.SetIntValue(node_trigger_mode_off.GetValue())

                # 重置触发源为软件触发
                node_trigger_source = PySpin.CEnumerationPtr(nodemap.GetNode('TriggerSource'))
                if PySpin.IsReadable(node_trigger_source) and PySpin.IsWritable(node_trigger_source):
                    node_trigger_source_software = node_trigger_source.GetEntryByName('Software')
                    if PySpin.IsReadable(node_trigger_source_software):
                        node_trigger_source.SetIntValue(node_trigger_source_software.GetValue()) 

        except PySpin.SpinnakerException as ex:
            print(f'设置触发模式错误: {ex}')

    def _set_acquisition_mode(self, nodemap):
        """设置采集模式"""
        try:
            node_acquisition_mode = PySpin.CEnumerationPtr(nodemap.GetNode('AcquisitionMode'))
            if not PySpin.IsAvailable(node_acquisition_mode) or not PySpin.IsWritable(node_acquisition_mode):
                print('采集模式节点不可用')
                return

            # 设置为连续采集模式
            node_acquisition_mode_continuous = node_acquisition_mode.GetEntryByName('Continuous')
            if PySpin.IsReadable(node_acquisition_mode_continuous):
                acquisition_mode_continuous = node_acquisition_mode_continuous.GetValue()
                node_acquisition_mode.SetIntValue(acquisition_mode_continuous)

        except PySpin.SpinnakerException as ex:
            print(f'设置采集模式错误: {ex}')

    def _set_framerate(self, nodemap):
        """设置帧率"""
        try:
            # 启用帧率控制
            node_acquisition_frame_rate_enable = PySpin.CBooleanPtr(nodemap.GetNode('AcquisitionFrameRateEnable'))
            if PySpin.IsAvailable(node_acquisition_frame_rate_enable) and PySpin.IsWritable(node_acquisition_frame_rate_enable):
                node_acquisition_frame_rate_enable.SetValue(True)

            # 设置帧率
            node_acquisition_frame_rate = PySpin.CFloatPtr(nodemap.GetNode('AcquisitionFrameRate'))
            if PySpin.IsAvailable(node_acquisition_frame_rate) and PySpin.IsWritable(node_acquisition_frame_rate):
                node_acquisition_frame_rate.SetValue(FLIR_FRAMERATE)

        except PySpin.SpinnakerException as ex:
            print(f'设置帧率错误: {ex}')

    def _enable_chunk_data(self, nodemap):
        """启用数据块模式"""
        try:
            # 启用ChunkMode
            node_chunk_mode_active = PySpin.CBooleanPtr(nodemap.GetNode('ChunkModeActive'))
            if not PySpin.IsAvailable(node_chunk_mode_active):
                print('ChunkMode节点不可用')
                return

            node_chunk_mode_active.SetValue(True)

            # 启用时间戳和曝光时间块
            node_chunk_selector = PySpin.CEnumerationPtr(nodemap.GetNode('ChunkSelector'))
            if not PySpin.IsAvailable(node_chunk_selector) or not PySpin.IsWritable(node_chunk_selector):
                print('ChunkSelector节点不可用')
                return

            # 启用时间戳块
            entry_chunk_timestamp = node_chunk_selector.GetEntryByName('Timestamp')
            if PySpin.IsReadable(entry_chunk_timestamp):
                node_chunk_selector.SetIntValue(entry_chunk_timestamp.GetValue())
                node_chunk_enable = PySpin.CBooleanPtr(nodemap.GetNode('ChunkEnable'))
                if PySpin.IsWritable(node_chunk_enable):
                    node_chunk_enable.SetValue(True)

            # 启用曝光时间块
            entry_chunk_exposure_time = node_chunk_selector.GetEntryByName('ExposureTime')
            if PySpin.IsReadable(entry_chunk_exposure_time):
                node_chunk_selector.SetIntValue(entry_chunk_exposure_time.GetValue())
                node_chunk_enable = PySpin.CBooleanPtr(nodemap.GetNode('ChunkEnable'))
                if PySpin.IsWritable(node_chunk_enable):
                    node_chunk_enable.SetValue(True)

        except PySpin.SpinnakerException as ex:
            print(f'启用数据块模式错误: {ex}')

    def _disable_chunk_data(self, nodemap):
        """禁用数据块模式
        Args:
            nodemap: 节点映射
        Returns:
            bool: 是否成功
        """
        try:
            # 获取ChunkSelector节点
            chunk_selector = PySpin.CEnumerationPtr(nodemap.GetNode('ChunkSelector'))
            if not PySpin.IsReadable(chunk_selector) or not PySpin.IsWritable(chunk_selector):
                print('无法访问ChunkSelector节点')
                return False

            # 禁用所有数据块
            entries = [PySpin.CEnumEntryPtr(chunk_selector_entry) for chunk_selector_entry in chunk_selector.GetEntries()]
            for chunk_selector_entry in entries:
                if not PySpin.IsReadable(chunk_selector_entry):
                    continue

                chunk_selector.SetIntValue(chunk_selector_entry.GetValue())
                chunk_enable = PySpin.CBooleanPtr(nodemap.GetNode('ChunkEnable'))
                
                if PySpin.IsWritable(chunk_enable):
                    chunk_enable.SetValue(False)

            # 关闭ChunkMode
            chunk_mode_active = PySpin.CBooleanPtr(nodemap.GetNode('ChunkModeActive'))
            if PySpin.IsWritable(chunk_mode_active):
                chunk_mode_active.SetValue(False)

            return True

        except PySpin.SpinnakerException as ex:
            print(f'禁用数据块模式错误: {ex}')
            return False

    def _reset_trigger(self, nodemap):
        """重置触发模式
        Args:
            nodemap: 节点映射
        Returns:
            bool: 是否成功
        """
        try:
            # 关闭触发模式
            node_trigger_mode = PySpin.CEnumerationPtr(nodemap.GetNode('TriggerMode'))
            if not PySpin.IsReadable(node_trigger_mode) or not PySpin.IsWritable(node_trigger_mode):
                print('无法访问触发模式节点')
                return False

            node_trigger_mode_off = node_trigger_mode.GetEntryByName('Off')
            if PySpin.IsReadable(node_trigger_mode_off):
                node_trigger_mode.SetIntValue(node_trigger_mode_off.GetValue())

            # 重置触发源为软件触发
            node_trigger_source = PySpin.CEnumerationPtr(nodemap.GetNode('TriggerSource'))
            if PySpin.IsReadable(node_trigger_source) and PySpin.IsWritable(node_trigger_source):
                node_trigger_source_software = node_trigger_source.GetEntryByName('Software')
                if PySpin.IsReadable(node_trigger_source_software):
                    node_trigger_source.SetIntValue(node_trigger_source_software.GetValue())

            return True

        except PySpin.SpinnakerException as ex:
            print(f'重置触发模式错误: {ex}')
            return False

    def acquire_images(self, cam, nodemap, path):
        """采集图像
        Args:
            cam: 相机实例
            nodemap: 节点映射
            path: 保存路径
        Returns:
            bool: 采集是否成功
        """
        try:
            images = np.empty((NUM_IMAGES, FLIR_HEIGHT, FLIR_WIDTH), dtype=np.uint8)
            timestamps = np.zeros(NUM_IMAGES, dtype=np.uint64)
            exposure_times = np.zeros(NUM_IMAGES, dtype=float)
            
            for i in range(NUM_IMAGES):
                if RUNNING.value == 0:
                    break
                image_result = cam.GetNextImage(1000)
                if image_result.IsIncomplete():
                    print(f'图像不完整: {image_result.GetImageStatus()}')
                    image_result.Release()
                    continue
                
                images[i] = image_result.GetNDArray()
                _, exposure_times[i], timestamps[i] = self.read_chunk_data(image_result)
                image_result.Release()
            
            cam.EndAcquisition()
            ACQUISITION_FLAG.value = 1  # 使用 .value 访问共享变量

            self._save_data(images, exposure_times, timestamps, path)
            
            # 添加重置
            self._disable_chunk_data(nodemap)
            self._reset_trigger(nodemap)
            
            return True
            
        except PySpin.SpinnakerException as ex:
            print(f'图像采集错误: {ex}')
            RUNNING.value = 0
            return False

    def _save_data(self, images, exposure_times, timestamps, path):
        """保存采集数据
        Args:
            images: 图像数据
            exposure_times: 曝光时间
            timestamps: 时间戳
            path: 保存路径
        """
        # 创建 FLIR 数据目录
        flir_dir = os.path.join(path, "flir")
        preview_dir = os.path.join(flir_dir, "preview_images")
        os.makedirs(preview_dir, exist_ok=True)

        # 保存原始数据
        with open(os.path.join(flir_dir, "images.raw"), 'wb') as f:
            # 写入头信息
            header = np.array([NUM_IMAGES, FLIR_OFFSET_X, FLIR_OFFSET_Y, 
                             FLIR_WIDTH, FLIR_HEIGHT], dtype=np.int32)
            f.write(header.tobytes())
            
            # 写入图��数据并生成预览图
            for idx, img_data in enumerate(images):
                f.write(img_data.tobytes())
                if idx % 10 == 0:
                    
                    # 确保使用正确的 Bayer 模式进行转换
                    if FLIR_OFFSET_X % 2 == 0 and FLIR_OFFSET_Y % 2 == 0:
                        rgb_image = cv.cvtColor(img_data, cv.COLOR_BayerRG2RGB)
                    else:
                        # 根据偏移量选择正确的 Bayer 模式
                        bayer_patterns = {
                            (0, 0): cv.COLOR_BayerRG2RGB,  # RG
                            (1, 0): cv.COLOR_BayerGR2RGB,  # GR
                            (0, 1): cv.COLOR_BayerGB2RGB,  # GB
                            (1, 1): cv.COLOR_BayerBG2RGB   # BG
                        }
                        pattern_key = (FLIR_OFFSET_X % 2, FLIR_OFFSET_Y % 2)
                        rgb_image = cv.cvtColor(img_data, bayer_patterns[pattern_key])
                    
                    cv.imwrite(os.path.join(preview_dir, f'preview_{idx}.png'), 
                             cv.cvtColor(rgb_image, cv.COLOR_RGB2BGR))

        # 保存时间信息到 FLIR 目录
        np.savetxt(os.path.join(flir_dir, 'exposure_times.txt'), exposure_times)
        np.savetxt(os.path.join(flir_dir, 'timestamps.txt'), timestamps)

    def read_chunk_data(self, image):
        """读取图像块数据
        Args:
            image: 图像对象
        Returns:
            tuple: (成功标志, 曝光时间, 时间戳)
        """
        try:
            chunk_data = image.GetChunkData()
            return True, chunk_data.GetExposureTime(), chunk_data.GetTimestamp()
        except PySpin.SpinnakerException as ex:
            print(f'数据块读取错误: {ex}')
            return False, 0, 0

    def cleanup(self):
        """清理相机资源"""
        try:
            if self.cam_list:
                try:
                    self.cam_list.Clear()
                    print("相机列表已清理")
                except PySpin.SpinnakerException as ex:
                    print(f"清理相机列表错误: {ex}")
                self.cam_list = None

            if self.system:
                try:
                    self.system.ReleaseInstance()
                    print("系统实例已释放")
                except PySpin.SpinnakerException as ex:
                    print(f"释放系统实例错误: {ex}")
                finally:
                    self.system = None   
        except Exception as ex:
            print(f"资源清理错误: {ex}")
