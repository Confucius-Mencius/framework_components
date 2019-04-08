#ifndef HTTP_WS_THREADS_SRC_RAW_TCP_COMMON_LOGIC_COMMON_LOGIC_H_
#define HTTP_WS_THREADS_SRC_RAW_TCP_COMMON_LOGIC_COMMON_LOGIC_H_

#include <vector>
#include "msg_dispatcher.h"
#include "module_loader.h"
#include "part_msg_mgr.h"
#include "http_ws_logic_args.h"
#include "http_ws_logic_interface.h"
#include "proto_msg_codec.h"
#include "raw_tcp_logic_interface.h"
#include "scheduler.h"

namespace tcp
{
namespace raw
{
struct HTTPWSLogicItem
{
    std::string logic_so_path;
    ModuleLoader logic_loader;
    tcp::http_ws::LogicInterface* logic;

    HTTPWSLogicItem() : logic_so_path(), logic_loader()
    {
        logic = nullptr;
    }
};

typedef std::vector<HTTPWSLogicItem> HTTPWSLogicItemVec;

class HTTPWSCommonLogic : public CommonLogicInterface, public TimerSinkInterface
{
public:
    HTTPWSCommonLogic();
    virtual ~HTTPWSCommonLogic();

    ///////////////////////// ModuleInterface /////////////////////////
    const char* GetVersion() const override;
    const char* GetLastErrMsg() const override;
    void Release() override;
    int Initialize(const void* ctx) override;
    void Finalize() override;
    int Activate() override;
    void Freeze() override;

    ///////////////////////// tcp::LogicInterface /////////////////////////
    void OnStop() override;
    void OnReload() override;
    void OnClientConnected(const ConnGUID* conn_guid) override;
    void OnClientClosed(const ConnGUID* conn_guid) override;
    void OnRecvClientData(const ConnGUID* conn_guid, const void* data, size_t len) override;
    void OnTask(const ConnGUID* conn_guid, ThreadInterface* source_thread, const void* data, size_t len) override;

    ///////////////////////// TimerSinkInterface /////////////////////////
    void OnTimer(TimerID timer_id, void* data, size_t len, int times);

private:
    int LoadHTTPWSCommonLogic();
    int LoadHTTPWSLogicGroup();
    void OnRecvClientMsg(const ConnGUID* conn_guid, const ::proto::MsgHead& msg_head,
                         const void* msg_body, size_t msg_body_len);

private:
    HTTPWSLogicArgs http_ws_logic_args_;

    ModuleLoader http_ws_common_logic_loader_;
    tcp::http_ws::CommonLogicInterface* http_ws_common_logic_;
    HTTPWSLogicItemVec http_ws_logic_item_vec_;

    ::proto::MsgCodec msg_codec_;
    tcp::http_ws::Scheduler scheduler_;
    tcp::http_ws::MsgDispatcher msg_dispatcher_;
    tcp::http_ws::PartMsgMgr part_msg_mgr_;
};
}
}

#endif // HTTP_WS_THREADS_SRC_RAW_TCP_COMMON_LOGIC_COMMON_LOGIC_H_