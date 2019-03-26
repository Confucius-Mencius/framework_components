#include "base_conn.h"
#include "log_util.h"

namespace tcp
{
namespace raw
{
BaseConn::BaseConn() : conn_guid_(), client_ip_(), data_()
{
    created_time_ = 0;
    client_port_ = 0;
    sock_fd_ = -1;
    thread_sink_ = NULL;
}

BaseConn::~BaseConn()
{
}
}
}
