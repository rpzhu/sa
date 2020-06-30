﻿#ifndef SACHARTSETWIDGET_H
#define SACHARTSETWIDGET_H

#include <QWidget>
#include <QScopedPointer>
class QwtPlot;
class SAChart2D;
///
/// \brief
///
class SAChartSetWidget : public QWidget
{
    Q_OBJECT
public:
    SAChartSetWidget(QWidget* parent = nullptr);
    ~SAChartSetWidget();
    void setChart(SAChart2D *chart);
    void updateAll();
    void updatePlotItemsSet();
    void updateAxesSet();
    void updateNormalSet();
signals:
    void chartTitleChanged(QwtPlot* chart,const QString& text);
private slots:
    void onChartDelete(QObject *obj);
private:
    class UI;
    SAChartSetWidget::UI* ui;
};

#endif // SAFIGURECANVASSETWIDGET_H
