!include("../Common/retroshare_plugin.pri"): error("Could not include file ../Common/retroshare_plugin.pri")

greaterThan(QT_MAJOR_VERSION, 4) {
	# Qt 5
	QT += widgets
}

INCLUDEPATH += jsoncpp

HEADERS += \
    ExportManager.h \
    ImportManager.h \
    ExportImportPlugin.h \
    ExportImportPage.h \
    jsoncpp/json/json.h \
    jsoncpp/json/json-forwards.h

HEADERS -= upnp/upnputil.h
SOURCES -= upnp/upnputil.c
SOURCES += \
    jsoncpp/jsoncpp.cpp \
    ExportManager.cpp \
    ImportManager.cpp \
    ExportImportPlugin.cpp \
    ExportImportPage.cpp

FORMS += \
    ExportImportPage.ui

RESOURCES += \
    ExportImport_images.qrc
