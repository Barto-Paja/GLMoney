QT       += core gui sql charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RC_ICONS = dollar-symbol.ico

TARGET = Budzet" "Domowy

SOURCES += \
    income_and_expense/ribbon/core/addnewcategorywindow.cpp \
    data.cpp \
    homebudget/homebudget_form.cpp \
    homebudget/monthresume.cpp \
    income_and_expense/incomesandexpenses_form.cpp \
    income_and_expense/ribbon/core/newaccountwindow.cpp \
    income_and_expense/ribbon/core/newmemberwindow.cpp \
    income_and_expense/ribbon/core/newpayeewindow.cpp \
    income_and_expense/ribbon/core/newsubcategorywindow.cpp \
    income_and_expense/ribbon/core/payeeinformationwindow.cpp \
    income_and_expense/ribbon/export/exporttocsvwindow.cpp \
    income_and_expense/ribbon/import/importcsvwindow.cpp \
    income_and_expense/ribbon/utils/maketransactionsbetweenaccounts.cpp \
    income_and_expense/ribbon/report/transatcionshistorywindow.cpp \
    main.cpp \
    mainmenu.cpp \
    mainwindow.cpp

HEADERS += \
    DataStructs.h \
    income_and_expense/ribbon/core/addnewcategorywindow.hpp \
    data.hpp \
    homebudget/homebudget_form.hpp \
    homebudget/monthresume.hpp \
    income_and_expense/incomesandexpenses_form.hpp \
    income_and_expense/ribbon/core.h \
    income_and_expense/ribbon/core/newaccountwindow.hpp \
    income_and_expense/ribbon/core/newmemberwindow.hpp \
    income_and_expense/ribbon/core/newpayeewindow.hpp \
    income_and_expense/ribbon/core/newsubcategorywindow.hpp \
    income_and_expense/ribbon/core/payeeinformationwindow.hpp \
    income_and_expense/ribbon/export.h \
    income_and_expense/ribbon/export/exporttocsvwindow.hpp \
    income_and_expense/ribbon/import.h \
    income_and_expense/ribbon/import/importcsvwindow.hpp \
    income_and_expense/ribbon/report.h \
    income_and_expense/ribbon/utils.h \
    income_and_expense/ribbon/utils/maketransactionsbetweenaccounts.hpp \
    income_and_expense/ribbon/report/transatcionshistorywindow.hpp \
    mainmenu.hpp \
    mainwindow.h

FORMS += \
    income_and_expense/ribbon/core/addnewcategorywindow.ui \
    income_and_expense/ribbon/core/payeeinformationwindow.ui \
    income_and_expense/ribbon/export/exporttocsvwindow.ui \
    homebudget/homebudget_form.ui \
    homebudget/monthresume.ui \
    income_and_expense/incomesandexpenses_form.ui \
    income_and_expense/ribbon/import/importcsvwindow.ui \
    mainmenu.ui \
    mainwindow.ui \
    income_and_expense/ribbon/utils/maketransactionsbetweenaccounts.ui \
    income_and_expense/ribbon/core/newaccountwindow.ui \
    income_and_expense/ribbon/core/newmemberwindow.ui \
    income_and_expense/ribbon/core/newpayeewindow.ui \
    income_and_expense/ribbon/core/newsubcategorywindow.ui \
    income_and_expense/ribbon/report/transatcionshistorywindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    dollar-symbol.ico

RESOURCES += \
    images.qrc
