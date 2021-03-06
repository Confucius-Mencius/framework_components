#ifndef WORK_THREADS_SRC_SCHEDULER_H_
#define WORK_THREADS_SRC_SCHEDULER_H_

#include "proto_msg_codec.h"
#include "work_scheduler_interface.h"
#include "work_threads_interface.h"

namespace work
{
class ThreadSink;

class Scheduler : public SchedulerInterface
{
public:
    Scheduler();
    virtual ~Scheduler();

    void SetThreadSink(ThreadSink* sink)
    {
        thread_sink_ = sink;
    }

    void SetMsgCodec(::proto::MsgCodec* msg_codec)
    {
        msg_codec_ = msg_codec;
    }

    int Initialize(const void* ctx);
    void Finalize();

    void SetRelatedThreadGroups(RelatedThreadGroups* related_thread_groups);

    ///////////////////////// SchedulerInterface /////////////////////////
    int SendToClient(const ConnGUID* conn_guid, const ::proto::MsgHead& msg_head,
                     const void* msg_body, size_t msg_body_len);
    int SendToClient(const ConnGUID* conn_guid, const void* data, size_t len);
    int CloseClient(const ConnGUID* conn_guid);
    int SendToGlobalThread(const ConnGUID* conn_guid, const ::proto::MsgHead& msg_head,
                           const void* msg_body, size_t msg_body_len) override;
    int SendToWorkThread(const ConnGUID* conn_guid, const ::proto::MsgHead& msg_head,
                         const void* msg_body, size_t msg_body_len, int work_thread_idx) override;
    int SendToBurdenThread(const ConnGUID* conn_guid, const ::proto::MsgHead& msg_head,
                           const void* msg_body, size_t msg_body_len, int burden_thread_idx) override;
    int SendToTCPThread(const ConnGUID* conn_guid, const ::proto::MsgHead& msg_head,
                        const void* msg_body, size_t msg_body_len, int tcp_thread_idx) override;
    int SendToProtoTCPThread(const ConnGUID* conn_guid, const ::proto::MsgHead& msg_head,
                             const void* msg_body, size_t msg_body_len, int proto_tcp_thread_idx) override;
    int SendToWebThread(const ConnGUID* conn_guid, const ::proto::MsgHead& msg_head,
                        const void* msg_body, size_t msg_body_len, int web_thread_idx) override;
    TransID SendToServer(const Peer& peer, const ::proto::MsgHead& msg_head,
                         const void* msg_body, size_t msg_body_len, const AsyncCtx* async_ctx) override;
    TransID HTTPGet(const Peer& peer, const http::GetParams& params, const AsyncCtx* async_ctx) override;
    TransID HTTPPost(const Peer& peer, const http::PostParams& params, const AsyncCtx* async_ctx) override;
    void CancelTrans(TransID trans_id) override;

private:
    int GetScheduleWorkThreadIdx(int work_thread_idx);
    int GetScheduleBurdenThreadIdx(int burden_thread_idx);
    int GetScheduleTCPThreadIdx(int tcp_thread_idx);
    int GetScheduleProtoTCPThreadIdx(int proto_tcp_thread_idx);
    int GetScheduleWebThreadIdx(int web_thread_idx);

    enum
    {
        THREAD_TYPE_GLOBAL,
        THREAD_TYPE_WORK,
        THREAD_TYPE_BURDEN,
        THREAD_TYPE_TCP,
        THREAD_TYPE_PROTO_TCP,
        THREAD_TYPE_WEB_TCP,
    };

    int SendToThread(int thread_type, const ConnGUID* conn_guid, const ::proto::MsgHead& msg_head,
                     const void* msg_body, size_t msg_body_len, int thread_idx);

private:
    ThreadSink* thread_sink_;
    ::proto::MsgCodec* msg_codec_;
    const ThreadsCtx* threads_ctx_;
    RelatedThreadGroups* related_thread_groups_;

    int last_work_thread_idx_;
    int last_burden_thread_idx_;
    int last_tcp_thread_idx_;
    int last_proto_tcp_thread_idx_;
    int last_web_thread_idx_;
};
}

#endif // WORK_THREADS_SRC_SCHEDULER_H_
