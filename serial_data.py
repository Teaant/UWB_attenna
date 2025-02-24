import serial  
import csv  
import time  

# 配置串口参数，请根据你的设置修改  
COM_PORT = 'COM3'  # 或 '/dev/ttyUSB0' (Linux/Mac)  
BAUD_RATE = 115200  # 波特率要和STM32一致  
TIMEOUT = None  # 超时时间（秒）  

# 创建串口对象  
ser = serial.Serial(COM_PORT, BAUD_RATE, timeout=TIMEOUT)  

# 等待连接稳定  
time.sleep(2)  

# CSV文件名  
csv_file = 'aoa_param.csv'  

# 打开CSV文件以写入  
with open(csv_file, mode='a', newline='') as file:  
    writer = csv.writer(file)  
    
    # 写入CSV文件标题（如果需要的话）  
    writer.writerow(['Sequence', 'Index', 'Phi', 'Beta'])  

    try:  
        print("开始接收数据...")  
        while True:  
            # 从串口读取一行数据  
            line = ser.readline().decode('utf-8').strip()  
            if line:  
                # 分割数据根据逗号  
                data = line.split(',')  
                if len(data) == 4:  # 确保接收到的字段数量正确  
                    try:  
                        # 将数据写入CSV文件  
                        writer.writerow(data)  
                        print(f"已写入数据: {data}")  
                    except Exception as e:  
                        print(f"写入错误: {e}")  
                else:  
                    print(f"警告: 数据格式不正确: {line}")  

    except KeyboardInterrupt:  
        print("程序已停止.")  
    except Exception as e:  
        print(f"发生错误: {e}")  
    finally:  
        ser.close()  
        print("串口已关闭.")