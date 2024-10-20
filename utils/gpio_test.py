import Jetson.GPIO as GPIO
import time

# 设置GPIO模式为BOARD（物理引脚编号）
GPIO.setmode(GPIO.BOARD)

# 定义要测试的GPIO引脚，比如Pin 12
output_pin = 18  # 可以根据你的实际需求修改这个引脚编号

# 将引脚设置为输出模式
GPIO.setup(output_pin, GPIO.OUT)

try:
    while True:
        # 设置引脚为高电平
        GPIO.output(output_pin, GPIO.HIGH)
        print(f"引脚 {output_pin}: 高电平")
        time.sleep(0.1)

        # 设置引脚为低电平
        GPIO.output(output_pin, GPIO.LOW)
        print(f"引脚 {output_pin}: 低电平")
        time.sleep(0.1)

except KeyboardInterrupt:
    # 捕获Ctrl+C信号来终止程序
    print("程序终止")

finally:
    # 清理GPIO设置
    GPIO.cleanup()

