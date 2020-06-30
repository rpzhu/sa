#include "mainwindow.h"
#include "SARibbonBar.h"
#include "SARibbonButtonGroupWidget.h"
#include "SARibbonCategory.h"
#include "SARibbonCheckBox.h"
#include "SARibbonComboBox.h"
#include "SARibbonGallery.h"
#include "SARibbonLineEdit.h"
#include "SARibbonMenu.h"
#include "SARibbonPannel.h"
#include "SARibbonQuickAccessBar.h"
#include "SARibbonToolButton.h"
#include <QAbstractButton>
#include <QAction>
#include <QDebug>
#include <QElapsedTimer>
#include <QMenu>
#include <QPushButton>
#include <QTextEdit>

#include <centerWidget.h>
//#define PRINT_COST(ElapsedTimer, LastTime, STR)                               \
//    do {                                                                      \
//        int ___TMP_INT = ElapsedTimer.elapsed();                              \
//        qDebug() << STR << ___TMP_INT - LastTime << "(" << ___TMP_INT << ")"; \
//        LastTime = ___TMP_INT;                                                \
//    } while (0)

MainWindow::MainWindow(QWidget* par)
  : SARibbonMainWindow(par)
{

    setWindowTitle(QStringLiteral("ribbon test"));
    m_edit = new QTextEdit();
    // m_edit->show();
    // setCentralWidget(m_edit);
    centerWidget* centerWid = new centerWidget(this);
    setCentralWidget(centerWid);

    SARibbonBar* ribbon = ribbonBar();
    QFont f = ribbon->font();
    f.setFamily("微软雅黑");
    ribbon->setFont(f);
    ribbon->applitionButton()->setText(QStringLiteral("File"));
    ribbon->applitionButton()->hide();
    SARibbonCategory* categoryMain = ribbon->addCategoryPage(QStringLiteral("Main"));
    createCategoryMain(categoryMain);
    SARibbonCategory* categoryOther = ribbon->addCategoryPage(QStringLiteral("Other"));
    createCategoryOther(categoryOther);
    m_contextCategory = ribbon->addContextCategory(QStringLiteral("MyContext"), Qt::red, 1);
    SARibbonCategory* contextCategoryPage1 = m_contextCategory->addCategoryPage(QStringLiteral("Page1"));
    SARibbonCategory* contextCategoryPage2 = m_contextCategory->addCategoryPage(QStringLiteral("Page1"));
    SARibbonPannel* pannel = contextCategoryPage1->addPannel("widget");
    pannel->addWidget(new centerWidget(this), 10, 20, 30, 40);
    //    ribbon->quickAccessBar()->addButton(new QAction(QIcon(":/icon/icon/chartDataManager.png"), "action1", this));
    //    ribbon->quickAccessBar()->addButton(new QAction(QIcon(":/icon/icon/figureIcon.png"), "action2", this));
    //    ribbon->quickAccessBar()->addButton(new QAction(QIcon(":/icon/icon/information.png"), "action3", this));
    //    ribbon->quickAccessBar()->addButton(new QAction(QIcon(":/icon/icon/inRangDataRemove.png"), "action4", this));
    showMaximized();
    onShowContextCategory(true);
}

void
MainWindow::onShowContextCategory(bool on)
{
    ribbonBar()->setContextCategoryVisible(m_contextCategory, on);
}

void
MainWindow::onWpsStyle(bool on)
{
    if (on)
        ribbonBar()->setRibbonStyle(SARibbonBar::WpsLiteStyle);
    else
        ribbonBar()->setRibbonStyle(SARibbonBar::OfficeStyle);
    update();
}

void
MainWindow::createCategoryMain(SARibbonCategory* page)
{
    SARibbonToolButton* btn;
    SARibbonMenu* menu = new SARibbonMenu(this);
    menu->addAction(QIcon(":/icon/icon/folder.png"), QStringLiteral("1"));
    menu->addAction(QIcon(":/icon/icon/folder.png"), QStringLiteral("2"));
    menu->addAction(QIcon(":/icon/icon/folder.png"), QStringLiteral("3"));
    menu->addAction(QIcon(":/icon/icon/folder.png"), QStringLiteral("4"));
    menu->addAction(QIcon(":/icon/icon/folder.png"), QStringLiteral("5"));
    //    menu->addAction(QIcon(":/icon/icon/folder.png"), QStringLiteral("6"));
    //    menu->addAction(QIcon(":/icon/icon/folder.png"), QStringLiteral("7"));
    //    menu->addAction(QIcon(":/icon/icon/folder.png"), QStringLiteral("8"));
    //    menu->addAction(QIcon(":/icon/icon/folder.png"), QStringLiteral("9"));
    //    menu->addAction(QIcon(":/icon/icon/folder.png"), QStringLiteral("10"));
    SARibbonPannel* pannel = page->addPannel(QStringLiteral("Panel 1"));

    QAction* act = new QAction(this);
    act->setIcon(QIcon(":/icon/icon/save.png"));
    act->setText(QStringLiteral("SAVE"));
    pannel->addLargeAction(act); //大action
    connect(act, &QAction::triggered, this, [this](bool b) {
        this->m_edit->append("action clicked");
    });

    act = new QAction(this);
    act->setIcon(QIcon(":/icon/icon/save.png"));
    act->setText(QStringLiteral("hide ribbon"));
    act->setCheckable(true);     //设置有两种模态
    pannel->addSmallAction(act); //小action
    connect(act, &QAction::triggered, this, [this](bool b) {
        this->ribbonBar()->setHideMode(b);
    });

    act = new QAction(this);
    act->setIcon(QIcon(":/icon/icon/save.png"));
    act->setText(QStringLiteral("show hide button"));
    act->setCheckable(true);
    pannel->addSmallAction(act);
    connect(act, &QAction::triggered, this, [this](bool b) {
        this->ribbonBar()->showHideModeButton(b);
    });

    act = new QAction(this);
    act->setIcon(QIcon(":/icon/icon/filter.png"));
    act->setText(QStringLiteral("use wps style"));
    act->setCheckable(true);
    act->setChecked(false);
    btn = pannel->addSmallAction(act);
    connect(act, SIGNAL(triggered(bool)), this, SLOT(onWpsStyle(bool)));

    act = new QAction(this);
    act->setIcon(QIcon(":/icon/icon/folder.png"));
    act->setText(QStringLiteral("test 3"));
    act->setMenu(menu);
    btn = pannel->addSmallAction(act);
    btn->setPopupMode(QToolButton::MenuButtonPopup);

    act = new QAction(this);
    act->setIcon(QIcon(":/icon/icon/folder.png"));
    act->setText(QStringLiteral("test 4"));
    act->setMenu(menu);
    btn = pannel->addSmallAction(act);
    btn->setPopupMode(QToolButton::InstantPopup);

    pannel->addSeparator();

    act = new QAction(this);
    act->setIcon(QIcon(":/icon/icon/folder.png"));
    act->setText(QStringLiteral("DelayedPopup"));
    act->setMenu(menu);
    btn = pannel->addLargeAction(act);
    btn->setPopupMode(QToolButton::DelayedPopup);
    btn->setObjectName("SA_DelayedPopup");
    connect(act, &QAction::triggered, this, &MainWindow::onDelayedPopupCheckabletriggered);

    act = new QAction(this);
    act->setIcon(QIcon(":/icon/icon/folder.png"));
    act->setText(QStringLiteral("MenuButtonPopup"));
    act->setMenu(menu);
    btn = pannel->addLargeAction(act);
    btn->setObjectName(QStringLiteral("MenuButtonPopup"));
    btn->setPopupMode(QToolButton::MenuButtonPopup);
    connect(act, &QAction::triggered, this, &MainWindow::onMenuButtonPopupCheckabletriggered);

    act = new QAction(this);
    act->setIcon(QIcon(":/icon/icon/Graph-add.png"));
    act->setText(QStringLiteral("InstantPopup"));
    act->setMenu(menu);
    btn = pannel->addLargeAction(act);
    btn->setObjectName("buttonInstantPopup");
    btn->setPopupMode(QToolButton::InstantPopup);
    connect(act, &QAction::triggered, this, &MainWindow::onInstantPopupCheckabletriggered);

    act = new QAction(this);
    act->setCheckable(true);
    act->setIcon(QIcon(":/icon/icon/folder.png"));
    act->setText(QStringLiteral("DelayedPopup"));
    act->setMenu(menu);
    btn = pannel->addLargeAction(act);
    btn->setPopupMode(QToolButton::DelayedPopup);
    btn->setCheckable(true);
    connect(act, &QAction::triggered, this, &MainWindow::onDelayedPopupCheckableTest);

    act = new QAction(this);
    act->setCheckable(true);
    act->setIcon(QIcon(":/icon/icon/folder.png"));
    act->setText(QStringLiteral("MenuButtonPopup\n checkable"));
    act->setMenu(menu);
    btn = pannel->addLargeAction(act);
    btn->setPopupMode(QToolButton::MenuButtonPopup);
    btn->setCheckable(true);
    connect(act, &QAction::triggered, this, &MainWindow::onMenuButtonPopupCheckableTest);

    act = new QAction(this);
    act->setCheckable(true);
    act->setIcon(QIcon(":/icon/icon/Graph-add.png"));
    act->setText(QStringLiteral("InstantPopup"));
    act->setMenu(menu);
    btn = pannel->addLargeAction(act);
    btn->setCheckable(true);
    btn->setPopupMode(QToolButton::InstantPopup);
    connect(act, &QAction::triggered, this, &MainWindow::onInstantPopupCheckableTest);

    SARibbonPannel* panne2 = page->addPannel(QStringLiteral("pannel 2"));
    act = new QAction(this);
    act->setCheckable(true);
    act->setIcon(QIcon(":/icon/icon/Graph-add.png"));
    act->setText(QStringLiteral("show\nContext"));
    btn = panne2->addLargeAction(act);
    btn->setCheckable(true);
    //btn->setPopupMode(QToolButton::InstantPopup);
    connect(act, &QAction::triggered, this, [this](bool on) {
        if (on)
            this->ribbonBar()->showContextCategory(this->m_contextCategory);
        else
            this->ribbonBar()->hideContextCategory(this->m_contextCategory);
    });

    act = new QAction(this);
    act->setCheckable(true);
    act->setIcon(QIcon(":/icon/icon/Graph-add.png"));
    act->setText(QStringLiteral("unactive"));
    act->setMenu(menu);
    btn = panne2->addLargeAction(act);
    btn->setCheckable(true);
    btn->setPopupMode(QToolButton::InstantPopup);
    btn->setEnabled(false);

    SARibbonComboBox* com = new SARibbonComboBox(this);
    com->setSizePolicy(QSizePolicy::Expanding,
                       QSizePolicy::Fixed);
    com->setWindowIcon(QIcon(":/icon/icon/Graph-add.png"));
    com->setWindowTitle("ComboBox");
    for (int i = 0; i < 40; ++i)
        com->addItem("SARibbonComboBox test");
    com->setEditable(true);
    pannel->addWidget(com);

    com = new SARibbonComboBox(this);
    com->setSizePolicy(QSizePolicy::Expanding,
                       QSizePolicy::Fixed);
    com->setWindowIcon(QIcon(":/icon/icon/folder.png"));
    com->setWindowTitle("ComboBox Editable");
    for (int i = 0; i < 40; ++i)
        com->addItem("a");
    pannel->addWidget(com);

    SARibbonLineEdit* lineEdit = new SARibbonLineEdit(this);
    lineEdit->setSizePolicy(QSizePolicy::Expanding,
                            QSizePolicy::Fixed);
    lineEdit->setWindowIcon(QIcon(":/icon/icon/folder.png"));
    lineEdit->setWindowTitle("Line Edit");
    lineEdit->setText("SARibbonLineEdit");
    pannel->addWidget(lineEdit);
    QWidget* w = lineEdit->parentWidget();
    while (w) {
        qDebug() << w->metaObject()->className();
        w = w->parentWidget();
    }
    SARibbonCheckBox* checkBox = new SARibbonCheckBox(this);
    checkBox->setSizePolicy(QSizePolicy::Expanding,
                            QSizePolicy::Fixed);
    checkBox->setWindowIcon(QIcon(":/icon/icon/folder.png"));
    checkBox->setText("checkBox");
    pannel->addWidget(checkBox);

    pannel->setExpanding();
    QAction* optAct = new QAction(this);
    pannel->addOptionAction(optAct);
}

void
MainWindow::createCategoryOther(SARibbonCategory* page)
{
    SARibbonMenu* menu = new SARibbonMenu(this);
    QAction* item = menu->addAction(QIcon(":/icon/icon/folder.png"), QStringLiteral("b"));
    menu->addAction(QIcon(":/icon/icon/folder.png"), QStringLiteral("b"));
    menu->addAction(QIcon(":/icon/icon/folder.png"), QStringLiteral("b"));
    menu->addAction(QIcon(":/icon/icon/folder.png"), QStringLiteral("b"));
    menu->addAction(QIcon(":/icon/icon/folder.png"), QStringLiteral("b"));
    menu->addAction(QIcon(":/icon/icon/folder.png"), QStringLiteral("b"));
    menu->addAction(QIcon(":/icon/icon/folder.png"), QStringLiteral("b"));
    menu->addAction(QIcon(":/icon/icon/folder.png"), QStringLiteral("b"));
    menu->addAction(QIcon(":/icon/icon/folder.png"), QStringLiteral("b"));
    menu->addAction(QIcon(":/icon/icon/folder.png"), QStringLiteral("b"));

    SARibbonPannel* pannel = page->addPannel(QStringLiteral("pannel 1"));
    SARibbonButtonGroupWidget* btnGroup = new SARibbonButtonGroupWidget(pannel);
    btnGroup->setFrameShape(QFrame::Box);
    btnGroup->addButton(new QAction(QIcon(":/icon/icon/figureIcon.png"), "", this));
    btnGroup->addButton(new QAction(QIcon(":/icon/icon/information.png"), "", this));
    pannel->addWidget(btnGroup, 0, 3);

    SARibbonToolButton* btn;
    btn = pannel->addLargeAction(item);
    btn->setIcon(QIcon(":/icon/icon/folder.png"));
    btn->setText(QStringLiteral("un format\nicon"));
    btn->setPopupMode(QToolButton::DelayedPopup);
    btn->setFixedHeight(78);
    btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    btn->setMenu(menu);
    pannel->addWidget(btn);

    btn = pannel->addLargeAction(item);
    btn->setIcon(QIcon(":/icon/icon/folder.png"));
    btn->setText(QStringLiteral("change page\ntest"));
    btn->setPopupMode(QToolButton::MenuButtonPopup);
    btn->setFixedHeight(78);
    btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    btn->setMenu(menu);
    pannel->addWidget(btn);

    btn = pannel->addLargeAction(item);
    btn->setIcon(QIcon(":/icon/icon/folder.png"));
    btn->setText(QStringLiteral("LargeBtn"));
    btn->setPopupMode(QToolButton::InstantPopup);
    btn->setFixedHeight(78);
    btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    btn->setMenu(menu);
    pannel->addWidget(btn);

    SARibbonGallery* gallery = pannel->addGallery();
    SARibbonGalleryGroup* group = gallery->addGalleryGroup();
    for (int i = 0; i < 100; ++i) {
        group->addItem(QIcon(":/icon/icon/folder.png"));
    }
    QAction* optAct = new QAction(this);
    pannel->addOptionAction(optAct);
    pannel->setObjectName("debug");
}

void
MainWindow::onMenuButtonPopupCheckableTest(bool b)
{
    m_edit->append(QStringLiteral("MenuButtonPopupCheckableTest : %1").arg(b));
}

void
MainWindow::onInstantPopupCheckableTest(bool b)
{
    m_edit->append(QStringLiteral("InstantPopupCheckableTest : %1").arg(b));
}

void
MainWindow::onDelayedPopupCheckableTest(bool b)
{
    m_edit->append(QStringLiteral("DelayedPopupCheckableTest : %1").arg(b));
}

void
MainWindow::onMenuButtonPopupCheckabletriggered(bool b)
{
    m_edit->append(QStringLiteral("MenuButtonPopupCheckabletriggered : %1").arg(b));
}

void
MainWindow::onInstantPopupCheckabletriggered(bool b)
{
    m_edit->append(QStringLiteral("InstantPopupCheckabletriggered : %1").arg(b));
}

void
MainWindow::onDelayedPopupCheckabletriggered(bool b)
{
    m_edit->append(QStringLiteral("DelayedPopupCheckabletriggered : %1").arg(b));
}
