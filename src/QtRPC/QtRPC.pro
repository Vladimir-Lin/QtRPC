NAME         = QtRPC
TARGET       = $${NAME}

QT           = core
QT          -= gui
QT          += network
QT          += sql
QT          += script
QT          += Essentials
QT          += NetProtocol

load(qt_module)

INCLUDEPATH += $${PWD}/../../include/$${NAME}

HEADERS     += $${PWD}/../../include/$${NAME}/qtrpc.h

SOURCES     += $${PWD}/nRpcClient.cpp
SOURCES     += $${PWD}/nRpcServer.cpp
SOURCES     += $${PWD}/nRpcFinder.cpp
SOURCES     += $${PWD}/nRpcInterpreter.cpp

OTHER_FILES += $${PWD}/../../include/$${NAME}/headers.pri

include ($${PWD}/../../doc/Qt/Qt.pri)

TRNAME       = $${NAME}
include ($${PWD}/../../Translations.pri)
