
a=candidate:2 1 UDP 2130706175 fd32:16ee:71c8::2 52073 typ host
a=candidate:1 1 UDP 2122317823 192.168.0.5 52073 typ host

候选者以a=candidate:开头

字段对比
字段	候选者 1	候选者 2	说明
foundation	2	1	基础值，用于区分不同类型的候选。通常是随机生成的数字。
component-id	1	1	表示组件 ID，1 通常表示 RTP 数据流。
transport	UDP	UDP	传输协议，这里都是 UDP。
priority	2130706175	2122317823	优先级，用于 ICE 选择过程。优先级越高，越有可能被选中。
ip	fd32:16ee:71c8::2	192.168.0.5	候选的 IP 地址。前者是 IPv6 地址，后者是 IPv4 地址。
port	52073	52073	候选的端口号，这里两者相同。
type	host	host	候选类型，host 表示本地主机的候选。

IPv6的跑不通, IPv4的可以跑通