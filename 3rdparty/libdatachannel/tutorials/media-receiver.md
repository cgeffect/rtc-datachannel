
1. 先使用google浏览器打开main.html

2. 运行media-receiver程序
    把打印出来的sdp复制到浏览器里, 点击提交, 然后会返回answer的sdp, 再把answer的sdp复制到media-server的控制台, 回车. 
    双方建立连接

    然后这个时候, 浏览器就会不停地采集摄像头的数据发送到media-server里, 在
    
    track->onMessage([session, sock, addr](rtc::binary message) {
                std::cout << message.size() << std::endl;
			    // This is an RTP packet
			    sendto(sock, reinterpret_cast<const char *>(message.data()), int(message.size()), 0,
			           reinterpret_cast<const struct sockaddr *>(&addr), sizeof(addr));
		    },
		    nullptr);
    的回调函数里会传输rtp数据, sendto是把数据发送到本地的127.0.0.1:5000的端口, 使用 下面的命令可以看到预览效果
    gst-launch-1.0 udpsrc address=127.0.0.1 port=5000 caps="application/x-rtp" ! queue ! rtph264depay ! video/x-h264,stream-format=byte-stream ! queue ! avdec_h264 ! queue ! autovideosink
    这个回调函数就是压缩的264数据
