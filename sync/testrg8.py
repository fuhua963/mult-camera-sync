import cv2 as cv
import numpy as np
import os
import glob

def ensure_dir(directory):
    """确保目录存在，如果不存在则创建"""
    if not os.path.exists(directory):
        os.makedirs(directory)

def convert_rg8_to_rgb(input_folder, output_folder):
    """
    将RG8格式的图像转换为RGB格式
    :param input_folder: 输入文件夹路径，包含RG8格式的图像
    :param output_folder: 输出文件夹路径，用于保存转换后的RGB图像
    """
    # 确保输出目录存在
    ensure_dir(output_folder)
    
    # 获取输入文件夹中的所有图像文件
    image_files = glob.glob(os.path.join(input_folder, "*.raw"))
    
    for image_file in sorted(image_files):
        # 从文件名获取基本信息
        base_name = os.path.splitext(os.path.basename(image_file))[0]
        
        # 读取raw文件
        with open(image_file, 'rb') as f:
            # 读取ROI信息
            offset_x, offset_y, width, height = np.fromfile(f, dtype=np.int32, count=4)
            # 读取图像数据
            bayer_data = np.fromfile(f, dtype=np.uint8).reshape((height, width))
            
            # 计算Bayer模式的偏移
            x_odd = (offset_x % 2) == 1
            y_odd = (offset_y % 2) == 1
            
            # 根据偏移选择正确的Bayer模式
            bayer_patterns = {
                (False, False): cv.COLOR_BAYER_RG2RGB,  # 偶数行偶数列 -> RGGB
                (True, False): cv.COLOR_BAYER_GR2RGB,   # 奇数行偶数列 -> GRBG
                (False, True): cv.COLOR_BAYER_GB2RGB,   # 偶数行奇数列 -> GBRG
                (True, True): cv.COLOR_BAYER_BG2RGB     # 奇数行奇数列 -> BGGR
            }
            
            bayer_pattern = bayer_patterns[(x_odd, y_odd)]
            
            # 转换为RGB
            rgb_image = cv.cvtColor(bayer_data, bayer_pattern)
            
            # 保存转换后的图像
            output_path = os.path.join(output_folder, f"{base_name}.png")
            cv.imwrite(output_path, cv.cvtColor(rgb_image, cv.COLOR_RGB2BGR))
            
            print(f"已转换: {base_name}")

if __name__ == "__main__":
    # 设置输入和输出文件夹路径
    input_folder = "raw"  # 包含RG8格式图像的文件夹
    output_folder = "rgb"  # 转换后的RGB图像保存文件夹
    
    try:
        convert_rg8_to_rgb(input_folder, output_folder)
        print("转换完成！")
    except Exception as e:
        print(f"转换过程中出错: {str(e)}")