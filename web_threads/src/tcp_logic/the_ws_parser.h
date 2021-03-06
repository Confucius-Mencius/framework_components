#ifndef WEB_THREADS_SRC_TCP_LOGIC_THE_WS_PARSER_H_
#define WEB_THREADS_SRC_TCP_LOGIC_THE_WS_PARSER_H_

/*
0               1               2               3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-------+-+-------------+-------------------------------+
|F|R|R|R| opcode|M| Payload len |    Extended payload length    |
|I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
|N|V|V|V|       |S|             |   (if payload len==126/127)   |
| |1|2|3|       |K|             |                               |
+-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
|     Extended payload length continued, if payload len == 127  |
+ - - - - - - - - - - - - - - - +-------------------------------+
|                               |Masking-key, if MASK set to 1  |
+-------------------------------+-------------------------------+
| Masking-key (continued)       |          Payload Data         |
+-------------------------------- - - - - - - - - - - - - - - - +
:                     Payload Data continued ...                :
+ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
|                     Payload Data continued ...                |
+---------------------------------------------------------------+
*/

#include "conn.h"
#include "websocket_parser.h"

// only RFC6455
#include <string>

namespace tcp
{
class WebLogic;

namespace web
{
namespace http
{
class Req;
}

namespace ws
{
class Parser
{
public:
    Parser();
    ~Parser();

    int CheckUpgrade(const http::Req& http_req);
    std::string MakeHandshake();

    void SetWebLogic(tcp::WebLogic* web_logic)
    {
        web_logic_ = web_logic;
    }

    void SetConnID(ConnID conn_id)
    {
        conn_id_ = conn_id;
    }

    int Execute(const char* buffer, size_t count);

    static int OnFrameHeader(websocket_parser* parser);
    static int OnFrameBody(websocket_parser* parser, const char* at, size_t length);
    static int OnFrameEnd(websocket_parser* parser);

private:
    std::string key_;
    std::string protocol_;

    tcp::WebLogic* web_logic_;
    ConnID conn_id_;

    struct websocket_parser parser_;
    int opcode_;
    bool is_text_; // 因为存在分帧，不能简单地用opcode确定帧的类型
    bool is_binary_;
    int fin_;
    std::string body_;
    std::string payloads_;
};
}
}
}

#endif // WEB_THREADS_SRC_TCP_LOGIC_THE_WS_PARSER_H_
