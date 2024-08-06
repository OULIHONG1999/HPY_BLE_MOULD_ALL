**仓库说明**
此仓库包含了一对多的蓝牙控制器（蓝牙拓展模块）和蓝牙传感器的驱动程序，使用的芯片为杰理AC6321。

程序设计支持最多四个设备同时链接，并进行数据的交换

蓝牙控制器（蓝牙拓展模块）作为客户端，链接多个蓝牙传感器（服务端），传感器将数据发送到蓝牙控制器，控制器汇总数据并整理，通过串口发送给上位机，实现传感器数据的采集。

相反，上位机通过串口发送数据给蓝牙控制器，蓝牙控制器分析命令然后发送到指定的传感器并进行控制。


**注意：传输协议当且未完全确定，需要在进一步开发时，确定协议功能，此仓库大概不会再更新！！**

**修改日志**
2024年7月1日
1. 添加了蓝牙拓展模块和蓝牙传感器的远端仓库
2. 此仓库作为备份文件
3. 实际驱动，以远端仓库的修改为准
4. 文件缺乏pcb文件
