#ifndef APP_FRAME_INC_TASK_TYPE_H_
#define APP_FRAME_INC_TASK_TYPE_H_

enum TaskType
{
    TASK_TYPE_MIN = 0,
    TASK_TYPE_TCP_CONN_CONNECTED = TASK_TYPE_MIN,
    TASK_TYPE_TCP_CONN_CLOSED,
    TASK_TYPE_TCP_SEND_TO_CLIENT,
    TASK_TYPE_TCP_CLOSE_CONN,
    TASK_TYPE_UDP_SEND_TO_CLIENT,
    TASK_TYPE_UDP_CLOSE_CONN,

    TASK_TYPE_GLOBAL_RETURN_TO_HTTP,
    TASK_TYPE_WORK_RETURN_TO_HTTP,
    TASK_TYPE_BURDEN_RETURN_TO_HTTP,

    TASK_TYPE_NORMAL,
    TASK_TYPE_MAX,
};

#endif // APP_FRAME_INC_TASK_TYPE_H_
