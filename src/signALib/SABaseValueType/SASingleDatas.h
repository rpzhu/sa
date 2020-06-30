﻿#ifndef SASINGLEDATAS
#define SASINGLEDATAS
#include "SAAbstractDatas.h"

class SADataReference;

///
/// \brief 基本数据容器
///
template<typename DATA_TYPE>
class SASingleDatas : public SAAbstractDatas
{
public:
    SASingleDatas();
    virtual int getSize(int dim=SA::Dim1) const;
    //获取维度
    virtual int getDim() const;
    virtual QVariant getAt(const std::initializer_list<size_t>& index) const;
    virtual QString displayAt(const std::initializer_list<size_t>& index) const;
#if 0
    DATA_TYPE& innerData();
#endif
    virtual void read(QDataStream & in);
    virtual void write(QDataStream & out) const;
    //判断该数据在上次write之后是否内存有变更
    virtual bool isDirty() const;
    //设置内存有变更
    virtual void setDirty(bool dirty);
    //用于编辑-返回true设置成功，返回false设置失败，默认SAAbstractDatas返回false不接受编辑
    virtual bool setAt(const QVariant& val,const std::initializer_list<size_t>& index);

public:
    const DATA_TYPE& innerData() const;
protected:
    DATA_TYPE m_d;
    mutable bool m_isDirty;
};



#endif // SASINGLEDATAS



template<typename DATA_TYPE>
SASingleDatas<DATA_TYPE>::SASingleDatas():SAAbstractDatas()
{
    setDirty(true);
}
template<typename DATA_TYPE>
int SASingleDatas<DATA_TYPE>::getSize(int dim) const
{
    Q_UNUSED(dim);
    if(SA::Dim0 == dim || SA::Dim1== dim)
    {
        return 1;
    }
    return 0;
}

template<typename DATA_TYPE>
int SASingleDatas<DATA_TYPE>::getDim() const
{
    return SA::Dim0;
}
///
/// \brief getAt()或者getAt(0)getAt(0,0)或者getAt({0,0,0,0...,0})只要是0都可以获得数据
/// \param index
/// \return
///
template<typename DATA_TYPE>
QVariant SASingleDatas<DATA_TYPE>::getAt(const std::initializer_list<size_t> &index) const
{
    bool isZeroIndex = true;
    for(auto i=index.begin();i!=index.end();++i)
    {
        if(0!=*i)
        {
            isZeroIndex = false;
            break;
        }
    }
    if(isZeroIndex)
    {
        return QVariant::fromValue<DATA_TYPE>(m_d);
    }
    return QVariant();
}

template<typename DATA_TYPE>
QString SASingleDatas<DATA_TYPE>::displayAt(const std::initializer_list<size_t> &index) const
{
    return getAt(index).toString();
}

template<typename DATA_TYPE>
const DATA_TYPE &SASingleDatas<DATA_TYPE>::innerData() const
{
    return m_d;
}
template<typename DATA_TYPE>
void SASingleDatas<DATA_TYPE>::read(QDataStream &in)
{
    SAAbstractDatas::read(in);
    in >> m_d;
    setDirty(false);
}

template<typename DATA_TYPE>
void SASingleDatas<DATA_TYPE>::write(QDataStream &out) const
{
    SAAbstractDatas::write(out);
    out << m_d;
    m_isDirty = false;
}

template<typename DATA_TYPE>
bool SASingleDatas<DATA_TYPE>::isDirty() const
{
    return m_isDirty;
}

template<typename DATA_TYPE>
void SASingleDatas<DATA_TYPE>::setDirty(bool dirty)
{
    m_isDirty = dirty;
}

template<typename DATA_TYPE>
bool SASingleDatas<DATA_TYPE>::setAt(const QVariant &val, const std::initializer_list<size_t> &index)
{
    for(auto i = index.begin();i!=index.end();++i)
    {
        if(*i != 0)
        {
            return false;
        }
    }
    if(val.canConvert<DATA_TYPE>())
    {
        m_d = val.value<DATA_TYPE>();
        setDirty(true);
        return true;
    }
    return false;
}


#if 0
template<typename DATA_TYPE>
DATA_TYPE &SASingleDatas<DATA_TYPE>::innerData()
{
    return m_d;
}
#endif
