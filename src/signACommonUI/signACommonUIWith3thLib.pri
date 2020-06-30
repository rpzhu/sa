INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
INCLUDEPATH += $$PWD/Chart2D
DEPENDPATH += $$PWD/Chart2D

CONFIG(debug, debug|release){
    LIBS += -L$$PWD/../bin_qt$$[QT_VERSION]_debug/libs/ -lsignACommonUI
}else {
    LIBS += -L$$PWD/../bin_qt$$[QT_VERSION]_release/libs/ -lsignACommonUI
}
include($$PWD/../signAChart/signAChart.pri)

# user need add 
include($$PWD/../3rdParty/qwt/qwt_set.pri)

# 需要在pro中添加
#include($$[QT_INSTALL_PREFIX]/../Src/qttools/src/shared/qtpropertybrowser/qtpropertybrowser.pri)
