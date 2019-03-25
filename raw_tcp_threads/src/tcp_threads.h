#ifndef RAW_TCP_THREADS_SRC_TCP_THREADS_H_
#define RAW_TCP_THREADS_SRC_TCP_THREADS_H_

#include "raw_tcp_threads_interface.h"

namespace tcp
{
namespace raw
{
class Threads : public ThreadsInterface
{
public:
    Threads();
    virtual ~Threads();

    ///////////////////////// ModuleInterface /////////////////////////
    const char* GetVersion() const override;
    const char* GetLastErrMsg() const override;
    void Release() override;
    int Initialize(const void* ctx) override;
    void Finalize() override;
    int Activate() override;
    void Freeze() override;

    ///////////////////////// ThreadsInterface /////////////////////////
    int CreateThreadGroup() override;
    void SetRelatedThreadGroups(const RelatedThreadGroups* related_thread_groups) override;
    ThreadGroupInterface* GetListenThreadGroup() const override;
    ThreadGroupInterface* GetTCPThreadGroup() const override;

private:
    ThreadsCtx threads_ctx_;
    ThreadGroupInterface* listen_thread_group_;
    ThreadGroupInterface* tcp_thread_group_;
    RelatedThreadGroups related_thread_groups_;
};
}
}

#endif // RAW_TCP_THREADS_SRC_TCP_THREADS_H_