
# demo 运行流程
## 1. client 运行

    1.先运行信令服务, 
    运行signaling-server-python

    使用python服务
    python服务运行步骤
    先安装虚拟环境, 安装虚拟环境的目的是不影响系统的python环境

    
    python3 -m venv path/to/venv
    source path/to/venv/bin/activate
    pip install websockets
    
    运行信令
    python3 signaling-server.py

    使用Xcode运行datachannel-client, 或者命令行运行两个client程序

    2.运行client程序
    运行client程序, 运行两个client程序, 把其中一个的roomid设置为另一个的roomid, 这样两个client就可以连接起来了
    
