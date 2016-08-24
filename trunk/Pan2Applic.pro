# Pan2Applic - a tool to convert and compile PANGAEA output files
# Rainer Sieger
# Alfred Wegener Institute, Bremerhaven, Germany
# last change: 2014-10-04

macx {
    # creating cache file
    cache()

    # Set SDK
    QMAKE_MAC_SDK = macosx10.11

    # Only Intel binaries are accepted so force this
    CONFIG += x86_64

    # Minimum OS X version for submission is 10.7
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.7

    # Icon by Rainer Sieger, 2012-07-02
    ICON = ./Resources/icon/Application.icns

    # Replace default Info.plist
    QMAKE_INFO_PLIST = ./Resources/Info.plist

    QMAKE_CFLAGS += -gdwarf-2
    QMAKE_CXXFLAGS += -gdwarf-2
    QMAKE_CFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
    QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO
    QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO
}

win32 {
    # OpenSSL
    LIBS += -LC:/Qt/OpenSSL-Win32/bin -lubsec
    INCLUDEPATH += C:/Qt/OpenSSL-Win32/include

    # Icon
    RC_FILE = ./Resources/Application.rc
}

linux-g++ {
}

TARGET   = Pan2Applic
TEMPLATE = app

QT += widgets network

INCLUDEPATH += ./Headers

HEADERS = ./Headers/Application.h \
    ./Headers/Globals.h \
    ./Headers/Webfile.h \
    ./Headers/shapefil.h \
    ./Forms/AboutDialog/AboutDialog.h \
    ./Forms/PanGetDialog/PanGetDialog.h \
    ./Forms/SetGeocodeRangeDialog/SetGeocodeRangeFloatDialog.h \
    ./Forms/SetGeocodeRangeDialog/SetGeocodeRangeDateTimeDialog.h \
    ./Forms/SetGearIDDialog/SetGearIDDialog.h \
    ./Forms/GoogleEarthOptionsDialog/GoogleEarthOptionsDialog.h \
    ./Forms/OceanDataViewOptionsDialog/OceanDataViewOptionsDialog.h \
    ./Forms/ShapeFileOptionsDialog/ShapeFileOptionsDialog.h \
    ./Forms/formatedTextOptionsDialog/formatedTextOptionsDialog.h \
    ./Forms/unformatedTextOptionsDialog/unformatedTextOptionsDialog.h \
    ./Forms/SelectParametersDialog/selectParametersDialog.h \
    ./Forms/SelectEnvironmentDialog/selectEnvironmentDialog.h \
    ./Forms/ExifToolOptionsDialog/ExifToolOptionsDialog.h

SOURCES = ./Sources/ApplicationInit.cpp \
    ./Sources/ApplicationMainWindow.cpp \
    ./Sources/ApplicationErrors.cpp \
    ./Sources/ApplicationPreferences.cpp \
    ./Sources/ApplicationCreateMenu.cpp \
    ./Sources/createGoogleEarthImportFile.cpp \
    ./Sources/createOceanDataViewFile.cpp \
    ./Sources/createShapefile.cpp \
    ./Sources/createTextFile.cpp \
    ./Sources/extractExif.cpp \
    ./Sources/analyzeDataset.cpp \
    ./Sources/convertDateTime.cpp \
    ./Sources/buildHeader.cpp \
    ./Sources/buildParameter.cpp \
    ./Sources/getEnvironment.cpp \
    ./Sources/setDeviceType.cpp \
    ./Sources/createDir.cpp \
    ./Sources/getDOI.cpp \
    ./Sources/getCitation.cpp \
    ./Sources/getDatasets.cpp \
    ./Sources/getMetadata.cpp \
    ./Sources/startProgram.cpp \
    ./Sources/containsSelectedEnvironments.cpp \
    ./Sources/setFilename.cpp \
    ./Sources/setGeocodeRange.cpp \
    ./Sources/QualityFlag.cpp \
    ./Sources/writeDataLine.cpp \
    ./Sources/Webfile.cpp \
    ./Sources/dbfopen.c \
    ./Sources/shpopen.c \
    ./Sources/safileio.c \
    ./Forms/AboutDialog/AboutDialog.cpp \
    ./Forms/PanGetDialog/PanGetDialog.cpp \
    ./Forms/SetGeocodeRangeDialog/SetGeocodeRangeFloatDialog.cpp \
    ./Forms/SetGeocodeRangeDialog/SetGeocodeRangeDateTimeDialog.cpp \
    ./Forms/SetGearIDDialog/SetGearIDDialog.cpp \
    ./Forms/GoogleEarthOptionsDialog/GoogleEarthOptionsDialog.cpp \
    ./Forms/OceanDataViewOptionsDialog/OceanDataViewOptionsDialog.cpp \
    ./Forms/ShapeFileOptionsDialog/ShapeFileOptionsDialog.cpp \
    ./Forms/formatedTextOptionsDialog/formatedTextOptionsDialog.cpp \
    ./Forms/unformatedTextOptionsDialog/unformatedTextOptionsDialog.cpp \
    ./Forms/SelectParametersDialog/selectParametersDialog.cpp \
    ./Forms/SelectEnvironmentDialog/selectEnvironmentDialog.cpp \
    ./Forms/ExifToolOptionsDialog/ExifToolOptionsDialog.cpp

FORMS = ./Forms/AboutDialog/aboutdialog.ui \
    ./Forms/PanGetDialog/pangetdialog.ui \
    ./Forms/GoogleEarthOptionsDialog/googleearthoptionsdialog.ui \
    ./Forms/SetGeocodeRangeDialog/setgeocoderangefloatdialog.ui \
    ./Forms/SetGeocodeRangeDialog/setgeocoderangedatetimedialog.ui \
    ./Forms/SetGearIDDialog/setgeariddialog.ui \
    ./Forms/OceanDataViewOptionsDialog/oceandataviewoptionsdialog.ui \
    ./Forms/ShapeFileOptionsDialog/shapefileoptionsdialog.ui \
    ./Forms/formatedTextOptionsDialog/formatedtextoptionsdialog.ui \
    ./Forms/unformatedTextOptionsDialog/unformatedtextoptionsdialog.ui \
    ./Forms/SelectParametersDialog/selectparametersdialog.ui \
    ./Forms/SelectEnvironmentDialog/selectenvironmentdialog.ui \
    ./Forms/ExifToolOptionsDialog/ExifTooloptionsdialog.ui
