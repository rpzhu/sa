#ifndef SAPROTOCOLHEADER_H
#define SAPROTOCOLHEADER_H
#include "SAProtocolGlobal.h"
#include <QDataStream>
#include <QDebug>
/// \def 定义魔数头
#ifndef SA_PROTOCOL_HEADER_MAGIC_START
#define SA_PROTOCOL_HEADER_MAGIC_START (0xCC880307)
#endif

/// \def 定义魔数尾
#ifndef SA_PROTOCOL_HEADER_MAGIC_END
#define SA_PROTOCOL_HEADER_MAGIC_END (0xAA900615)
#endif


/**
 * @brief sa 协议的帧头，固定长度
 * 固定36字节
 */
struct SA_PROTOCOL_EXPORT SAProtocolHeader
{
    uint32_t magic_start;///< 开始魔数，理论恒等于 \sa SA_PROTOCOL_HEADER_MAGIC_START
    int32_t sequenceID;///< 流水编号，对于多个同类型请求的区分
    int32_t protocolTypeID;///< 分类号，区分协议
    int32_t protocolFunID;///< 功能号，区分协议功能
    uint32_t dataSize;///< 标记数据包的尺寸
    uint32_t extendValue;///< 扩展值，目前无用
    uint32_t dataCrc32;///< 标记数据区的crc32值
    uint32_t magic_end;///< 结束魔数，理论恒等于 \sa SA_PROTOCOL_HEADER_MAGIC_END
    void init();
    bool isValid() const;
};
//用于判断是否是一个正确的协议头，此函数会读取p指针位置后32字节，需要确保字节有效
SA_PROTOCOL_EXPORT bool is_valid_sa_protocol_header(const char * p);
SA_PROTOCOL_EXPORT QDataStream& operator <<(QDataStream& io,const SAProtocolHeader& d);
SA_PROTOCOL_EXPORT QDataStream& operator >>(QDataStream& io,SAProtocolHeader& d);
SA_PROTOCOL_EXPORT QDebug& operator<<(QDebug& debug, const SAProtocolHeader &d);
#endif // SAPROTOCOLHEADER_H
