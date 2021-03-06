# common_component
C++ common components used in Linux server programming, including framework components

verified in CentOS7/LinuxMint18/Ubuntu16.04 and above

公共组件，包括基础库(base)、日志引擎(log_engine)、配置中心(conf_center)、定时器(timer_axis)、线程管理(thread_center、tcp_threads、 ws_threads、udp_threads、global_thread、work_threads、burden_threads)、网络通信(tcp、websocket、http、udp)、app launcher、app frame等，每个组件都是一个独立的工程。

## app_frame
app框架，根据配置启动tcp、websocket(含http)、udp、global、work、burden线程组。

## burden_threads
burden线程组，是第三级线程组。app_frame通过dlopen的方式加载burden_threads。burden_theads对thread_center进行了包装，是一种类型的thread_center，其他线程组也是这样。可以调度任务给所有的线程组，包括burden_threads。

## client_center_mgr
client指的是连接其他服务的一方。提供了通过tcp、udp、http/https异步请求其他服务的接口。thread_center通过dlopen的方式加载client_center_mgr。

## conn_center_mgr
conn指的是作为服务器接收客户端的连接。tcp_threads使用其中的tcp_conn_center,udp_threads使用其中的udp_conn_center,http_threads使用其中的http_conn_center。分别管理各自类型的连接和io。

## doc
文档目录。

## global_thread
全局线程，其中执行一些全局唯一的逻辑。可以调度任务给io(tcp、udp、http)线程组、work_threads、burden_threads。广播或者制定线程均可。

## http_threads
http线程组，监听http连接并处理io，是第一级线程组，也是一种io线程组。可以调度任务给work_threads、global_threads。

## tcp_threads
tcp线程组，监听tcp连接并处理io，是第一级线程组，也是一种io线程组。可以调度任务给work_threads、global_threads。

## third_party
公共cmake文件，定义的是第三方库的inc、link路径和lib名。

## trans_center
通用的异步事务接口。一个线程使用一个trans_center管理异步事务。

## udp_threads
udp线程组，监听udp请求并处理io，是第一级线程组，也是一种io线程组。可以调度任务给work_threads、global_threads。

## work_threads
work线程组，是第二级线程组。可以调度任务给所有的线程组，包括work_threads。

线程总数=main thread(1) + global thread(1) + listen thread(1) + io threads count + work threads count + burden threads count + http threads count + udp threads count




//
## event_center
事件订阅和发布。降低模块之间的耦合。

## msg_codec_center
消息编解码器。实现了自定义tcp协议的编解码。

## random_engine
随机数引擎。

## script_engine
脚本引擎。实现了对lua脚本的调用。

## time_service
时间服务接口。
