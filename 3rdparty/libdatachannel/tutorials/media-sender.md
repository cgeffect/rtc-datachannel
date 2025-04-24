
1. 先使用google浏览器打开main.html

2. 运行media-receiver程序
    把打印出来的sdp复制到浏览器里, 点击提交, 然后会返回answer的sdp, 再把answer的sdp复制到media-server的控制台, 回车. 
    双方建立连接
3. 发送流数据, 使用这个命令采集摄像头发送到浏览器
    gst-launch-1.0 avfvideosrc device-index=0 ! video/x-raw,width=640,height=480 ! videoconvert ! queue ! x264enc tune=zerolatency bitrate=1000 key-int-max=30 ! video/x-h264, profile=constrained-baseline ! rtph264pay pt=96 mtu=1200 ! udpsink host=127.0.0.1 port=6000
4. 浏览器可以看到采集到的画面


