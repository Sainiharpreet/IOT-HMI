# Add files and directories to ship with the application 
# by adapting the examples below.
# file1.source = myfile
# dir1.source = mydir
Qt += androidextras
DEPLOYMENTFOLDERS = # file1 dir1

# If your application uses the Qt Mobility libraries, uncomment
# the following lines and add the respective components to the 
# MOBILITY variable. 
# CONFIG += mobility
# MOBILITY +=

SOURCES += main.cpp mainwindow.cpp \
    usb.cpp
HEADERS += mainwindow.h \
    usb.h
FORMS += mainwindow.ui

# Please do not modify the following two lines. Required for deployment.
include(deployment.pri)
qtcAddDeployment()

OTHER_FILES += \
    android/res/drawable-ldpi/icon.png \
    android/res/values-zh-rTW/strings.xml \
    android/res/values-fr/strings.xml \
    android/res/values-id/strings.xml \
    android/res/drawable/icon.png \
    android/res/drawable/logo.png \
    android/res/values-ru/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/values-es/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-de/strings.xml \
    android/res/drawable-mdpi/icon.png \
    android/res/values-it/strings.xml \
    android/res/layout/splash.xml \
    android/res/values-el/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values-nl/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-ms/strings.xml \
    android/res/drawable-hdpi/icon.png \
    android/res/values/libs.xml \
    android/res/values/strings.xml \
    android/res/values-ja/strings.xml \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/kde/necessitas/origo/QtApplication.java \
    android/src/org/kde/necessitas/origo/QtActivity.java \
    android/AndroidManifest.xml \
    android/version.xml

RESOURCES += \
    scada_image.qrc
