#ifndef SADATACLIENT_H
#define SADATACLIENT_H
#include <QObject>
#include <QDateTime>
#include <QAbstractSocket>
#include "../global/SAGlobals.h"
#include "SAProtocolHeader.h"
#include "SATcpDataProcessClient.h"
class QThread;

//对于不暴露的接口，不使用impl方式

/**
 * @brief 作为客户端，负责和服务端连接
 *
 * @code
 * SADataClient m_client;
 *
 * ...
 *
 *
 * @endcode
 */
class SADataClient : public QObject
{
    Q_OBJECT
public:
    SADataClient(QObject* p=nullptr);
    ~SADataClient();

signals:
    /**
     * @brief 通知消息投递
     * @param info 消息内容
     * @param messageType 消息类型
     */
    void messageInfo(const QString& info,SA::MeaasgeType messageType = SA::NormalMessage);
    /**
     * @brief 心跳超时
     * 心跳超时是根据HEART_BREAK_COUNT_AS_DISCONNECT统计的累积超时才会发出此消息，期间有任何在链接情况都会打断此触发
     */
    void heartbeatCheckerTimerout();
    /**
     * @brief 连接服务的结果，此信号是在tryToConnectToServe后触发，tryToConnectToServe连接成功后会发射此信号
     */
    void connectedServeResult(bool state);
    /**
     * @brief 连接服务器发射的信号
     * @param timeout 超时时间
     */
    void startConnectToServe(int timeout);
public slots:
    //尝试连接服务器，此函数失败会继续重连，由于失败会继续，因此会阻塞
    Q_SLOT void tryConnectToServe(int retrycount = 5, int timeout = 5000);
    //请求2维数据的统计描述
    Q_SLOT bool request2DPointsDescribe(const QVector<QPointF>& arrs,uint key);
private slots:
    //连接成功槽
    Q_SLOT void onSocketConnected();
    //连接失败槽
    Q_SLOT void onSocketDisconnected();
    //定时心跳检测时间到达触发槽
    Q_SLOT void onHeartbeatCheckerTimerout(const QDateTime& lastdate);
    //错误发生
    Q_SLOT void onSocketErrorOccure(int socketError);
    //客户错误发生
    Q_SLOT void onClientErrorOccure(int clientError);
    //重新连接服务器
    Q_SLOT void reconnectToServe();
signals:
    /**
     * @brief 接收到2d点描述
     * @param header 通讯头
     * @param res 描述的协议
     */
    void rec2DPointsDescribe(const SAProtocolHeader &header, SAXMLProtocolParserPtr res);
private:
    SATcpDataProcessClient* m_client;
    QThread* m_thread;
    int m_maxConnectRetryCount;///< 记录重试连接的最大次数
    int m_connectRetryCount;///< 记录重试连接的次数
    int m_timeout;///< 连接服务器超时时间
};

#endif // SADATACLIENT_H
