#ifndef PROTO_TCP_THREADS_SRC_RAW_TCP_COMMON_LOGIC_COMMON_LOGIC_H_
#define PROTO_TCP_THREADS_SRC_RAW_TCP_COMMON_LOGIC_COMMON_LOGIC_H_

#include <vector>
#include "module_loader.h"
#include "proto_args.h"
#include "proto_tcp_logic_interface.h"
#include "raw_tcp_logic_interface.h"

namespace tcp
{
namespace raw
{
struct LogicItem
{
    std::string logic_so_path;
    ModuleLoader logic_loader;
    tcp::proto::LogicInterface* logic;

    LogicItem() : logic_so_path(), logic_loader()
    {
        logic = NULL;
    }
};

typedef std::vector<LogicItem> LogicItemVec;

class ProtoCommonLogic : public CommonLogicInterface
{
public:
    ProtoCommonLogic();
    virtual ~ProtoCommonLogic();

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

private:
    int LoadProtoTCPCommonLogic();
    int LoadProtoTCPLogicGroup();

private:
    ProtoArgs proto_args_;

    ModuleLoader proto_tcp_common_logic_loader_;
    tcp::proto::CommonLogicInterface* proto_tcp_common_logic_;
    LogicItemVec proto_tcp_logic_item_vec_;

//    struct ConnRecvCtx
//    {
//        char total_msg_len_network_[TOTAL_MSG_LEN_FIELD_LEN];
//        ssize_t total_msg_len_network_recved_len_;
//        int32_t total_msg_len_;
//        ssize_t total_msg_recved_len_;
//        char* msg_recv_buf_;

//        ConnRecvCtx()
//        {
//            total_msg_len_network_recved_len_ = 0;
//            total_msg_len_ = 0;
//            total_msg_recved_len_ = 0;
//            msg_recv_buf_ = NULL;
//        }
//    };

//    typedef __hash_map<int, ConnRecvCtx> ConnRecvCtxHashTable;
//    ConnRecvCtxHashTable conn_recv_ctx_hash_table_;
};
}
}

#endif // PROTO_TCP_THREADS_SRC_RAW_TCP_COMMON_LOGIC_COMMON_LOGIC_H_
