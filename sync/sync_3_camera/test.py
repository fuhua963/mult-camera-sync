import PySpin

def main():
    """FLIR相机基础使用流程示例"""
    try:
        # 1. 获取系统实例
        system = PySpin.System.GetInstance()
        
        # 2. 获取相机列表
        cam_list = system.GetCameras()
        if cam_list.GetSize() == 0:
            print("未检测到相机")
            return False
            
        # 3. 获取第一个相机
        cam = cam_list[0]
        
        # 4. 初始化相机
        cam.Init()
        
        # 5. 简单配置
        # 设置连续采集模式
        nodemap = cam.GetNodeMap()
        node_acquisition_mode = PySpin.CEnumerationPtr(nodemap.GetNode('AcquisitionMode'))
        if PySpin.IsAvailable(node_acquisition_mode) and PySpin.IsWritable(node_acquisition_mode):
            node_acquisition_mode_continuous = node_acquisition_mode.GetEntryByName('Continuous')
            acquisition_mode_continuous = node_acquisition_mode_continuous.GetValue()
            node_acquisition_mode.SetIntValue(acquisition_mode_continuous)
        
        # 6. 开始采集
        cam.BeginAcquisition()
        
        # 7. 抓取5张图片
        for i in range(5):
            image_result = cam.GetNextImage(1000)
            if image_result.IsIncomplete():
                print(f"图像获取不完整: {image_result.GetImageStatus()}")
            else:
                print(f"成功获取图像 {i+1}")
            image_result.Release()
            
        # 8. 停止采集
        cam.EndAcquisition()
        
        # 9. 清理资源
        cam.DeInit()
        del cam
        cam_list.Clear()
        try:
            system.ReleaseInstance()
        except Exception as e:
            print(f"释放系统实例错误: {e}")
        
        return True
        
    except PySpin.SpinnakerException as ex:
        print(f"错误: {ex}")
        return False

if __name__ == '__main__':
    main()
