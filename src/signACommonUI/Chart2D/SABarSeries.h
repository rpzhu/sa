﻿#ifndef SABARSERIES_H
#define SABARSERIES_H
#include "SACommonUIGlobal.h"
#include "SASeriesAndDataPtrMapper.h"
#include "qwt_plot_barchart.h"
class SAAbstractDatas;
class SA_COMMON_UI_EXPORT SABarSeries : public QwtPlotBarChart,public SASeriesAndDataPtrMapper
{
public:
    explicit SABarSeries(const QString &title = QString::null);
    explicit SABarSeries( const QwtText &title );
    explicit SABarSeries(SAAbstractDatas* intData,const QString &title = QString::null);
    using QwtPlotBarChart::setSamples;
    bool setSamples(SAAbstractDatas* intData);
};

#endif // SABARSERIES_H
