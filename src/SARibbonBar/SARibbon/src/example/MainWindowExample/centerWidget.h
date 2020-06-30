#if defined(_MSC_VER) && (_MSC_VER >= 1600)
# pragma execution_character_set("utf-8")
#endif
#ifndef CENTERWIDGET_H
#define CENTERWIDGET_H

#include "ui_centerWidget.h"

class centerWidget : public QWidget, private Ui::centerWidget
{
    Q_OBJECT

public:
    explicit centerWidget(QWidget *parent = nullptr);
};

#endif // CENTERWIDGET_H
