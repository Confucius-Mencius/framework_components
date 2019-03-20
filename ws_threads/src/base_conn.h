#ifndef WS_THREADS_SRC_BASE_CONN_H_
#define WS_THREADS_SRC_BASE_CONN_H_

#include "conn_define.h"
#include "new_conn.h"

namespace ws
{
class ThreadSink;

class BaseConn
{
public:
    BaseConn();
    virtual ~BaseConn();

    virtual void Release() = 0;
    virtual int Initialize(const void* ctx) = 0;
    virtual void Finalize() = 0;
    virtual int Activate() = 0;
    virtual void Freeze() = 0;

    virtual int Send(const void* data, size_t len) = 0;

    void SetCreatedTime(time_t t)
    {
        created_time_ = t;
    }

    void SetSockFD(int sock_fd)
    {
        sock_fd_ = sock_fd;
    }

    int GetSockFD() const
    {
        return sock_fd_;
    }

    void SetClientIP(const char* ip)
    {
        client_ip_ = ip;
    }

    const char* GetClientIP() const
    {
        return client_ip_.c_str();
    }

    void SetClientPort(unsigned short port)
    {
        client_port_ = port;
    }

    unsigned short GetClientPort() const
    {
        return client_port_;
    }

    void SetConnGUID(int io_thread_idx, ConnID conn_id)
    {
        conn_guid_.io_thread_type = IO_THREAD_TYPE_WS;
        conn_guid_.io_thread_idx = io_thread_idx;
        conn_guid_.conn_id = conn_id;
    }

    const ConnGUID* GetConnGUID() const
    {
        return &conn_guid_;
    }

    void SetThreadSink(ThreadSink* sink)
    {
        thread_sink_ = sink;
    }

    void SetConnType(ConnType conn_type)
    {
        conn_type_ = conn_type;
    }

    ConnType GetConnType() const
    {
        return conn_type_;
    }

protected:
    time_t created_time_;
    int sock_fd_;
    std::string client_ip_;
    unsigned short client_port_;
    ConnGUID conn_guid_;
    ThreadSink* thread_sink_;
    ConnType conn_type_;
};
}

#endif // WS_THREADS_SRC_BASE_CONN_H_
