#include "SAMdiSubWindow.h"

#include <QApplication>
#include <QCloseEvent>
#include <QDateTime>
#include <QEvent>
#include <QMenu>
#include <QMessageBox>
#include <QString>
#include <QTemporaryFile>
#include <QTextStream>

#include <fstream>
#include <string>

using std::ifstream;
using std::string;

SAMdiSubWindow::SAMdiSubWindow(QWidget* parent, Qt::WindowFlags flags)
  : QMdiSubWindow(parent, flags)
  , m_birthdate(QDateTime::currentDateTime().toString(Qt::LocalDate))
{
    setAttribute(Qt::WA_DeleteOnClose);
    m_id = intptr_t(this); //获取一个唯一id
}

void
SAMdiSubWindow::closeEvent(QCloseEvent* e)
{
    if (m_confirm_close) {
        switch (
            QMessageBox::question(this, windowTitle(), tr("Are You Sure Close Window?"))) {
        case QMessageBox::Yes:
            emit closedWindow(this);
            e->accept();
            break;
        default:
            e->ignore();
            break;
        }
    } else {
        emit closedWindow(this);
        e->accept();
    }
}

void
SAMdiSubWindow::setHidden()
{
    hide();
    emit hiddenWindow(this);
}

bool
SAMdiSubWindow::eventFilter(QObject* object, QEvent* e)
{
    if (e->type() == QEvent::ContextMenu && object == widget()) {
        emit showContextMenu();
        return true;
    }
    return QMdiSubWindow::eventFilter(object, e);
}
