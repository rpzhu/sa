#ifndef SATCPDATAPROCESSCLIENT_H
#define SATCPDATAPROCESSCLIENT_H
#include "SAServeGlobal.h"
#include "SATcpClient.h"
#include "SAXMLProtocolParser.h"
#include <memory>
#include <SAItem.h>

SA_IMPL_FORWARD_DECL(SATcpDataProcessClient)

/**
 * @brief 用于数据处理的客户端
 */
class SASERVE_EXPORT SATcpDataProcessClient : public SATcpClient
{
    Q_OBJECT
    SA_IMPL(SATcpDataProcessClient)
public:
    SATcpDataProcessClient(QObject* par = nullptr);
    ~SATcpDataProcessClient();
    static QVariant vectorpointsToVariant(const QVector<QPointF> &arrs);
    static QVector<QPointF> variantToVectorpoints(const QVariant& var);
protected:
    virtual bool dealXmlProtocol(const SAProtocolHeader &header, SAXMLProtocolParserPtr xml) override;
private:
    bool dealReply2DPointsDescribe(const SAProtocolHeader &header,SAXMLProtocolParserPtr xml);
public slots:
    //请求2维数据的统计描述
    bool request2DPointsDescribe(const QVector<QPointF>& arrs,uint key,int sortcount = 20);
signals:
    /**
     * @brief 返回数据点描述结果
     * @param res 结果以satree来进行描述
     */
    void reply2DPointsDescribe(const SAProtocolHeader &header, SAXMLProtocolParserPtr res);
};

#endif // SADATAPROCESSCLIENT_H
