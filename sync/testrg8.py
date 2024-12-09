import cv2 as cv
import numpy as np
import os
import glob
import argparse

def ensure_dir(directory):
    """确保目录存在，如果不存在则创建"""
    if not os.path.exists(directory):
        os.makedirs(directory)

def convert_rg8_to_rgb(input_path, output_folder=None):
    """
    将RG8格式的图像转换为RGB格式
    :param input_path: 输入文件夹路径，包含images.raw文件
    :param output_folder: 输出文件夹路径，用于保存转换后的RGB图像。如果为None，则在输入目录下创建rgb文件夹
    """
    # 如果output_folder为None，则在输入目录下创建rgb文件夹
    if output_folder is None:
        output_folder = os.path.join(os.path.dirname(input_path), "rgb")
    
    # 确保输出目录存在
    ensure_dir(output_folder)
    
    try:
        # 读取raw文件
        with open(input_path, 'rb') as f:
            # 读取文件头信息
            header = np.fromfile(f, dtype=np.int32, count=5)
            num_images = header[0]
            offset_x = header[1]
            offset_y = header[2]
            width = header[3]
            height = header[4]
            
            print(f"图像信息��")
            print(f"图像数量: {num_images}")
            print(f"偏移量: ({offset_x}, {offset_y})")
            print(f"尺寸: {width} x {height}")
            
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
            
            # 读取并处理每张图像
            for i in range(num_images):
                # 读取单张图像数据
                bayer_data = np.fromfile(f, dtype=np.uint8, count=width*height)
                if len(bayer_data) != width*height:
                    print(f"警告：图像 {i} 数据不完整，跳过")
                    continue
                    
                bayer_data = bayer_data.reshape((height, width))
                
                # 转换为RGB
                rgb_image = cv.cvtColor(bayer_data, bayer_pattern)
                
                # 保存转换后的图像
                output_path = os.path.join(output_folder, f"image_{i:04d}.png")
                cv.imwrite(output_path, cv.cvtColor(rgb_image, cv.COLOR_RGB2BGR))
                
                if i % 10 == 0:
                    print(f"已处理: {i}/{num_images}")
                
            print(f"处理完成！共转换 {num_images} 张图像")

    except FileNotFoundError:
        print(f"错误：找不到文件 {input_path}")
    except Exception as e:
        print(f"处理过程中出错: {str(e)}")

def main():
    # 创建命令行参数解析器
    parser = argparse.ArgumentParser(description='将RG8格式的图像转换为RGB格式')
    parser.add_argument('input_path', help='输入文件路径，包含images.raw文件')
    parser.add_argument('--output', '-o', help='输出文件夹路径（可选）')
    
    # 解析命令行参数
    args = parser.parse_args()
    
    # 检查输入路径
    if not os.path.isfile(args.input_path):
        if os.path.isdir(args.input_path):
            # 如果输入是目录，则查找images.raw文件
            raw_file = os.path.join(args.input_path, "raw", "images.raw")
            if not os.path.isfile(raw_file):
                print(f"错误：在目录 {args.input_path} 中找不到 raw/images.raw 文件")
                return
            input_path = raw_file
        else:
            print(f"错误：输入路径 {args.input_path} 不存在")
            return
    else:
        input_path = args.input_path
    
    # 转换图像
    convert_rg8_to_rgb(input_path, args.output)

if __name__ == "__main__":
    main()