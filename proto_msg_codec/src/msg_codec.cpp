#include "msg_codec.h"
#include <string.h>
#include "fast_crc32.h"
#include "log_util.h"
#include "version.h"

namespace proto
{
static const int MIN_TOTAL_MSG_LEN = CHECKSUM_FIELD_LEN + MSG_HEAD_LEN;
static const int MIN_DATA_LEN = TOTAL_MSG_LEN_FIELD_LEN + MIN_TOTAL_MSG_LEN;

MsgCodec::MsgCodec() : msg_codec_ctx_()
{
}

MsgCodec::~MsgCodec()
{
}

const char* MsgCodec::GetVersion() const
{
    return PROTO_MSG_CODEC_PROTO_MSG_CODEC_VERSION;
}

const char* MsgCodec::GetLastErrMsg() const
{
    return NULL;
}

void MsgCodec::Release()
{
    delete this;
}

int MsgCodec::Initialize(const void* ctx)
{
    if (NULL == ctx)
    {
        return -1;
    }

    msg_codec_ctx_ = *((MsgCodecCtx*) ctx);
    return 0;
}

void MsgCodec::Finalize()
{
}

int MsgCodec::Activate()
{
    return 0;
}

void MsgCodec::Freeze()
{
}

//bool MsgCodec::IsWholeMsg(MsgID& err_msg_id, size_t& total_msg_len, struct evbuffer* ev_buf)
//{
//    static const size_t max_total_msg_len = MIN_TOTAL_MSG_LEN + msg_codec_ctx_.max_msg_body_len;

//    const size_t evbuf_len = evbuffer_get_length(ev_buf);
//    if (evbuf_len < MIN_DATA_LEN)
//    {
//        err_msg_id = MSG_ID_NOT_A_WHOLE_MSG;
//        return false;
//    }

//    uint32_t total_msg_len_network;
//    if (evbuffer_copyout(ev_buf, &total_msg_len_network, sizeof(total_msg_len_network)) !=
//            sizeof(total_msg_len_network))
//    {
//        const int err = errno;
//        LOG_ERROR("failed to copy out data from evbuffer, errno: " << err << ", err msg: " << strerror(err));
//        err_msg_id = MSG_ID_COPY_OUT_FAILED;
//        return false;
//    }

//    total_msg_len = ntohl(total_msg_len_network);
//    if (total_msg_len < MIN_TOTAL_MSG_LEN || total_msg_len > max_total_msg_len)
//    {
//        LOG_ERROR("invalid msg len: " << total_msg_len << ", throw away all bytes in the buf");
//        evbuffer_drain(ev_buf, evbuf_len);
//        err_msg_id = MSG_ID_INVALID_MSG_LEN;
//        return false;
//    }

//    if (evbuf_len < (TOTAL_MSG_LEN_FIELD_LEN + total_msg_len))
//    {
//        err_msg_id = MSG_ID_NOT_A_WHOLE_MSG;
//        return false;
//    }

//    LOG_DEBUG("recv a whole msg, msg len: " << total_msg_len);
//    return true;
//}

bool MsgCodec::IsWholeMsg(MsgID& err_msg_id, size_t& total_msg_len, const char* data, size_t len)
{
    static const size_t max_total_msg_len = MIN_TOTAL_MSG_LEN + msg_codec_ctx_.max_msg_body_len;

    total_msg_len = ntohl(*(int32_t*) data);
    if (total_msg_len < MIN_TOTAL_MSG_LEN || total_msg_len > max_total_msg_len)
    {
        LOG_ERROR("invalid msg len: " << total_msg_len << ", throw away all bytes in the buf");
        err_msg_id = MSG_ID_INVALID_MSG_LEN;
        return false;
    }

    if (len < (TOTAL_MSG_LEN_FIELD_LEN + total_msg_len))
    {
        err_msg_id = MSG_ID_NOT_A_WHOLE_MSG;
        return false;
    }

    LOG_DEBUG("recv a whole msg, msg len: " << total_msg_len);
    return true;
}

int MsgCodec::DecodeMsg(MsgID& err_msg_id, MsgHead* msg_head, char** msg_body, size_t& msg_body_len,
                        const char* total_msg_buf, size_t total_msg_len)
{
    uint32_t checksum_self = 0;

    if (msg_codec_ctx_.do_checksum)
    {
        const uint32_t checksum_peer = ntohl(*((uint32_t*) total_msg_buf));
        checksum_self = FastCRC32(total_msg_buf + CHECKSUM_FIELD_LEN, total_msg_len - CHECKSUM_FIELD_LEN);
        if (checksum_peer != checksum_self)
        {
            LOG_ERROR("checksum not match, self checksum: " << checksum_self << ", peer checksum: " << checksum_peer);
            err_msg_id = MSG_ID_CHECKSUM_MISMATCH;
            return -1;
        }
    }

    msg_head->passback = ntohl(*(int32_t*) (total_msg_buf + CHECKSUM_FIELD_LEN));
    msg_head->msg_id = ntohl(*(int32_t*) (total_msg_buf + CHECKSUM_FIELD_LEN + PASSBACK_FIELD_LEN));

    *msg_body = NULL;
    msg_body_len = 0;

    if (total_msg_len > MIN_TOTAL_MSG_LEN)
    {
        *msg_body = ((char*) total_msg_buf) + MIN_TOTAL_MSG_LEN;
        msg_body_len = total_msg_len - MIN_TOTAL_MSG_LEN;
    }

    LOG_DEBUG("decode msg ok, checksum: " << checksum_self << ", " << *msg_head << ", msg body len: " << msg_body_len);
    return 0;
}

int MsgCodec::EncodeMsg(char** data, size_t& len, const MsgHead& msg_head, const void* msg_body, size_t msg_body_len)
{
    *((int32_t*) (*data)) = htonl(MIN_TOTAL_MSG_LEN + msg_body_len);
    *((int32_t*) (*data + TOTAL_MSG_LEN_FIELD_LEN + CHECKSUM_FIELD_LEN)) = htonl(msg_head.passback);
    *((int32_t*) (*data + TOTAL_MSG_LEN_FIELD_LEN + CHECKSUM_FIELD_LEN + PASSBACK_FIELD_LEN)) = htonl(msg_head.msg_id);

    if (msg_body != NULL)
    {
        memcpy(*data + TOTAL_MSG_LEN_FIELD_LEN + MIN_TOTAL_MSG_LEN, msg_body, msg_body_len);
    }

    uint32_t checksum = 0;

    if (msg_codec_ctx_.do_checksum)
    {
        checksum = FastCRC32(*data + TOTAL_MSG_LEN_FIELD_LEN + CHECKSUM_FIELD_LEN,
                             MIN_TOTAL_MSG_LEN - CHECKSUM_FIELD_LEN + msg_body_len);
    }

    *((uint32_t*) (*data + TOTAL_MSG_LEN_FIELD_LEN)) = htonl(checksum);
    len = TOTAL_MSG_LEN_FIELD_LEN + MIN_TOTAL_MSG_LEN + msg_body_len;

    LOG_DEBUG("encode msg ok, checksum: " << checksum << ", " << msg_head << ", msg body len: " << msg_body_len);
    return 0;
}
}
