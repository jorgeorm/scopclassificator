#-------------------------------------------------
#
# Project created by QtCreator 2015-09-15T17:27:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = scoclassificator_v2
TEMPLATE = app
SOURCES += main.cpp\
        mainwindow.cpp \
    services/datasetservice.cpp \
    models/dataset.cpp \
    controllers/datasetcontroller.cpp \
    views/datasetview.cpp \
    models/configuration.cpp \
    controllers/configurationcontroller.cpp \
    services/configurationservice.cpp \
    services/loggerservice.cpp \
    lib/randomgenerator.cpp \
    models/scopentry.cpp \
    services/scopentryservice.cpp \
    views/viewModels/scopentrytablemodel.cpp \
    models/bio/atom.cpp \
    models/bio/chain.cpp \
    models/bio/residue.cpp \
    models/featuredefinition.cpp \
    models/predictivemodel.cpp \
    views/featuredefinitionview.cpp \
    controllers/featurecontroller.cpp \
    services/featureservice.cpp \
    views/modelevaluationview.cpp \
    models/utils/matrix.cpp \
    services/featuregraphicsservice.cpp \
    views/modeltrainingview.cpp \
    views/clusteringSpecificViews/algorithmoptions.cpp \
    views/clusteringSpecificViews/claraoptions.cpp \
    services/clusteringservice.cpp \
    models/clusteringresult.cpp \
    services/clusteringalgorithmgeneratorservice.cpp \
    controllers/modeltrainingcontroller.cpp \
    services/algorithmservice.cpp \
    services/predictivemodelservice.cpp \
    controllers/modelevaluationcontroller.cpp \
    models/predictedclassification.cpp \
    views/clusteringSpecificViews/kmeansoptions.cpp

HEADERS  += mainwindow.h \
    services/datasetservice.h \
    models/dataset.h \
    controllers/datasetcontroller.h \
    views/datasetview.h \
    models/configuration.h \
    controllers/configurationcontroller.h \
    services/configurationservice.h \
    services/loggerservice.h \
    lib/randomgenerator.h \
    models/scopentry.h \
    services/scopentryservice.h \
    views/viewModels/scopentrytablemodel.h \
    models/bio/atom.h \
    models/bio/chain.h \
    models/bio/residue.h \
    models/bio/bio.h \
    models/featuredefinition.h \
    models/predictivemodel.h \
    views/featuredefinitionview.h \
    controllers/featurecontroller.h \
    services/featureservice.h \
    views/modelevaluationview.h \
    models/utils/matrix.h \
    services/featuregraphicsservice.h \
    views/modeltrainingview.h \
    views/clusteringSpecificViews/algorithmoptions.h \
    views/clusteringSpecificViews/claraoptions.h \
    controllers/modeltrainingcontroller.h \
    services/clusteringservice.h \
    models/clusteringresult.h \
    services/clusteringalgorithmgeneratorservice.h \
    services/algorithmservice.h \
    services/predictivemodelservice.h \
    controllers/modelevaluationcontroller.h \
    models/predictedclassification.h \
    views/clusteringSpecificViews/kmeansoptions.h

FORMS    += mainwindow.ui \
    views/datasetview.ui \
    views/featuredefinitionview.ui \
    views/modelevaluationview.ui \
    views/modeltrainingview.ui \
    views/clusteringSpecificViews/claraoptions.ui \
    views/clusteringSpecificViews/kmeansoptions.ui

DISTFILES += \
    lib/R/getRflag.sh \
    lib/R/installRDepencies.R

CONFIG += c++11

# Scalable Vector Graphics (SVG) is an XML-based language for describing
# two-dimensional vector graphics. Qt provides classes for rendering and
# displaying SVG drawings in widgets and on other paint devices.
QT += svg
