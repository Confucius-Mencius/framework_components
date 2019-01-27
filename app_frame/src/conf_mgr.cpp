#include "conf_mgr.h"

namespace app_frame
{
ConfMgr::ConfMgr() : rwlock_()
{
}

ConfMgr::~ConfMgr()
{
}

int ConfMgr::Load()
{
    AUTO_THREAD_WLOCK(rwlock_);

    enable_cpu_profiling_ = false;
    enable_mem_profiling_ = false;
    release_free_mem_ = false;
    global_common_logic_so_ = "";
    global_logic_so_group_.clear();
    tcp_addr_port_ = "";
    tcp_conn_count_limit_ = 0;
    tcp_inactive_conn_check_interval_sec_ = 0;
    tcp_inactive_conn_check_interval_usec_ = 0;
    tcp_inactive_conn_life_ = 0;
    tcp_storm_interval_ = 0;
    tcp_storm_threshold_ = 0;
    tcp_thread_count_ = 0;
    tcp_common_logic_so_ = "";
    tcp_logic_so_group_.clear();
    udp_addr_port_ = "";
    udp_inactive_conn_check_interval_sec_ = 0;
    udp_inactive_conn_check_interval_usec_ = 0;
    udp_inactive_conn_life_ = 0;
    udp_thread_count_ = 0;
    udp_common_logic_so_ = "";
    udp_logic_so_group_.clear();
    work_thread_count_ = 0;
    work_common_logic_so_ = "";
    work_logic_so_group_.clear();
    io_to_work_tq_size_limit_ = 0;
    burden_thread_count_ = 0;
    burden_common_logic_so_ = "";
    burden_logic_so_group_.clear();

    if (LoadEnableCPUProfiling() != 0)
    {
        return -1;
    }

    if (LoadEnableMemProfiling() != 0)
    {
        return -1;
    }

    if (LoadReleaseFreeMem() != 0)
    {
        return -1;
    }

    if (LoadGlobalCommonLogicSo() != 0)
    {
        return -1;
    }

    if (LoadGlobalLogicSoGroup() != 0)
    {
        return -1;
    }

    if (LoadTCPAddrPort() != 0)
    {
        return -1;
    }

    if (LoadTCPConnCountLimit() != 0)
    {
        return -1;
    }

    if (LoadTCPInactiveConnCheckIntervalSec() != 0)
    {
        return -1;
    }

    if (LoadTCPInactiveConnCheckIntervalUsec() != 0)
    {
        return -1;
    }

    if (LoadTCPInactiveConnLife() != 0)
    {
        return -1;
    }

    if (LoadTCPStormInterval() != 0)
    {
        return -1;
    }

    if (LoadTCPStormThreshold() != 0)
    {
        return -1;
    }

    if (LoadTCPThreadCount() != 0)
    {
        return -1;
    }

    if (LoadTCPCommonLogicSo() != 0)
    {
        return -1;
    }

    if (LoadTCPLogicSoGroup() != 0)
    {
        return -1;
    }

    if (LoadUDPAddrPort() != 0)
    {
        return -1;
    }

    if (LoadUDPInactiveConnCheckIntervalSec() != 0)
    {
        return -1;
    }

    if (LoadUDPInactiveConnCheckIntervalUsec() != 0)
    {
        return -1;
    }

    if (LoadUDPInactiveConnLife() != 0)
    {
        return -1;
    }

    if (LoadUDPThreadCount() != 0)
    {
        return -1;
    }

    if (LoadUDPCommonLogicSo() != 0)
    {
        return -1;
    }

    if (LoadUDPLogicSoGroup() != 0)
    {
        return -1;
    }

    if (LoadWorkThreadCount() != 0)
    {
        return -1;
    }

    if (LoadWorkCommonLogicSo() != 0)
    {
        return -1;
    }

    if (LoadWorkLogicSoGroup() != 0)
    {
        return -1;
    }

    if (LoadIOToWorkTQSizeLimit() != 0)
    {
        return -1;
    }

    if (LoadBurdenThreadCount() != 0)
    {
        return -1;
    }

    if (LoadBurdenCommonLogicSo() != 0)
    {
        return -1;
    }

    if (LoadBurdenLogicSoGroup() != 0)
    {
        return -1;
    }

    return 0;
}
}
