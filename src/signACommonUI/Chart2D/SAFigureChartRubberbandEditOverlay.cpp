﻿#include "SAFigureChartRubberbandEditOverlay.h"
#include "SAChart2D.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QHash>
#include <QCoreApplication>
#include "SAFigureOptCommands.h"
#include <QDebug>
struct private_widget_data
{
    SAFigureChartRubberbandEditOverlay::RectRange rectRange;

};


class SAFigureChartRubberbandEditOverlayPrivate
{
    SA_IMPL_PUBLIC(SAFigureChartRubberbandEditOverlay)
public:
    QPoint lastMouseMovePos;///< 记录最后一次窗口移动的坐标
    QBrush chart2dEditModeContorlPointBrush;///< 绘制chart2d在编辑模式下控制点的画刷
    QPen chart2dEditModeEdgetPen;///< 绘制chart2d在编辑模式下的画笔
    bool isStartResize;///< 标定开始进行缩放
    QWidget* activeWidget;///标定当前激活的窗口，如果没有就为nullptr
    int searchIndex;///< 记录搜索的索引
    QRect oldSizeRect;///< 保存旧的窗口位置，用于redo/undo
    SAFigureChartRubberbandEditOverlay::RectRange activeWidgetRectRange;///< 记录当前缩放窗口的位置情况
    SAFigureChartRubberbandEditOverlayPrivate(SAFigureChartRubberbandEditOverlay* p):q_ptr(p)
      ,lastMouseMovePos(0,0)
      ,chart2dEditModeContorlPointBrush(Qt::blue)
      ,chart2dEditModeEdgetPen(Qt::blue)
      ,isStartResize(false)
      ,activeWidget(nullptr)
    {

    }
};


SAFigureChartRubberbandEditOverlay::SAFigureChartRubberbandEditOverlay(SAFigureWindow *fig)
    :SAFigureWindowOverlay(fig)
    ,d_ptr(new SAFigureChartRubberbandEditOverlayPrivate(this))
{
   //setAttribute( Qt::WA_TransparentForMouseEvents,false);
   setFocusPolicy( Qt::ClickFocus );
   fig->installEventFilter(this);
   QList<SAChart2D*> chart = figure()->get2DPlots();
   if(!chart.isEmpty())
   {
       d_ptr->activeWidget = chart.first();
   }
   else
   {
       selectNextWidget();
   }
}

SAFigureChartRubberbandEditOverlay::~SAFigureChartRubberbandEditOverlay()
{

}


void SAFigureChartRubberbandEditOverlay::drawChartEditMode(QPainter *painter,const QRect& rect) const
{
    painter->setBrush(Qt::NoBrush);
    painter->setPen(d_ptr->chart2dEditModeEdgetPen);
    QRect edgetRect = rect.adjusted(-1,-1,1,1);
    //绘制矩形边框
    painter->drawRect(edgetRect);
    //绘制边框到figure四周
    QPen linePen(d_ptr->chart2dEditModeEdgetPen);
    linePen.setStyle(Qt::DotLine);
    painter->setPen(linePen);
    QPoint center = rect.center();
    painter->drawLine(center.x(),0,center.x(),rect.top());//top
    painter->drawLine(center.x(),rect.bottom(),center.x(),height());//bottom
    painter->drawLine(0,center.y(),rect.left(),center.y());//left
    painter->drawLine(rect.right(),center.y(),width(),center.y());//right
    //绘制四个角落
    painter->setPen(Qt::NoPen);
    painter->setBrush(d_ptr->chart2dEditModeContorlPointBrush);
    QRect connerRect(0,0,5,5);
    connerRect.moveTo(edgetRect.topLeft()-QPoint(2,2));
    painter->drawRect(connerRect);
    connerRect.moveTo(edgetRect.topRight()-QPoint(2,2));
    painter->drawRect(connerRect);
    connerRect.moveTo(edgetRect.bottomLeft()-QPoint(2,2));
    painter->drawRect(connerRect);
    connerRect.moveTo(edgetRect.bottomRight()-QPoint(2,2));
    painter->drawRect(connerRect);
}
///
/// \brief 根据范围获取图标
/// \param rr 范围
/// \return 图标
///
Qt::CursorShape SAFigureChartRubberbandEditOverlay::rectRangeToCursorShape(SAFigureChartRubberbandEditOverlay::RectRange rr)
{
    switch(rr)
    {
    case Top:
    case Bottom:
        return Qt::SizeVerCursor;
    case Left:
    case Right:
        return Qt::SizeHorCursor;
    case TopLeft:
    case BottomRight:
        return Qt::SizeFDiagCursor;
    case TopRight:
    case BottomLeft:
        return Qt::SizeBDiagCursor;
    case Inner:
        return Qt::SizeAllCursor;
    default:
        break;
    }
    return Qt::ArrowCursor;
}
///
/// \brief 根据点和矩形的关系，返回图标的样式
/// \param pos 点
/// \param region 矩形区域
/// \param err 允许误差
/// \return
///
SAFigureChartRubberbandEditOverlay::RectRange SAFigureChartRubberbandEditOverlay::getPointInRectRange(const QPoint &pos, const QRect &region, int err)
{
    if(!region.adjusted(-err,-err,err,err).contains(pos,true))
    {
        return OutSide;
    }
    if(pos.x() < (region.left()+err))
    {
        if(pos.y() < region.top()+err)
        {
            return TopLeft;
        }
        else if(pos.y() > region.bottom()-err)
        {
            return BottomLeft;
        }
        return Left;
    }
    else if(pos.x() > (region.right()-err))
    {
        if(pos.y() < region.top()+err)
        {
            return TopRight;
        }
        else if(pos.y() > region.bottom()-err)
        {
            return BottomRight;
        }
        return Right;
    }
    else if(pos.y() < (region.top()+err))
    {
        if(pos.x() < region.left()+err)
        {
            return TopLeft;
        }
        else if(pos.x() > region.right()-err)
        {
            return TopRight;
        }
        return Top;
    }
    else if(pos.y() > region.bottom()-err)
    {
        if(pos.x() < region.left()+err)
        {
            return BottomLeft;
        }
        else if(pos.x() > region.right()-err)
        {
            return BottomRight;
        }
        return Bottom;
    }
    return Inner;
}
///
/// \brief 判断点是否在矩形区域的边缘
/// \param pos 点
/// \param region 矩形区域
/// \param err 允许误差
/// \return 如果符合边缘条件，返回true
///
bool SAFigureChartRubberbandEditOverlay::isPointInRectEdget(const QPoint &pos, const QRect &region, int err)
{
    if(!region.adjusted(-err,-err,err,err).contains(pos))
    {
        return false;
    }
    if((pos.x() < (region.left()-err)) && (pos.x() < (region.left()+err)))
    {
        return true;
    }
    else if((pos.x() > (region.right()-err)) && (pos.x() < (region.right()+err)))
    {
        return true;
    }
    else if((pos.y() > (region.top()-err)) && (pos.y() < (region.top()+err)))
    {
        return true;
    }
    else if((pos.y() > region.bottom()-err) && (pos.y() < region.bottom()+err))
    {
        return true;
    }
    return false;
}

void SAFigureChartRubberbandEditOverlay::selectNextWidget(bool forward)
{
    QList<QWidget*> ws = figure()->getWidgets();
    if(ws.isEmpty())
    {
        d_ptr->activeWidget = nullptr;
        return;
    }
    if(nullptr == d_ptr->activeWidget)
    {
        d_ptr->activeWidget = ws.first();
        return;
    }
    int index = 0;
    for(int i=0;i<ws.size();++i)
    {
        if(ws[i] == d_ptr->activeWidget)
        {
            index = (forward ? i+1 : i-1);
            break;
        }
    }
    if(index == ws.size())
    {
        index = 0;
    }
    if(index<0)
    {
        index = ws.size()-1;
    }
    d_ptr->activeWidget = ws[index];
}

void SAFigureChartRubberbandEditOverlay::drawOverlay(QPainter *p) const
{
    if(d_ptr->activeWidget)
    {
        //对于激活的窗口，绘制到四周的距离提示线
        p->save();
        drawChartEditMode(p,d_ptr->activeWidget->frameGeometry());
        p->restore();
    }
}

QRegion SAFigureChartRubberbandEditOverlay::maskHint() const
{
    return figure()->rect();
}

bool SAFigureChartRubberbandEditOverlay::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type())
    {
    case QEvent::MouseButtonPress:
    {
        QMouseEvent* me = static_cast<QMouseEvent*>(event);
        return onMousePressedEvent(me);
    }
    case QEvent::MouseButtonRelease:
    {
        QMouseEvent* me = static_cast<QMouseEvent*>(event);
        return onMouseReleaseEvent(me);
    }
    case QEvent::KeyPress:
    {
        QKeyEvent* ke = static_cast<QKeyEvent*>(event);
        return onKeyPressedEvent(ke);
    }
    case QEvent::MouseMove:
    {
        QMouseEvent* e = static_cast<QMouseEvent*>(event);
        return onMouseMoveEvent(e);
    }
    case QEvent::HoverMove:
    {
        QHoverEvent* me = static_cast<QHoverEvent*>(event);
        return onMouseHoverMoveEvent(me);
    }
    default:
        break;
    }
    return QObject::eventFilter(obj, event);
}

bool SAFigureChartRubberbandEditOverlay::onMouseMoveEvent(QMouseEvent *me)
{
    if(d_ptr->activeWidget)
    {
        if(!d_ptr->isStartResize)
        {
            d_ptr->isStartResize = true;
            d_ptr->oldSizeRect = d_ptr->activeWidget->geometry();
        }
        QRect geoRect = d_ptr->activeWidget->geometry();
        switch (d_ptr->activeWidgetRectRange)
        {
        case Top:
        {
            int resultY = me->pos().y();
            geoRect.adjust(0,resultY-geoRect.top(),0,0);
            d_ptr->activeWidget->setGeometry(geoRect);
            break;
        }
        case Bottom:
        {
            int resultY = me->pos().y();
            geoRect.adjust(0,0,0,resultY - geoRect.bottom());
            d_ptr->activeWidget->setGeometry(geoRect);
            break;
        }
        case Left:
        {
            int resultX = me->pos().x();
            geoRect.adjust(resultX - geoRect.left(),0,0,0);
            d_ptr->activeWidget->setGeometry(geoRect);
            break;
        }
        case Right:
        {
            int resultX = me->pos().x();
            geoRect.adjust(0,0,resultX - geoRect.right(),0);
            d_ptr->activeWidget->setGeometry(geoRect);
            break;
        }
        case TopLeft:
        {
            geoRect.adjust(me->pos().x() - geoRect.left(),me->pos().y()-geoRect.top(),0,0);
            d_ptr->activeWidget->setGeometry(geoRect);
            break;
        }
        case TopRight:
        {
            geoRect.adjust(0,me->pos().y()-geoRect.top(),me->pos().x() - geoRect.right(),0);
            d_ptr->activeWidget->setGeometry(geoRect);
            break;
        }
        case BottomLeft:
        {
            geoRect.adjust(me->pos().x() - geoRect.left(),0,0,me->pos().y() - geoRect.bottom());
            d_ptr->activeWidget->setGeometry(geoRect);
            break;
        }
        case BottomRight:
        {
            geoRect.adjust(0,0,me->pos().x() - geoRect.right(),me->pos().y() - geoRect.bottom());
            d_ptr->activeWidget->setGeometry(geoRect);
            break;
        }
        case Inner:
        {
            QPoint offset = me->pos() - d_ptr->lastMouseMovePos;
            d_ptr->activeWidget->move(d_ptr->activeWidget->pos()+offset);
            break;
        }
        default:
            d_ptr->lastMouseMovePos = me->pos();
            return false;
        }
        d_ptr->lastMouseMovePos = me->pos();
        updateOverlay();
    }
    return true;
}

bool SAFigureChartRubberbandEditOverlay::onMouseReleaseEvent(QMouseEvent *me)
{
    if(Qt::LeftButton == me->button())
    {
        if(d_ptr->isStartResize)
        {
            d_ptr->isStartResize = false;
            if(d_ptr->activeWidget)
            {
                SAFigureSubChartResizeCommand * resizeCmd = new SAFigureSubChartResizeCommand(figure()
                                                                                ,d_ptr->activeWidget
                                                                                ,d_ptr->oldSizeRect
                                                                                ,d_ptr->activeWidget->geometry()
                                                                                ,tr("resize"));
                figure()->appendCommand(resizeCmd);
            }
        }
    }
    return true;
}

bool SAFigureChartRubberbandEditOverlay::onMousePressedEvent(QMouseEvent *me)
{
    if(Qt::LeftButton == me->button())
    {
        QList<QWidget*> ws = figure()->getWidgets();
        for(int i=0;i<ws.size();++i)
        {
            if(ws[i]->frameGeometry().contains(me->pos(),true))
            {
                //记录激活的窗口
                d_ptr->activeWidget = ws[i];
                d_ptr->searchIndex = 0;
                d_ptr->lastMouseMovePos = me->pos();
                d_ptr->isStartResize = false;
                d_ptr->activeWidgetRectRange = getPointInRectRange(me->pos(),d_ptr->activeWidget->frameGeometry(),4);
                updateOverlay();
                //找到第一个窗口就跳出
                return true;
            }
        }
    }
    return true;
}

bool SAFigureChartRubberbandEditOverlay::onMouseHoverMoveEvent(QHoverEvent *me)
{
    //d_ptr->lastMouseMovePos = me->pos();
    if(d_ptr->activeWidget)
    {
        RectRange rectRange = getPointInRectRange(me->pos(),d_ptr->activeWidget->frameGeometry(),4);
        Qt::CursorShape shape = rectRangeToCursorShape(rectRange);
        if(Qt::ArrowCursor == shape)
        {
            figure()->unsetCursor();
            return true;
        }
        figure()->setCursor(shape);
        d_ptr->activeWidgetRectRange = rectRange;
        //找到第一个窗口就跳出
        return true;

    }
    return true;
}

bool SAFigureChartRubberbandEditOverlay::onKeyPressedEvent(QKeyEvent *ke)
{
    switch(ke->key())
    {
    case Qt::Key_Enter:
        selectNextWidget(true);
        break;
    case Qt::Key_Up:
    case Qt::Key_Left:
        selectNextWidget(true);
        break;
    case Qt::Key_Right:
    case Qt::Key_Down:
        selectNextWidget(false);
        break;
    default:
        return true;
    }
    updateOverlay();
    return true;
}


