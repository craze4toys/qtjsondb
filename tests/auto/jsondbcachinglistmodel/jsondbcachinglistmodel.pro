TARGET = tst_jsondbcachinglistmodel

INCLUDEPATH += ../../shared/

QT = core network testlib gui qml jsondb-private
CONFIG -= app_bundle
CONFIG += testcase

include($$PWD/../../shared/shared.pri)

DEFINES += JSONDB_DAEMON_BASE=\\\"$$QT.jsondb.bins\\\"
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += testjsondbcachinglistmodel.h \
           $$PWD/../../shared/requestwrapper.h
SOURCES += testjsondbcachinglistmodel.cpp

OTHER_FILES += \
    partitions.json

data.files = $$OTHER_FILES
data.path = $$[QT_INSTALL_TESTS]/$$TARGET
INSTALLS += data
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0
