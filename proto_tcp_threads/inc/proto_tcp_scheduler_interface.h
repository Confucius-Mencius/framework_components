#ifndef PROTO_TCP_THREADS_INC_PROTO_TCP_SCHEDULER_INTERFACE_H_
#define PROTO_TCP_THREADS_INC_PROTO_TCP_SCHEDULER_INTERFACE_H_

struct ConnGUID;

namespace proto
{
struct MsgHead;
}

namespace tcp
{
namespace proto
{
class SchedulerInterface
{
public:
    virtual ~SchedulerInterface()
    {
    }

    /**
     * @brief send to client，可以发往其它io线程管理的客户端
     * @return
     */
    virtual int SendToClient(const ConnGUID* conn_guid, const ::proto::MsgHead& msg_head,
                             const void* msg_body, size_t msg_body_len) = 0;

    /**
     * @brief send to client，send序列化好的数据，可以发往其它io线程管理的客户端
     * @return
     */
    virtual int SendToClient(const ConnGUID* conn_guid, const void* data, size_t len) = 0;

    /**
     * @brief 断开与客户端的连接，可以关闭其它io线程管理的客户端
     * @param conn_guid
     * @return
     */
    virtual int CloseClient(const ConnGUID* conn_guid) = 0;

    // 注意：线程之间的调度不使用异步事务
    virtual int SendToGlobalThread(const ConnGUID* conn_guid, const ::proto::MsgHead& msg_head,
                                   const void* msg_body, size_t msg_body_len) = 0;

    /**
     * @brief
     * @param work_thread_idx 为-1则由框架自行调度一个work线程
     * @return
     */
    virtual int SendToWorkThread(const ConnGUID* conn_guid, const ::proto::MsgHead& msg_head,
                                 const void* msg_body, size_t msg_body_len, int work_thread_idx) = 0;

    /**
     * @brief 发送给proto tcp thread，包括自己
     * @param proto_tcp_thread_idx 为-1则由框架自行调度一个proto tcp线程
     * @return
     */
    virtual int SendToProtoTCPThread(const ConnGUID* conn_guid, const ::proto::MsgHead& msg_head,
                                     const void* msg_body, size_t msg_body_len, int proto_tcp_thread_idx) = 0;
};
}
}

#endif // PROTO_TCP_THREADS_INC_PROTO_TCP_SCHEDULER_INTERFACE_H_
