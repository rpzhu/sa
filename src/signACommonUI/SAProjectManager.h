﻿#ifndef SAPROJECTMANAGER_H
#define SAPROJECTMANAGER_H
#include <QObject>
#include <QMap>
#include <QSet>
#include <functional>
#include "SACommonUIGlobal.h"
#include "SAUIInterface.h"
#include <functional>
#include "SAValueManager.h"
class QMdiSubWindow;
class QDomDocument;
class QDomNode;
class SAValueManager;
class SAProjectManagerPrivate;
class SAMdiSubWindow;
class SA_COMMON_UI_EXPORT SAProjectManager : public QObject
{
    Q_OBJECT
private:
    explicit SAProjectManager();
    Q_DISABLE_COPY(SAProjectManager)
public:
    /**
     * @brief 保存加载的回调函数，用于插件需要进行特殊的保存
     */
    typedef std::function<void(SAProjectManager*)> FunAction;
    /**
     * @brief 保存窗口的函数指针
     *
     * QFile* 文件指针，QWidget* 内置窗口指针，SAUIInterface* ui，QString* 错误信息
     */
    typedef std::function<bool(QFile*,QWidget*,SAUIInterface*,QString*)> SubWndSaveFun;
    typedef std::function<QWidget*(QFile*,SAUIInterface*,QString*)> SubWndLoadFun;
    ~SAProjectManager();
    //是否是可用项目
    bool isValid() const;
    //获取对象
    static SAProjectManager *getInstance();
    //获取当前项目的全称路径
    QString getProjectFullPath() const;
    //保存工程
    bool save();
    //另存工程
    bool saveAs(const QString &savePath);
    //加载工程
    bool load(const QString &projectedPath);
    //当前工程是否已变更
    bool isDirty() const;
    //设置当前工程已经变更
    void setDirty(bool isDirty);
    //工程名设置
    QString getProjectName() const;
    void setProjectName(const QString &projectName);
    //工程描述设置
    QString getProjectDescribe() const;
    void setProjectDescribe(const QString &projectDescribe);
    //获取工程的xml描述文件
    static QString getProjectDescribeFilePath(const QString &projectFolder);
    //获取工程的数据文件目录
    static QString getProjectDataFolderPath(const QString &projectFolder, bool autoMakePath= true);
    QString getProjectDataFolderPath(bool autoMakePath= true);
    //获取工程的数据文件目录
    static QString getProjectSubWindowFolderPath(const QString &projectFolder, bool autoMakePath= true);
    QString getProjectSubWindowFolderPath(bool autoMakePath= true);
    //获取工程的临时文件目录
    static QString getProjectTempFolderPath(const QString &projectFolder, bool autoMakePath= true);
    QString getProjectTempFolderPath(bool autoMakePath= true);
    //获取每个数据对应的文件路径
    //QString getDataFilePath(const SAAbstractDatas* dataPtr) const;
    //添加保存时的额外动作
    void registerExternAction(FunAction savefun,FunAction loadfun);
    //注册资窗口类名和子窗口后缀名，如果不注册，将无法保存子窗口到文件，也无法打开对应的子窗口
    void registerMdiSubWindow(const QString& className,SubWndSaveFun savefun,SubWndLoadFun loadFun);
    //建立ui的关联
    void setupUI(SAUIInterface* ui);
    SAUIInterface* ui();
private:
    static QString getProjectSubFolderPath(const QString &projectFolder,const QString& sub, bool autoMakePath= true);
    //设置项目的路径
    void setProjectFullPath(const QString &projectFullPath);
    //保存项目的描述信息
    void saveProjectInfo(const QString &projectFullPath);
    //把子窗口保存到目录下,isremoveNoneSubWndFile默认为true，就是会把当前目录下和getSubWindowList不匹配的窗口文件删除
    bool saveSubWindowToFolder(const QString &folderPath, bool isremoveNoneSubWndFile=true);
    //从文件夹中加载窗口
    int loadSubWindowFromFolder(const QString &folderPath);
    //保存子窗口
    bool saveSubWindow(SAMdiSubWindow *w,const QString& folderPath,QString* errString);
    //从文件加载子窗口
    QWidget* loadSubWindow(const QString& filepath,QString* errString);
    //加载项目信息
    bool loadProjectInfo(const QString &projectFullPath,QStringList& valuesNameList);
    //加载项目信息中的变量信息
    void loadValuesProjectInfo(QDomNode* nodeValues, QStringList& valueNames);
    //加载变量
    void loadValues(const QString &projectFullPath, const QStringList &suffixs);
    //保存变量
    void saveValues(const QString &projectFullPath);
    //移除记录的要删除的数据
    void removeNonExistDatas(const QString &projectFullPath);
    //加载一个sad文件
    SAValueManager::IDATA_PTR loadSad(const QString &filePath);
    //保存一个数据
    bool saveOneValue(const SAAbstractDatas *data, const QString &path, QString *errString);
    //写入变量的xml描述
    void writeValuesXmlInfos(QDomDocument* doc, QDomNode* root);
    //把当前文件夹下不是已经打开的子窗体的文件删除
    void removeSubWndFileNotInMainWindow(const QString &folderPath, const QList<QMdiSubWindow *> &subWindows);
signals:
    ///
    /// \brief 信息，对于一些操作的错误等内容，通过message信号发射，信息的类型通过type进行筛选
    /// \param des 信息的内容
    /// \param type 信息的类型
    ///
    void messageInformation(const QString& des,SA::MeaasgeType type);
    ///
    /// \brief 变更状态改变发射的信号
    /// \param isdirty 变更状态true时表示需要重新保存
    ///
    void dirtyStateChanged(bool isdirty);
private slots:
    //数据管理器删除数据发射信号的绑定
    void onDataRemoved(const QList<SAAbstractDatas*>& dataBeDeletedPtr);
    //数据管理器清除数据发射信号的绑定
    void onDataClear();
    //数据管理器变量名字变更的绑定
    void onDataNameChanged(SAAbstractDatas* data,const QString& oldName);
private:
    SA_IMPL(SAProjectManager)
};

#ifndef saProjectManager
#define saProjectManager SAProjectManager::getInstance()
#endif
#endif // SAPROJECTMANAGER_H
