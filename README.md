### W5500EVB通过MQTT协议连接Blinker云平台

#### 硬件平台
W5500EVB
W5500EVB以STM32F103RCT6这款主流的单片机作为MCU，以太网接口部分采用W5500，该芯片不但集成了以太网的MAC和PHY，而且还用硬件逻辑实现了网络层和传输层。W5500通过标准四线高速SPI接口 与MCU进行数据交互。W5500杰出的硬件TCP/IP协议栈内核不仅能够提供8路相互独立的通信，而且高速、安全、稳定，广泛用于工业领域。
https://www.w5500.com/index.html

#### 云平台
点灯科技
https://www.diandeng.tech/home

#### MQTT协议
MQTT是一个客户端服务端架构的发布/订阅模式的消息传输协议。它的设计思想是轻巧、开放、简单、规范，易于实现。这些特点使得它对很多场景来说都是很好的选择，特别是对于受限的环境如机器与机器的通信（M2M）以及物联网环境（IoT）。

协议的具体规范可以在 https://mcxiaoke.gitbooks.io/mqtt-cn/content/ 浏览。


#### 本例程使用版本库-Eclipse Paho MQTT
**https://github.com/eclipse/paho.mqtt.embedded-c**

#### 协议具体使用
我们只需要将库中MQTTPacket文件夹的src移植自己的工程中，再将sample中的transport.c以及transport.h也移植到自己的工程。由于paho库已经实现了MQTT的功能，我们只需要将transport.c中的函数修改为对应自己使用设备的输入输出。sample文件夹中也有官方demo可以参考。


#### 例程操作步骤

1.注册blinker平台，在手机app上注册设备，可以拿到注册设备的authkey。如authkey：ae655874d3f1

2.发送get请求，链接里携带authkey以及通讯的协议信息(可在浏览器输入网址处进行)

 https://iot.diandeng.tech/api/v1/user/device/diy/auth?authKey=ae655874d3f1&protocol=mqtt


添加参数protocol可指定连接协议，目前支持的协议有：mqtt、mqtts、ws、wss

服务器返回信息：
```
{
    "detail": 
    {
        "broker": "blinker",
        "deviceName": "41A7E676A8DL98SZNS2AP7AB",       //clientID
        "host": "mqtt://broker.diandeng.tech",          //mqtt服务器地址，可以ping到ip地址
        "iotId": "7Q5QNKSYBzEoZR3c9j9N9cjxM2",          //账户
        "iotToken": "GQmBgLYpihzZC22DjQbcgMZixrfUFBVE", //密码
        "port": "1883",                                 //端口号
        "productKey": "blinker", 
        "uuid": "2146c54356de11eb81bb5254"              //设备身份
    },     
    "message": 1000
}
```



3.将拿到的信息加入代码中，即可连接点灯科技的mqtt服务器，并在手机app上看到设备在线信息。




