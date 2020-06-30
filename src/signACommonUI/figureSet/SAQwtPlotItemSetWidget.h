﻿#ifndef SAQWTPLOTITEMSETWIDGET_H
#define SAQWTPLOTITEMSETWIDGET_H

#include <QWidget>
class QwtPlotItem;
class SACheckBoxPropertyItem;
class SALineEditPropertyItem;
class SADoubleSpinBoxPropertyItem;
class SAChartAxisSetSelect;
class QLabel;
class QHBoxLayout;
class QVBoxLayout;
class QwtPlotBarChart;
class SAQwtPlotItemAxisBindSetWidget;
///
/// \brief 用于设置QwtPlotItem的widget
///
class SAQwtPlotItemSetWidget : public QWidget
{
    Q_OBJECT
public:
    SAQwtPlotItemSetWidget(QwtPlotItem *plotItem, QWidget* parent = nullptr);
    virtual void retranslateUi();
    virtual void upDateData(bool downLoad = true);
    static SAQwtPlotItemSetWidget* createQwtPlotItemSetWidget(QwtPlotItem *plotItem, QWidget* parent = nullptr);
signals:
    void plotItemTitleChanged(const QString& title);
    void plotItemAxixBindChanged(int xOldAxis, int yOldAxis,int xNewAxis, int yNewAxis);
private slots:
    void onPlotItemAxisBindChanged(int xaxis,int yaxis);
protected:
    void addWidget(QWidget* w, int stretch = 0, Qt::Alignment alignment = Qt::Alignment());
    void addLayout(QLayout *layout, int stretch = 0);
private:
    QwtPlotItem* m_itemPtr;
    SACheckBoxPropertyItem* m_visibleItem;
    SALineEditPropertyItem* m_titleItem;
//    SADoubleSpinBoxPropertyItem* m_zValueItem;
//    SAChartAxisSetSelect* m_axisSetItem;
//    QLabel* m_labelAxisSet;
//    QHBoxLayout* m_hAxisSetlayout;
    SAQwtPlotItemAxisBindSetWidget* plotItemAxisBindSetWidget;
    QVBoxLayout* m_layout;
};

#endif // SAQWTPLOTITEMSETWIDGET_H
