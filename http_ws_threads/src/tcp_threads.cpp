#include "tcp_threads.h"
#include "app_frame_conf_mgr_interface.h"
#include "file_util.h"
#include "log_util.h"
#include "mem_util.h"
#include "str_util.h"
#include "version.h"

namespace tcp
{
namespace http_ws
{
Threads::Threads() : threads_ctx_(), related_thread_groups_(), raw_tcp_threads_loader_(), http_ws_logic_args_()
{
    raw_tcp_threads_ = nullptr;
}

Threads::~Threads()
{
}

const char* Threads::GetVersion() const
{
    return HTTP_WS_THREADS_HTTP_WS_THREADS_VERSION;
}

const char* Threads::GetLastErrMsg() const
{
    return nullptr;
}

void Threads::Release()
{
    SAFE_RELEASE(raw_tcp_threads_);
    delete this;
}

int Threads::Initialize(const void* ctx)
{
    if (nullptr == ctx)
    {
        return -1;
    }

    threads_ctx_ = *(static_cast<const ThreadsCtx*>(ctx));

    if (LoadRawTCPThreads() != 0)
    {
        return -1;
    }

    return 0;
}

void Threads::Finalize()
{
    SAFE_FINALIZE(raw_tcp_threads_);
}

int Threads::Activate()
{
    if (SAFE_ACTIVATE_FAILED(raw_tcp_threads_))
    {
        return -1;
    }

    return 0;
}

void Threads::Freeze()
{
    SAFE_FREEZE(raw_tcp_threads_);
}

int Threads::CreateThreadGroup(const char* name_prefix)
{
    if (raw_tcp_threads_->CreateThreadGroup(name_prefix) != 0)
    {
        return -1;
    }

    return 0;
}

void Threads::SetRelatedThreadGroups(const tcp::RelatedThreadGroups* related_thread_groups)
{
    if (nullptr == related_thread_groups)
    {
        return;
    }

    raw_tcp_threads_->SetRelatedThreadGroups(related_thread_groups);
    related_thread_groups_ = *related_thread_groups;
}

ThreadGroupInterface* Threads::GetTCPThreadGroup() const
{
    return raw_tcp_threads_->GetTCPThreadGroup();
}

int Threads::LoadRawTCPThreads()
{
    char RAW_TCP_THREADS_SO_PATH[MAX_PATH_LEN] = "";
    StrPrintf(RAW_TCP_THREADS_SO_PATH, sizeof(RAW_TCP_THREADS_SO_PATH), "%s/libraw_tcp_threads.so",
              threads_ctx_.common_component_dir);

    if (raw_tcp_threads_loader_.Load(RAW_TCP_THREADS_SO_PATH) != 0)
    {
        LOG_ERROR(raw_tcp_threads_loader_.GetLastErrMsg());
        return -1;
    }

    raw_tcp_threads_ = static_cast<tcp::raw::ThreadsInterface*>(raw_tcp_threads_loader_.GetModuleInterface());
    if (nullptr == raw_tcp_threads_)
    {
        LOG_ERROR(raw_tcp_threads_loader_.GetLastErrMsg());
        return -1;
    }

    tcp::ThreadsCtx raw_threads_ctx = threads_ctx_;
    raw_threads_ctx.conf.io_type = IO_TYPE_HTTP_WS;
    raw_threads_ctx.conf.use_bufferevent = threads_ctx_.app_frame_conf_mgr->HTTPWSUseBufferevent();
    raw_threads_ctx.conf.addr = threads_ctx_.app_frame_conf_mgr->GetHTTPWSAddr();
    raw_threads_ctx.conf.port = threads_ctx_.app_frame_conf_mgr->GetHTTPWSPort();
    raw_threads_ctx.conf.thread_count = threads_ctx_.app_frame_conf_mgr->GetHTTPWSThreadCount();
    raw_threads_ctx.conf.common_logic_so = std::string(threads_ctx_.common_component_dir) + "/libhttp_ws_raw_tcp_common_logic.so";

    http_ws_logic_args_.app_frame_conf_mgr = threads_ctx_.app_frame_conf_mgr;
    http_ws_logic_args_.related_thread_groups = &related_thread_groups_;

    raw_threads_ctx.logic_args = &http_ws_logic_args_;

    if (raw_tcp_threads_->Initialize(&raw_threads_ctx) != 0)
    {
        return -1;
    }

    return 0;
}
}
}
