#if defined(_MSC_VER) && (_MSC_VER >= 1600)
# pragma execution_character_set("utf-8")
#endif
#include "centerWidget.h"

centerWidget::centerWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
}
