authkey：ae655874d3f1

GET https://iot.diandeng.tech/api/v1/user/device/diy/auth?authKey=ae655874d3f1&protocol=mqtt

添加参数protocol可指定连接协议，目前支持的协议有：mqtt、mqtts、ws、wss

eg:
{
    "detail": {
    "broker": "blinker",
    "deviceName": "41A7E676A8DL98SZNS2AP7AB", 
    "host": "mqtt://broker.diandeng.tech", 
    "iotId": "7Q5QNKSYBzEoZR3c9j9N9cjxM2", 
    "iotToken": "GQmBgLYpihzZC22DjQbcgMZixrfUFBVE", 
    "port": "1883", 
    "productKey": "blinker", 
    "uuid": "2146c54356de11eb81bb5254"
    }, 
    
    "message": 1000
}


"{"toDevice":"2146c54356de11eb81bb5254","data":{"state":"online"}}"

关于cJSON在keil项目中使用出现打印为0或乱码原因 -> 还通过wireshark发现拼接后发送的JSON数据并不是拼接完的数据，而是只发出去了一部分：
1.mcu分配堆内存不够
2.