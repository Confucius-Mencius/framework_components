#ifndef GLOBAL_THREADS_SRC_WORK_LOGIC_GLOBAL_LOGIC_H_
#define GLOBAL_THREADS_SRC_WORK_LOGIC_GLOBAL_LOGIC_H_

#include "global_logic_args.h"
#include "global_logic_interface.h"
#include "module_loader.h"
#include "msg_dispatcher.h"
#include "scheduler.h"
#include "timer_axis_interface.h"
#include "work_logic_interface.h"

namespace work
{
class GlobalLogic : public LogicInterface, public TimerSinkInterface
{
public:
    GlobalLogic();
    virtual ~GlobalLogic();

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
    void OnTask(const ConnGUID* conn_guid, ThreadInterface* source_thread, const void* data, size_t len) override;

    ///////////////////////// TimerSinkInterface /////////////////////////
    void OnTimer(TimerID timer_id, void* data, size_t len, int times);

private:
    int LoadGlobalLogic();

private:
    GlobalLogicArgs global_logic_args_;

    ModuleLoader global_logic_loader_;
    global::LogicInterface* global_logic_;

    global::Scheduler scheduler_;
    global::MsgDispatcher msg_dispatcher_;
};
}

#endif // GLOBAL_THREADS_SRC_WORK_LOGIC_GLOBAL_LOGIC_H_
