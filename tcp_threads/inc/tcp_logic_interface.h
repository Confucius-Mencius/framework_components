#ifndef TCP_THREADS_INC_TCP_LOGIC_INTERFACE_H_
#define TCP_THREADS_INC_TCP_LOGIC_INTERFACE_H_

#include <stddef.h>
#include <atomic>
#include "module_interface.h"

class ConfCenterInterface;
class TimerAxisInterface;
class ConnCenterInterface;
struct event_base;
struct ConnGUID;
class ThreadInterface;

namespace global
{
class LogicInterface;
}

namespace proto
{
class MsgDispatcherInterface;
}

namespace tcp
{
class SchedulerInterface;
class MsgDispatcherInterface;
class CommonLogicInterface;

struct LogicCtx
{
    int argc;
    char** argv;
    const char* common_component_dir;
    const char* cur_working_dir;
    const char* app_name;
    ConfCenterInterface* conf_center;
    TimerAxisInterface* timer_axis;
    ConnCenterInterface* conn_center;
    SchedulerInterface* scheduler;
    ::proto::MsgDispatcherInterface* msg_dispatcher;
    CommonLogicInterface* common_logic;
    struct event_base* thread_ev_base;
    int thread_idx;
    const void* logic_args;

    LogicCtx()
    {
        argc = 0;
        argv = nullptr;
        common_component_dir = nullptr;
        cur_working_dir = nullptr;
        app_name = nullptr;
        conf_center = nullptr;
        timer_axis = nullptr;
        conn_center = nullptr;
        scheduler = nullptr;
        msg_dispatcher = nullptr;
        common_logic = nullptr;
        thread_ev_base = nullptr;
        thread_idx = -1;
        logic_args = nullptr;
    }
};

class LogicInterface : public ModuleInterface
{
public:
    LogicInterface() : logic_ctx_()
    {
        can_exit_ = false;
        global_logic_ = nullptr;
    }

    virtual ~LogicInterface()
    {
    }

    void SetGlobalLogic(global::LogicInterface* global_logic)
    {
        global_logic_ = global_logic;
    }

    virtual int Initialize(const void* ctx)
    {
        if (nullptr == ctx)
        {
            return -1;
        }

        logic_ctx_ = *(static_cast<const LogicCtx*>(ctx));
        return 0;
    }

    virtual void OnStop()
    {
    }

    virtual void OnReload()
    {
    }

    LogicCtx* GetLogicCtx() const
    {
        return const_cast<LogicCtx*>(&logic_ctx_);
    }

    bool CanExit() const
    {
        return can_exit_;
    }

    /**
     * 连接管理接口，当有新的客户端连上来时会回调到这里
     * @param conn_guid
     */
    virtual void OnClientConnected(const ConnGUID* conn_guid)
    {
    }

    /**
     * 连接管理接口，当有客户端连接断开时会回调到这里，包括服务器主动关闭的连接
     * @param conn_guid
     */
    virtual void OnClientClosed(const ConnGUID* conn_guid)
    {
    }

    virtual void OnRecvClientData(const ConnGUID* conn_guid, const void* data, size_t len)
    {
    }

    virtual void OnTask(const ConnGUID* conn_guid, ThreadInterface* source_thread, const void* data, size_t len)
    {
    }

protected:
    LogicCtx logic_ctx_;
    std::atomic_bool can_exit_;
    global::LogicInterface* global_logic_;
};

class CommonLogicInterface : public LogicInterface
{
public:
    virtual ~CommonLogicInterface()
    {
    }

    virtual void OnRecvClientData(const ConnGUID* conn_guid, const void* data, size_t len) final {}
    virtual void OnTask(const ConnGUID* conn_guid, ThreadInterface* source_thread, const void* data, size_t len) final {}
};
}

#endif // TCP_THREADS_INC_TCP_LOGIC_INTERFACE_H_
