﻿#ifndef SASELECTREGIONDATAEDITOR_H
#define SASELECTREGIONDATAEDITOR_H
#include "SACommonUIGlobal.h"
#include "SASelectRegionEditor.h"
class SASelectRegionDataEditorPrivate;
class SA_COMMON_UI_EXPORT SASelectRegionDataEditor : public SAAbstractPlotEditor
{
    Q_OBJECT
    SA_IMPL(SASelectRegionDataEditor)
public:
    SASelectRegionDataEditor(SAChart2D *parent);
    ~SASelectRegionDataEditor();
    enum RTTI
    {
        RTTISelectDataRegionEditor = SA::RTTI_SASelectRegionDataEditor
    };
    virtual int rtti() const;
    //
    const SAChart2D* chart2D() const;
    SAChart2D* chart2D();
    //
    virtual void setEnabled( bool on );
private slots:
    void onCurrentSelectItemsChanged(const QList<QwtPlotItem*>& items);
private:
    //更新选中的索引
    void updateRegionIndex();
    //对数据进行偏移
    void offsetRegion(const QPointF& offset);
    //完成编辑
    bool completeEdit(const QPoint &screenPoint);
    //开始编辑
    void startEdit(const QPoint &screenPoint);
    //移动编辑
    void moveEdit(const QPoint &toScreenPoint);
    //完成键盘的编辑
    void completeKeyActionEdit();
protected:
    virtual bool mousePressEvent(const QMouseEvent *e);
    virtual bool mouseMovedEvent( const QMouseEvent *e);
    virtual bool mouseReleasedEvent( const QMouseEvent *e);
    virtual bool keyPressEvent(const QKeyEvent *e);
};

#endif // SASELECTREGIONDATAEDITOR_H
