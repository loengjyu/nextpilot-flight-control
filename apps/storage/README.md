# 数据存储

该文件夹存放 **数据存储** 相关功能模块，包括航线和围栏、飞行日志、系统日志、片上文件系统。

- dataman，用于存储和管理飞行航线、地理围栏、应急备降点
- logger，飞控日志存储和管理，通过订阅uORB消息，按照设定的频率保存到SD卡
- romfs，片上文件系统