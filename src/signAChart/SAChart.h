﻿#ifndef SACHART_H
#define SACHART_H
#include "SAChartGlobals.h"
#include "SASeries.h"
#include <QColor>
#include <QFont>
#include <QVector>
#include <QRectF>
#include "qwt_date.h"
#include "qwt_plot.h"
#include "qwt_symbol.h"
#include "qwt_plot_item.h"
#include <QPainterPath>
#include <QList>
#include "qwt_series_store.h"

#include "qwt_plot_barchart.h"
#include "qwt_plot_histogram.h"
#include "qwt_plot_intervalcurve.h"
#include "qwt_plot_multi_barchart.h"
#include "qwt_plot_scaleitem.h"
#include "qwt_plot_spectrocurve.h"
#include "qwt_plot_tradingcurve.h"
#include "qwt_plot_spectrogram.h"
class QwtPlotItem;
class QwtScaleDraw;
class QwtDateScaleDraw;
class QwtPlotCurve;
class QwtPlotBarChart;
///
/// \brief 这是一个辅助类，用于绘图的辅助
///
class SA_CHART_EXPORT SAChart
{
public:
    enum Value{
        Nan = -1
    };
    //更加强制的replot，就算设置为不实时刷新也能实现重绘
    void replot(QwtPlot* chart);
    //获取当前正在显示的区域
    static QRectF getVisibleRegionRang(QwtPlot* chart);
    //获取当前正在显示的区域
    static QRectF getVisibleRegionRang(QwtPlot* chart,int xAxis,int yAxis);
////////////////////// dynamic 操作//////////////////////////////
    //动态获取item的颜色，使用dynamic_cast,需要注意效率问题
    static QColor dynamicGetItemColor(const QwtPlotItem *item,const QColor& defaultColor = Qt::black);
    //动态获取可绘图的item，使用dynamic_cast,需要注意效率问题
    static QwtPlotItemList dynamicGetPlotChartItemList(const QwtPlot* chart);
    //动态判断是否是绘图item，使用dynamic_cast,需要注意效率问题
    static bool dynamicCheckIsPlotChartItem(const QwtPlotItem *item);
    //动态获取XY series item，使用dynamic_cast,需要注意效率问题
    static QwtPlotItemList dynamicGetXYSeriesItemList(const QwtPlot* chart);
    //动态获取plot chart item的数据点数，如果不是plot chart item,返回-1，使用dynamic_cast,需要注意效率问题
    static int dynamicGetPlotChartItemDataCount(const QwtPlotItem* item);
  ////////////////////// rtti 操作//////////////////////////////
    //通过rtti获取item的颜色
    static QColor getItemColor(const QwtPlotItem *item,const QColor& defaultColor = Qt::black);
    //通过rtti获取可绘图的item，
    static QwtPlotItemList getPlotChartItemList(const QwtPlot* chart);
    //通过rtti判断是否是绘图item
    static bool checkIsPlotChartItem(const QwtPlotItem *item);
    //通过rtti获取XY series item
    static QwtPlotItemList getXYSeriesItemList(const QwtPlot* chart);
    //通过rtti判断是否是XY series item
    static bool checkIsXYSeriesItem(const QwtPlotItem *item);
    //通过rtti获取plot chart item的数据点数，如果不是plot chart item,返回-1
    static int getPlotChartItemDataCount(const QwtPlotItem* item);
    //通过rtti获取所有plot的数据范围，并做并集
    static void dataRange(const QwtPlot* chart, QwtInterval *yLeft, QwtInterval *yRight, QwtInterval *xBottom, QwtInterval *xTop);
////////////////////// 坐标变换相关操作//////////////////////////////
    //坐标轴数据互转（把坐标轴转换为另外一个坐标轴数据而保持屏幕位置不变）
    static QPointF transformValue(QwtPlot*chart,const QPointF& p,int orgXAxis,int orgYAxis,int otherXAxis,int otherYAxis);
    //坐标轴数据互转（把坐标轴转换为另外一个坐标轴数据而保持屏幕位置不变）
    static QPainterPath transformPath(QwtPlot*chart,const QPainterPath& p,int orgXAxis,int orgYAxis,int otherXAxis,int otherYAxis);
    //图中1像素在实际数据的偏移(正向)
    static QPointF calcOnePixelOffset(QwtPlot* chart,int xaxis,int yaxis);
    //屏幕坐标转到对应的绘图坐标
    static QPointF screenPointToPlotPoint(QwtPlot*chart,const QPointF& screen,int xAxis,int yAxis);

////////////////////// 坐标轴相关操作//////////////////////////////

    //是否允许显示坐标轴
    static void setAxisEnable(QwtPlot*chart, int axisID, bool b);
    //设置坐标轴的标题
    static void setAxisTitle(QwtPlot*chart,int axisID,const QString& text);
    //设置坐标轴文字的字体
    static void setAxisFont(QwtPlot*chart,int axisID,const QFont &font);
    //设置坐标轴文字的旋转
    static void setAxisLabelRotation(QwtPlot*chart,int axisID,double v);
    //设置坐标轴最小刻度
    static void setAxisScaleMin(QwtPlot*chart,int axisID,double v);
    //设置坐标轴最大刻度
    static void setAxisScaleMax(QwtPlot*chart,int axisID,double v);
    //指定坐标轴端点到窗体的距离-影响坐标轴标题的显示
    static void setAxisBorderDistStart(QwtPlot*chart,int axisID,int v);
    //指定坐标轴端点到窗体的距离-影响坐标轴标题的显示
    static void setAxisBorderDistEnd(QwtPlot*chart,int axisID,int v);
    //设置坐标轴和画板的偏移距离
    static void setAxisMargin(QwtPlot*chart,int axisID,int v);
    //设置坐标轴的间隔
    static void setAxisSpacing(QwtPlot*chart,int axisID,int v);
    //设置坐标轴文字的对齐方式
    static void setAxisLabelAlignment(QwtPlot*chart,int axisID,Qt::Alignment v);
    //设置为普通线性坐标轴
    static QwtScaleDraw* setAxisNormalScale(QwtPlot *chart,int axisID);
    //设置坐标轴为时间坐标
    static QwtDateScaleDraw* setAxisDateTimeScale(QwtPlot *chart, int axisID,const QString& format,QwtDate::IntervalType type = QwtDate::Second);
    //获取时间坐标轴，若当前不是时间坐标轴，返回nullptr
    static QwtDateScaleDraw* getAxisDateTimeScale(QwtPlot *chart, int axisID);
    //获取对应坐标轴的id,如 xTop会返回xBottom
    static int otherAxis(int axisID);
    //判断是否是x轴
    static bool isXAxis(int axisID);
    //判断是否是y轴
    static bool isYAxis(int axisID);
////////////////////// 曲线数据相关操作//////////////////////////////
    ///
    /// \brief 获取QwtSeriesStore<T>对应的值
    /// \param vec 输出的QVector<T>
    /// \param series 输入的QwtSeriesStore<T>
    ///
    template<typename T>
    static void getSeriesData(QVector<T>& vec, const QwtSeriesStore<T>* series);
    ///
    /// \brief 获取QwtSeriesStore<T>对应的值
    /// \param vec 输出的QVector<T>
    /// \param series 输入的QwtSeriesStore<T>
    /// \param startIndex 开始的索引
    /// \param endIndex 结束的索引号(包括)
    ///
    template<typename T>
    static void getSeriesData(QVector<T>& vec, const QwtSeriesStore<T>* series,int startIndex,int endIndex);

    //获取一个曲线的xy值
    static void getXYDatas(const QVector<QPointF>& xys, QVector<double>* xs, QVector<double>* ys);
    static void getXYDatas(QVector<QPointF>& xys, const QwtSeriesStore<QPointF>* cur);
    static void getXYDatas(QVector<double>* xs, QVector<double>* ys, const QwtSeriesStore<QPointF>* cur);
    static size_t getXYDatas(QVector<QPointF>& xys,QVector<int>* indexs, const QwtSeriesStore<QPointF>* cur, const QRectF& rang);
    static size_t getXYDatas(QVector<double>* xs, QVector<double>* ys,QVector<int>* indexs,const QwtSeriesStore<QPointF>* cur, const QRectF& rang);
    //对2d数据点的提取操作
    static size_t getXYDatas(QVector<QPointF>& xys, QVector<int>* indexs, const QwtSeriesStore<QPointF>* series,const QPainterPath& rang);
    static size_t getXYDatas(QVector<double>* xs, QVector<double>* ys, QVector<int>* indexs,const QwtSeriesStore<QPointF>* series,const QPainterPath& rang);
    //对3d数据提取
    static void getXYZDatas(QVector<QwtPoint3D>& xyzs, const QwtSeriesStore<QwtPoint3D>* cur);
    //获取间隔数据
    static void getIntervalSampleDatas(QVector<QwtIntervalSample>& intv,const QwtSeriesStore<QwtIntervalSample>* cur);
    //判断点是否在选择的范围内
    static bool isPointInRange(const QPainterPath &range,const QPointF& point);
    static bool isHistogramSampleInRange(const QPainterPath &selectRange,const QwtIntervalSample& val);
    static bool isIntervalCurveSampleInRange(const QPainterPath &selectRange,const QwtIntervalSample& val);
    static bool isMultiBarChartSampleInRange(const QPainterPath &selectRange,const QwtSetSample& val);
    static bool isTradingCurveSampleInRange(const QPainterPath &selectRange,const QwtOHLCSample& val);
    static bool isSpectroCurveSampleInRange(const QPainterPath &selectRange,const QwtPoint3D& val);
    //偏移
    static void offsetPointSample(QPointF& sample,const double& xoffset,const double& yoffset);
    static void offsetHistogramSample(QwtIntervalSample& sample,const double& xoffset,const double& yoffset);
    static void offsetIntervalCurveSample(QwtIntervalSample& sample,const double& xoffset,const double& yoffset);
    static void offsetMultiBarChartSample(QwtSetSample& sample,const double& xoffset,const double& yoffset);
    static void offsetTradingCurveSample(QwtOHLCSample& sample,const double& xoffset,const double& yoffset);
    static void offsetSpectroCurveSample(QwtPoint3D& sample,const double& xoffset,const double& yoffset);
    //把范围内的数据移除 返回移除的个数
    static int removeDataInRang(const QRectF &removeRang, const QVector<QPointF>& rawData, QVector<QPointF>& newData);
    static int removeDataInRang(const QPainterPath &removeRang, const QVector<QPointF>& rawData, QVector<QPointF>& newData);
    static int removeDataInRang(const QRectF& removeRang,QwtSeriesStore<QPointF>* curve);
    static int removeDataInRang(const QPainterPath& removeRang,QwtSeriesStore<QPointF>* curve);
////////////////////// 针对QwtSeriesStore<T>的设置操作//////////////////////////////
    template<typename T,typename PlotItemType>
    static void setVectorSampleData(QwtPlotItem* item,const QVector<T>& datas);
    template<typename T,typename PlotItemType>
    static void getVectorSampleData(QwtPlotItem* item,QVector<T>& datas);
    //setSample;
    static void setPlotCurveSample(QwtPlotItem* p,const QVector<QPointF>& datas);
    static void setPlotBarChartSample(QwtPlotItem* p,const QVector<QPointF>& datas);
    static void setPlotHistogramSample(QwtPlotItem* p,const QVector<QwtIntervalSample>& datas);
    static void setPlotIntervalCurveSample(QwtPlotItem* p,const QVector<QwtIntervalSample>& datas);
    static void setPlotMultiBarChartSample(QwtPlotItem* p,const QVector<QwtSetSample>& datas);
    static void setPlotTradingCurveSample(QwtPlotItem* p,const QVector<QwtOHLCSample>& datas);
    static void setPlotSpectroCurveSample(QwtPlotItem* p,const QVector<QwtPoint3D>& datas);
    //getSample
    static void getPlotCurveSample(QwtPlotItem* p,QVector<QPointF>& datas);
    static void getPlotBarChartSample(QwtPlotItem* p,QVector<QPointF>& datas);
    static void getPlotHistogramSample(QwtPlotItem* p,QVector<QwtIntervalSample>& datas);
    static void getPlotIntervalCurveSample(QwtPlotItem* p,QVector<QwtIntervalSample>& datas);
    static void getPlotMultiBarChartSample(QwtPlotItem* p,QVector<QwtSetSample>& datas);
    static void getPlotTradingCurveSample(QwtPlotItem* p,QVector<QwtOHLCSample>& datas);
    static void getPlotSpectroCurveSample(QwtPlotItem* p,QVector<QwtPoint3D>& datas);
    //

////////////////////// QwtPlotCurve 曲线相关操作//////////////////////////////


    //设置符号
    static void setCurveSymbol(QwtPlotCurve* cur,QwtSymbol::Style style,const QSize &size = QSize(8,8));
    static void setCurveLinePenStyle(QwtPlotCurve* cur,Qt::PenStyle style);
    //设置曲线的样式
    static void setCurvePenStyle(QwtPlotCurve* cur,Qt::PenStyle style);
////////////////////// QwtPlotBarChart曲线相关操作//////////////////////////////
    //获取屏幕位置离bar最近的点，类似于QwtPlotCurve::closestPoint
    static int closestPoint(const QwtPlotBarChart* bar,const QPoint &pos, double *dist );
};

template<typename T>
void SAChart::getSeriesData(QVector<T> &vec, const QwtSeriesStore<T> *series)
{
    const int size = series->dataSize();
    if(vec.size() < size)
    {
        vec.reserve(size);
    }
    for(int i=0;i<size;++i)
    {
        vec.push_back(series->sample(i));
    }
}

template<typename T>
void SAChart::getSeriesData(QVector<T> &vec, const QwtSeriesStore<T> *series, int startIndex, int endIndex)
{
    const int end = endIndex+1;
    for(int i=startIndex;i<end;++i)
    {
        vec.push_back(series->sample(i));
    }
}



template<typename T,typename PlotItemType>
void SAChart::setVectorSampleData(QwtPlotItem* item,const QVector<T>& datas)
{
    static_cast<PlotItemType*>(item)->setSamples(datas);
}

template<typename T, typename PlotItemType>
void SAChart::getVectorSampleData(QwtPlotItem *item, QVector<T> &datas)
{
    PlotItemType* c = static_cast<PlotItemType*>(item);
    const int size = c->dataSize();
    datas.reserve(size);
    for(int i=0;i<size;++i)
    {
        datas.push_back(c->sample(i));
    }
}

#endif // SACHART_H
