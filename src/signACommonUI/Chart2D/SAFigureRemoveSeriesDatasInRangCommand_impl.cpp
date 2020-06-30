﻿#include "SAFigureOptCommands.h"
#include "SAChart.h"
#include "SAAlgorithm.h"
class SAFigureRemoveSeriesDatasInRangCommandPrivate
{
public:
    SAFigureRemoveSeriesDatasInRangCommandPrivate(SAChart2D *chart, QwtPlotItem *item)
        :m_chart(chart)
        ,m_item(item)
    {

    }
    virtual ~SAFigureRemoveSeriesDatasInRangCommandPrivate(){}
    virtual void redo() = 0;
    virtual void undo() = 0;
    virtual bool isValid() const = 0;
    SAChart2D* chart() const{return m_chart;}
    QwtPlotItem* item() const{return m_item;}
private:
    SAChart2D *m_chart;
    QwtPlotItem *m_item;
};


template<typename T,typename PlotItemType,typename FpSetSeriesSampleFun,typename FpCheckValueInRange>
class SAFigureRemoveSeriesDatasInRangCommandPrivate_SeriesStoreItem : public SAFigureRemoveSeriesDatasInRangCommandPrivate
{
public:
    SAFigureRemoveSeriesDatasInRangCommandPrivate_SeriesStoreItem(
            SAChart2D *chart
            ,QwtPlotItem *item
            ,const QPainterPath &selectRange
            , FpSetSeriesSampleFun fpSetSample
            , FpCheckValueInRange fpCheckValueInRange
            );
    //~SAFigureRemoveSeriesDatasInRangCommandPrivate_SeriesStoreItem();
    void init(const QPainterPath &selectRange);
    virtual void redo();
    virtual void undo();
    virtual bool isValid() const;
private:
    bool m_isValid;
    PlotItemType* m_series;
    FpSetSeriesSampleFun m_fpSetSample;
    FpCheckValueInRange m_fpCheckValueInRange;
    int m_oldDataSize;
    QVector<T> m_inRangeDatas;
    QVector<int> m_inRangeIndexs;///< 这个索引是以旧数据为基准
};


///
/// \brief SAFigureRemoveSeriesDatasInRangCommandPrivate_SeriesStoreItem<T, PlotItemType, FpSetSeriesSampleFun, FpCheckValueInRange>::SAFigureRemoveSeriesDatasInRangCommandPrivate_SeriesStoreItem
/// \param chart
/// \param item
/// \param selectRange 曲线选择的区域
/// \param fpSetSample 设置series setSample的函数指针
/// \param fpCheckValueInRange 判断是否越界的函数指针 bool (const T&,const QPainterPath &)
///
template<typename T,typename PlotItemType,typename FpSetSeriesSampleFun,typename FpCheckValueInRange>
SAFigureRemoveSeriesDatasInRangCommandPrivate_SeriesStoreItem<T,PlotItemType,FpSetSeriesSampleFun,FpCheckValueInRange>
::SAFigureRemoveSeriesDatasInRangCommandPrivate_SeriesStoreItem(
        SAChart2D *chart
        , QwtPlotItem *item
        , const QPainterPath &selectRange
        , FpSetSeriesSampleFun fpSetSample
        , FpCheckValueInRange fpCheckValueInRange
        )
    :SAFigureRemoveSeriesDatasInRangCommandPrivate(chart,item)
    ,m_isValid(false)
    ,m_fpSetSample(fpSetSample)
    ,m_fpCheckValueInRange(fpCheckValueInRange)
    ,m_oldDataSize(-1)
{
    m_series = static_cast<PlotItemType*>(item);
    init(selectRange);
}
template<typename T,typename PlotItemType,typename FpSetSeriesSampleFun,typename FpCheckValueInRange>
void SAFigureRemoveSeriesDatasInRangCommandPrivate_SeriesStoreItem<T,PlotItemType,FpSetSeriesSampleFun,FpCheckValueInRange>
::init(const QPainterPath &selectRange)
{
    m_oldDataSize = (int)(m_series->dataSize());
    for(int i = 0;i<m_oldDataSize;++i)
    {
        const T& data = m_series->sample(i);
        if(m_fpCheckValueInRange(selectRange,data))
        {
            m_inRangeDatas.append(data);
            m_inRangeIndexs.append(i);
        }
    }
    m_isValid = (m_inRangeDatas.size() > 0);
}
template<typename T,typename PlotItemType,typename FpSetSeriesSampleFun,typename FpCheckValueInRange>
void SAFigureRemoveSeriesDatasInRangCommandPrivate_SeriesStoreItem<T,PlotItemType,FpSetSeriesSampleFun,FpCheckValueInRange>
::redo()
{
    if(!m_isValid)
        return;
    if(m_oldDataSize != (int)(m_series->dataSize()))
        return;
    QVector<T> curDatas,newDatas;
    curDatas.reserve(m_oldDataSize);
    newDatas.reserve(m_oldDataSize - m_inRangeDatas.size());
    SAChart::getSeriesData(curDatas,m_series);
    SA::copy_out_of_indexs(curDatas.begin(),curDatas.end(),m_inRangeIndexs.begin(),m_inRangeIndexs.end(),std::back_inserter(newDatas));
    m_fpSetSample(item(),newDatas);
}
template<typename T,typename PlotItemType,typename FpSetSeriesSampleFun,typename FpCheckValueInRange>
void SAFigureRemoveSeriesDatasInRangCommandPrivate_SeriesStoreItem<T,PlotItemType,FpSetSeriesSampleFun,FpCheckValueInRange>
::undo()
{
    if(!m_isValid)
        return;
    const int dataSize = (int)(m_series->dataSize());
    QVector<T> curDatas,newDatas;
    curDatas.reserve(dataSize);
    newDatas.reserve(m_oldDataSize);
    SAChart::getSeriesData(curDatas,m_series);
    SA::insert_inner_indexs(m_inRangeIndexs.begin()
                             ,m_inRangeIndexs.end()
                             ,m_inRangeDatas.begin()
                             ,curDatas.begin()
                             ,curDatas.end()
                             ,std::back_inserter(newDatas));
    m_fpSetSample(item(),newDatas);
}
template<typename T,typename PlotItemType,typename FpSetSeriesSampleFun,typename FpCheckValueInRange>
bool SAFigureRemoveSeriesDatasInRangCommandPrivate_SeriesStoreItem<T,PlotItemType,FpSetSeriesSampleFun,FpCheckValueInRange>
::isValid() const
{
    return m_isValid;
}


///
/// \brief SAFigureRemoveSeriesDatasInRangCommand::SAFigureRemoveSeriesDatasInRangCommand
/// \param chart
/// \param item
/// \param selectRange
/// \param cmdName
/// \param parent
///
SAFigureRemoveSeriesDatasInRangCommand::SAFigureRemoveSeriesDatasInRangCommand(
        SAChart2D *chart
        , QwtPlotItem *item
        , const QPainterPath &selectRange
        , const QString &cmdName
        , QUndoCommand *parent)
    :SAFigureOptCommand(chart,cmdName,parent)
    ,d_ptr(nullptr)
{
    switch(item->rtti ())
    {
    case QwtPlotItem::Rtti_PlotCurve:
    {
        d_ptr = new SAFigureRemoveSeriesDatasInRangCommandPrivate_SeriesStoreItem
                <QPointF,QwtPlotCurve
                ,decltype(&SAChart::setPlotCurveSample)
                ,decltype(&SAChart::isPointInRange)
                >(chart,item,selectRange
                  ,SAChart::setPlotCurveSample,SAChart::isPointInRange);
        break;
    }
    case QwtPlotItem::Rtti_PlotBarChart:
    {
        d_ptr = new SAFigureRemoveSeriesDatasInRangCommandPrivate_SeriesStoreItem
                <QPointF,QwtPlotBarChart
                ,decltype(&SAChart::setPlotBarChartSample)
                ,decltype(&SAChart::isPointInRange)
                >(chart,item,selectRange
                  ,SAChart::setPlotBarChartSample,SAChart::isPointInRange);
        break;
    }
    case QwtPlotItem::Rtti_PlotHistogram:
    {
        d_ptr = new SAFigureRemoveSeriesDatasInRangCommandPrivate_SeriesStoreItem
                <QwtIntervalSample,QwtPlotHistogram
                ,decltype(&SAChart::setPlotHistogramSample)
                ,decltype(&SAChart::isHistogramSampleInRange)
                >(chart,item,selectRange
                  ,SAChart::setPlotHistogramSample,SAChart::isHistogramSampleInRange);
        break;
    }
    case QwtPlotItem::Rtti_PlotIntervalCurve:
    {
        d_ptr = new SAFigureRemoveSeriesDatasInRangCommandPrivate_SeriesStoreItem
                <QwtIntervalSample,QwtPlotIntervalCurve
                ,decltype(&SAChart::setPlotIntervalCurveSample)
                ,decltype(&SAChart::isIntervalCurveSampleInRange)
                >(chart,item,selectRange
                  ,SAChart::setPlotIntervalCurveSample,SAChart::isIntervalCurveSampleInRange);
        break;
    }
    case QwtPlotItem::Rtti_PlotMultiBarChart:
    {
        d_ptr = new SAFigureRemoveSeriesDatasInRangCommandPrivate_SeriesStoreItem
                <QwtSetSample,QwtPlotMultiBarChart
                ,decltype(&SAChart::setPlotMultiBarChartSample)
                ,decltype(&SAChart::isMultiBarChartSampleInRange)
                >(chart,item,selectRange
                  ,SAChart::setPlotMultiBarChartSample,SAChart::isMultiBarChartSampleInRange);
        break;
    }
    case QwtPlotItem::Rtti_PlotTradingCurve:
    {
        d_ptr = new SAFigureRemoveSeriesDatasInRangCommandPrivate_SeriesStoreItem
                <QwtOHLCSample,QwtPlotTradingCurve
                ,decltype(&SAChart::setPlotTradingCurveSample)
                ,decltype(&SAChart::isTradingCurveSampleInRange)
                >(chart,item,selectRange
                  ,SAChart::setPlotTradingCurveSample,SAChart::isTradingCurveSampleInRange);
        break;
    }
    case QwtPlotItem::Rtti_PlotSpectroCurve:
    {
        d_ptr = new SAFigureRemoveSeriesDatasInRangCommandPrivate_SeriesStoreItem
                <QwtPoint3D,QwtPlotSpectroCurve
                ,decltype(&SAChart::setPlotSpectroCurveSample)
                ,decltype(&SAChart::isSpectroCurveSampleInRange)
                >(chart,item,selectRange
                  ,SAChart::setPlotSpectroCurveSample,SAChart::isSpectroCurveSampleInRange);
        break;
    }
    default:
        break;
    }
}

SAFigureRemoveSeriesDatasInRangCommand::~SAFigureRemoveSeriesDatasInRangCommand()
{
    if(d_ptr)
    {
        delete d_ptr;
    }
}

void SAFigureRemoveSeriesDatasInRangCommand::redo()
{
    if(d_ptr)
    {
        d_ptr->redo();
    }
}

void SAFigureRemoveSeriesDatasInRangCommand::undo()
{
    if(d_ptr)
    {
        d_ptr->undo();
    }
}

bool SAFigureRemoveSeriesDatasInRangCommand::isValid() const
{
    if(d_ptr)
    {
        return d_ptr->isValid();
    }
    return false;
}



