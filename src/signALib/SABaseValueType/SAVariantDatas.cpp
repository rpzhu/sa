﻿#include "SAVariantDatas.h"


SAVariantDatas::SAVariantDatas(const QVariant &d):SASingleDatas<QVariant>()
{
    m_d = d;
}

SAVariantDatas::SAVariantDatas(int d):SASingleDatas<QVariant>()
{
    m_d = d;
}

SAVariantDatas::SAVariantDatas(float d):SASingleDatas<QVariant>()
{
    m_d = d;
}

SAVariantDatas::SAVariantDatas(double d):SASingleDatas<QVariant>()
{
    m_d = d;
}

SAVariantDatas::SAVariantDatas(char d):SASingleDatas<QVariant>()
{
    m_d = d;
}

SAVariantDatas::SAVariantDatas(unsigned int d):SASingleDatas<QVariant>()
{
    m_d = d;
}

SAVariantDatas::SAVariantDatas(unsigned char d):SASingleDatas<QVariant>()
{
    m_d = d;
}

SAVariantDatas::SAVariantDatas(const QString &d):SASingleDatas<QVariant>()
{
    m_d = d;
}

SAVariantDatas::SAVariantDatas(const QPoint &d):SASingleDatas<QVariant>()
{
    m_d = d;
}

SAVariantDatas::SAVariantDatas(const QPointF &d):SASingleDatas<QVariant>()
{
    m_d = d;
}

SAVariantDatas::SAVariantDatas(const QHash<QString, QVariant> &d):SASingleDatas<QVariant>()
{
    m_d = d;
}

SAVariantDatas::SAVariantDatas(const QMap<QString, QVariant> &d):SASingleDatas<QVariant>()
{
    m_d = d;
}

SAVariantDatas::SAVariantDatas(const QList<QVariant> &d):SASingleDatas<QVariant>()
{
    m_d = d;
}

int SAVariantDatas::getType() const
{
    return SA::Variant;
}

QString SAVariantDatas::getTypeName() const
{
    return "var";
}

bool SAVariantDatas::isEmpty() const
{
    return m_d.isValid();
}

bool SAVariantDatas::setAt(const QVariant &val, const std::initializer_list<size_t> &index)
{
    for(auto i = index.begin();i!=index.end();++i)
    {
        if(*i != 0)
        {
            return false;
        }
    }
    m_d = val;
    setDirty(true);
    return true;
}


