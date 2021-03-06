#include "scheduler.h"
#include <memory>
#include "log_util.h"
#include "websocket_parser.h"

namespace tcp
{
namespace web
{
Scheduler::Scheduler()
{
    tcp_scheduler_ = nullptr;
    msg_codec_ = nullptr;
}

Scheduler::~Scheduler()
{
}

int Scheduler::SendToClient(const ConnGUID* conn_guid, const void* data, size_t len)
{
    return tcp_scheduler_->SendToClient(conn_guid, data, len);
}

int Scheduler::SendWSMsgToClient(const ConnGUID* conn_guid, ws::FrameType frame_type, const void* data, size_t len)
{
    // TODO len超过多少时分帧发送？
    const int flags = (ws::TEXT_FRAME == frame_type ? WS_OP_TEXT : WS_OP_BINARY) | WS_FINAL_FRAME;
    const size_t frame_len = websocket_calc_frame_size((websocket_flags) flags, len);

    char* frame = (char*) malloc(frame_len);
    if (nullptr == frame)
    {
        LOG_ERROR("failed to alloc memory");
        return -1;
    }

    websocket_build_frame(frame, (websocket_flags) flags, nullptr, (const char*) data, len);
    tcp_scheduler_->SendToClient(conn_guid, frame, frame_len);
    free(frame);

    return 0;
}

int Scheduler::CloseClient(const ConnGUID* conn_guid)
{
    return tcp_scheduler_->CloseClient(conn_guid);
}

int Scheduler::SendToGlobalThread(const ConnGUID* conn_guid, const ::proto::MsgHead& msg_head,
                                  const void* msg_body, size_t msg_body_len)
{
    return SendToThread(THREAD_TYPE_GLOBAL, conn_guid, msg_head, msg_body, msg_body_len, 0);
}

int Scheduler::SendToWorkThread(const ConnGUID* conn_guid, const ::proto::MsgHead& msg_head,
                                const void* msg_body, size_t msg_body_len, int work_thread_idx)
{
    return SendToThread(THREAD_TYPE_WORK, conn_guid, msg_head, msg_body, msg_body_len, work_thread_idx);
}

int Scheduler::SendToWebThread(const ConnGUID* conn_guid, const proto::MsgHead& msg_head,
                               const void* msg_body, size_t msg_body_len, int tcp_thread_idx)
{
    return SendToThread(THREAD_TYPE_TCP, conn_guid, msg_head, msg_body, msg_body_len, tcp_thread_idx);
}

int Scheduler::SendToThread(int thread_type, const ConnGUID* conn_guid, const ::proto::MsgHead& msg_head,
                            const void* msg_body, size_t msg_body_len, int thread_idx)
{
    switch (thread_type)
    {
        case THREAD_TYPE_GLOBAL:
        {
            return tcp_scheduler_->SendToGlobalThread(conn_guid, msg_head, msg_body, msg_body_len);
        }
        break;

        case THREAD_TYPE_WORK:
        {
            return tcp_scheduler_->SendToWorkThread(conn_guid, msg_head, msg_body, msg_body_len, thread_idx);
        }
        break;

        case THREAD_TYPE_TCP:
        {
            return tcp_scheduler_->SendToTCPThread(conn_guid, msg_head, msg_body, msg_body_len, thread_idx);
        }
        break;

        default:
        {
            LOG_ERROR("invalid thread type: " << thread_type);
            return -1;
        }
        break;
    }

    return 0;
}
}
}
