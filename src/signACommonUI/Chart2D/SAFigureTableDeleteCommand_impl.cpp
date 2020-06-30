﻿#include "SAFigureTableCommands.h"
#include "SAData.h"
#include "SAAlgorithm.h"
#include "SAChart.h"

//////////////////////////////////////////////
class SAFigureTableDeleteCommandPrivate
{
public:
    SAFigureTableDeleteCommandPrivate(SAChart2D *chart, QwtPlotItem *item)
        :m_chart(chart)
        ,m_item(item)
    {

    }
    virtual ~SAFigureTableDeleteCommandPrivate()
    {

    }
    //获取chart
    SAChart2D *chart() const{return m_chart;}
    //获取item
    QwtPlotItem *item() const{return m_item;}
    virtual void redo() = 0;
    virtual void undo() = 0;
    virtual bool isValid() const = 0;
    ///
    /// \brief 获取索引表里有序不重复的行号
    /// \param input 以QVector<QPoint>组织的索引表
    /// \param index_begin 输出的有序行号迭代器起始地址，
    /// 注意，此迭代器为普通迭代器，在函数中会使用std::back_inserter自动插入，因此别传入std::back_inserter的迭代器
    ///
    template<typename Ite>
    static void getUniqueRows(const QVector<QPoint>& input,Ite index_begin)
    {
        QVector<int> rows;
        rows.resize(input.size());
        std::transform(input.begin(),input.end(),rows.begin()
                       ,[](const QPoint& p)->int
        {
            return p.y();
        });
        std::sort(rows.begin(),rows.end());
        std::unique_copy(rows.begin(),rows.end(),index_begin);
    }
private:
    SAChart2D *m_chart;
    QwtPlotItem *m_item;
};


//////////////////////////////////////////////////
///
/// \brief 适用于QwtSeriesStore<T>的接口
///
template<typename T,typename PlotItemType,typename FpSetSeriesSampleFun>
class SAFigureTableDeleteCommandPrivate_SeriesStoreItem : public SAFigureTableDeleteCommandPrivate
{
public:
    SAFigureTableDeleteCommandPrivate_SeriesStoreItem(
            SAChart2D *chart
            , QwtPlotItem *item
            , const QVector<QPoint> &deleteIndexs
            , FpSetSeriesSampleFun fpSetSeries
            );
    void init(const QVector<QPoint>& indexs);
    virtual void redo();
    virtual void undo();
    bool isValid() const;
private:
    bool m_isValid;
    QVector<T> m_deleteDatas;
    QVector<int> m_deleteIndexs;
    FpSetSeriesSampleFun m_fpSetSample;///< fun(QwtPlotItem* item,cosnt QVector<T>& series);
    PlotItemType* m_plotItem;
    int m_oldDataSize;
};

template<typename T,typename PlotItemType,typename FpSetSeriesSampleFun>
SAFigureTableDeleteCommandPrivate_SeriesStoreItem<T,PlotItemType,FpSetSeriesSampleFun>
::SAFigureTableDeleteCommandPrivate_SeriesStoreItem(
        SAChart2D *chart
        , QwtPlotItem *item
        , const QVector<QPoint> &deleteIndexs
        , FpSetSeriesSampleFun fpSetSeries
        )
    :SAFigureTableDeleteCommandPrivate(chart,item)
    ,m_isValid(false)
    ,m_fpSetSample(fpSetSeries)
    ,m_oldDataSize(0)
{
    m_plotItem = static_cast<PlotItemType*>(item);
    init(deleteIndexs);
}
template<typename T,typename PlotItemType,typename FpSetSeriesSampleFun>
void SAFigureTableDeleteCommandPrivate_SeriesStoreItem<T,PlotItemType,FpSetSeriesSampleFun>
::init(const QVector<QPoint> &indexs)
{
    m_oldDataSize = (int)(m_plotItem->dataSize());
    QVector<int> rowIndexs;
    rowIndexs.reserve(indexs.size());
    m_deleteIndexs.reserve(rowIndexs.size());
    getUniqueRows(indexs,std::back_inserter(rowIndexs));
    for(int i=0;i<rowIndexs.size();++i)
    {
        if(rowIndexs[i] < m_oldDataSize)
        {
            m_deleteIndexs.push_back(rowIndexs[i]);
        }
    }
    if(0==m_deleteIndexs.size())
    {
        return;
    }
    m_deleteDatas.reserve(m_deleteIndexs.size());
    for(int i=0;i<m_deleteIndexs.size();++i)
    {
        m_deleteDatas.push_back(m_plotItem->sample(i));
    }
    m_isValid = true;
}
template<typename T,typename PlotItemType,typename FpSetSeriesSampleFun>
void SAFigureTableDeleteCommandPrivate_SeriesStoreItem<T,PlotItemType,FpSetSeriesSampleFun>
::redo()
{
    if(!isValid())
        return;
    QVector<T> oldDatas;
    oldDatas.reserve(m_plotItem->dataSize());
    SAChart::getSeriesData(oldDatas,m_plotItem);
    if(m_deleteIndexs.size()>1)
    {
        QVector<T> newDatas;
        newDatas.reserve(m_oldDataSize-m_deleteIndexs.size());
        SA::copy_out_of_indexs(oldDatas.begin(),oldDatas.end(),m_deleteIndexs.begin(),m_deleteIndexs.end(),std::back_inserter(newDatas));
        m_fpSetSample(item(),newDatas);
    }
    else
    {
        oldDatas.remove(m_deleteIndexs[0]);
        m_fpSetSample(item(),oldDatas);
    }
}
template<typename T,typename PlotItemType,typename FpSetSeriesSampleFun>
void SAFigureTableDeleteCommandPrivate_SeriesStoreItem<T,PlotItemType,FpSetSeriesSampleFun>
::undo()
{
    if(!isValid())
        return;
    QVector<T> oldDatas,newDatas;
    oldDatas.reserve(m_plotItem->dataSize());
    SAChart::getSeriesData(oldDatas,m_plotItem);
    newDatas.reserve(m_oldDataSize);
    SA::insert_inner_indexs(m_deleteIndexs.begin(),m_deleteIndexs.end()
                             ,m_deleteDatas.begin()
                             ,oldDatas.begin(),oldDatas.end()
                             ,std::back_inserter(newDatas));
    m_fpSetSample(item(),newDatas);
}
template<typename T,typename PlotItemType,typename FpSetSeriesSampleFun>
bool SAFigureTableDeleteCommandPrivate_SeriesStoreItem<T,PlotItemType,FpSetSeriesSampleFun>
::isValid() const
{
    return m_isValid;
}

////////////////////////////////////////////////////

///
/// \brief SAFiguresTableDeleteCommand::SAFiguresTableDeleteCommand
/// \param chart
/// \param item
/// \param deleteIndexs
/// \param cmdName
/// \param parent
///
SAFigureTableDeleteCommand::SAFigureTableDeleteCommand(
        SAChart2D *chart
        , QwtPlotItem *item
        , const QVector<QPoint> &deleteIndexs
        , const QString &cmdName
        , QUndoCommand *parent)
    :SAFigureOptCommand(chart,cmdName,parent)
    ,d_ptr(nullptr)
{
    switch(item->rtti ())
    {
    case QwtPlotItem::Rtti_PlotCurve:
    {
        d_ptr = new SAFigureTableDeleteCommandPrivate_SeriesStoreItem
                <QPointF
                ,QwtPlotCurve
                ,decltype(&SAChart::setPlotCurveSample)
                >
                (chart,item,deleteIndexs,SAChart::setPlotCurveSample);
        break;
    }
    case QwtPlotItem::Rtti_PlotHistogram:
    {
        d_ptr = new SAFigureTableDeleteCommandPrivate_SeriesStoreItem
                <QwtIntervalSample
                ,QwtPlotHistogram
                ,decltype(&SAChart::setPlotHistogramSample)
                >
                (chart,item,deleteIndexs,SAChart::setPlotHistogramSample);
        break;
    }
    case QwtPlotItem::Rtti_PlotIntervalCurve:
    {
        d_ptr = new SAFigureTableDeleteCommandPrivate_SeriesStoreItem
                <QwtIntervalSample
                ,QwtPlotIntervalCurve
                ,decltype(&SAChart::setPlotIntervalCurveSample)
                >
                (chart,item,deleteIndexs,SAChart::setPlotIntervalCurveSample);
        break;
    }
    case QwtPlotItem::Rtti_PlotMultiBarChart:
    {
        d_ptr = new SAFigureTableDeleteCommandPrivate_SeriesStoreItem
                <QwtSetSample
                ,QwtPlotMultiBarChart
                ,decltype(&SAChart::setPlotMultiBarChartSample)
                >
                (chart,item,deleteIndexs,SAChart::setPlotMultiBarChartSample);
        break;
    }
    case QwtPlotItem::Rtti_PlotTradingCurve:
    {
        d_ptr = new SAFigureTableDeleteCommandPrivate_SeriesStoreItem
                <QwtOHLCSample
                ,QwtPlotTradingCurve
                ,decltype(&SAChart::setPlotTradingCurveSample)
                >
                (chart,item,deleteIndexs,SAChart::setPlotTradingCurveSample);
        break;
    }
    case QwtPlotItem::Rtti_PlotSpectroCurve:
    {
        d_ptr = new SAFigureTableDeleteCommandPrivate_SeriesStoreItem
                <QwtPoint3D
                ,QwtPlotSpectroCurve
                ,decltype(&SAChart::setPlotSpectroCurveSample)
                >
                (chart,item,deleteIndexs,SAChart::setPlotSpectroCurveSample);
        break;
    }
    default:
        break;
    }
}

SAFigureTableDeleteCommand::~SAFigureTableDeleteCommand()
{
    if(d_ptr)
        delete d_ptr;
}

void SAFigureTableDeleteCommand::redo()
{
    if(d_ptr)
        d_ptr->redo();
}

void SAFigureTableDeleteCommand::undo()
{
    if(d_ptr)
        d_ptr->undo();
}

bool SAFigureTableDeleteCommand::isValid() const
{
    if(d_ptr)
        return d_ptr->isValid();
    return false;
}


