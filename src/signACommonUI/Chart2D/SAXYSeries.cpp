﻿#include "SAXYSeries.h"
#include "SAAbstractDatas.h"
#include "SADataConver.h"

SAXYSeries::SAXYSeries(const QString &title):QwtPlotCurve(title)
{

}

SAXYSeries::SAXYSeries(const QwtText &title):QwtPlotCurve(title)
{

}

SAXYSeries::SAXYSeries(const QString &title, SAAbstractDatas *dataPoints):QwtPlotCurve(title)
{
    setSamples(dataPoints);
}

///
/// \brief 加载数据
/// \param dataPoints
/// \return
///
bool SAXYSeries::setSamples(SAAbstractDatas *dataPoints)
{
    QVector<QPointF> serPoints;
    if(!SADataConver::converToPointFVector(dataPoints,serPoints))
    {
        return false;
    }
    if(serPoints.size() <= 0)
    {
        return false;
    }
    clearDataPtrLink();
    insertData(dataPoints);
    QwtPlotCurve::setSamples(serPoints);
    return true;
}
///
/// \brief 加载数据
/// \param x x值
/// \param y y值
/// \return
///
bool SAXYSeries::setSamples(SAAbstractDatas *x, SAAbstractDatas *y)
{
    QVector<double> xd,yd;
    if(!SADataConver::converToDoubleVector(x,xd))
        return false;
    if(!SADataConver::converToDoubleVector(y,yd))
        return false;
    if(0 == xd.size() || 0 == yd.size())
        return false;
    clearDataPtrLink();
    insertData(x);
    insertData(y);
    QwtPlotCurve::setSamples(xd.data(),yd.data(),qMin(xd.size(),yd.size()));
    return true;
}
///
/// \brief SAXYSeries::setSamples
/// \param y
/// \param xStart
/// \param xDetal
/// \return
///
bool SAXYSeries::setSamples(SAAbstractDatas *y, double xStart, double xDetal)
{
    QVector<double> yd;
    if(!SADataConver::converToDoubleVector(y,yd))
        return false;
    if(0 == yd.size())
        return false;
    QVector<double> xd;
    for(int i=0;i<yd.size();++i)
    {
        xd.append(xStart + (i*xDetal));
    }
    clearDataPtrLink();
    insertData(y);
    QwtPlotCurve::setSamples(xd,yd);
    return true;
}





