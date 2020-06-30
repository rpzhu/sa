﻿#ifndef SAVECTORINTERVAL_H
#define SAVECTORINTERVAL_H

#define SA_QWT

#include "SAVectorDatas.h"

#ifdef SA_QWT

#include "qwt_samples.h"
#endif
///
/// \brief 保存区间数据的数组
///
class SALIB_EXPORT SAVectorInterval : public SAVectorDatas<QwtIntervalSample>
{
public:
    SAVectorInterval();
    SAVectorInterval(const QString & name);
    SAVectorInterval(const QString& name,const QVector<QwtIntervalSample>& datas);

    virtual ~SAVectorInterval(){}
    virtual int getType() const   {return SA::VectorInterval;}
    virtual QString getTypeName() const{return QString("interval Vector");}
    virtual int getDim() const;
    virtual int getSize(int dim=SA::Dim1) const;
    virtual QVariant getAt (const std::initializer_list<size_t>& index) const;
    virtual QString displayAt(const std::initializer_list<size_t>& index) const;
    virtual void write(QDataStream & out) const;
    virtual bool setAt(const QVariant &val, const std::initializer_list<size_t> &index);

};
Q_DECLARE_METATYPE(QwtIntervalSample)
///<QwtIntervalSample的序列化
SALIB_EXPORT QDataStream &operator<<(QDataStream & out, const QwtIntervalSample & item);
SALIB_EXPORT QDataStream &operator>>(QDataStream & in, QwtIntervalSample & item);
///<QwtInterval的序列化
SALIB_EXPORT QDataStream &operator<<(QDataStream & out, const QwtInterval & item);
SALIB_EXPORT QDataStream &operator>>(QDataStream & in, QwtInterval & item);

#ifndef QT_NO_DEBUG_STREAM
SALIB_EXPORT QDebug operator<<(QDebug debug, const QwtIntervalSample &c);
#endif

#endif // SAVECTORINTERVAL_H
