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
    models/modelevaluation.cpp \
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
    controllers/modelevaluationcontroller.cpp

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
    models/modelevaluation.h \
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
    controllers/modelevaluationcontroller.h

FORMS    += mainwindow.ui \
    views/datasetview.ui \
    views/featuredefinitionview.ui \
    views/modelevaluationview.ui \
    views/modeltrainingview.ui \
    views/clusteringSpecificViews/claraoptions.ui

DISTFILES += \
    lib/R/getRflag.sh \
    lib/R/installRDepencies.R

CONFIG += c++11

# Scalable Vector Graphics (SVG) is an XML-based language for describing
# two-dimensional vector graphics. Qt provides classes for rendering and
# displaying SVG drawings in widgets and on other paint devices.
QT += svg

## comment this out if you need a different version of R,
## and set set R_HOME accordingly as an environment variable
R_HOME = 		$$system(R RHOME)

#message("R_HOME is" $$R_HOME)

## include headers and libraries for R
RCPPFLAGS =     $$system($$R_HOME/bin/R CMD config --cppflags)
RLDFLAGS = 		$$system($$R_HOME/bin/R CMD config --ldflags)
RBLAS = 		$$system($$R_HOME/bin/R CMD config BLAS_LIBS)
RLAPACK = 		$$system($$R_HOME/bin/R CMD config LAPACK_LIBS)

## Added R library
R_LIBS = "./lib/R/"
## Installs R required packages
message("--------------Instalation---------------")
RINST = $$system($$R_HOME/bin/R CMD BATCH $$R_LIBS/installRDepencies.R)

message("-------------After Install--------------$$RINST")

## if you need to set an rpath to R itself, also uncomment
RRPATH =		-Wl,-rpath,$$R_HOME/lib, $$R_LIBS

## include headers and libraries for Rcpp interface classes
## note that RCPPLIBS will be empty with Rcpp (>= 0.11.0) and can be omitted
message("--------------Include SH---------------")
RCPPINCL = 		$$system(sh $$R_LIBS/getRflag.sh $$R_LIBS -rcpp.cxx)
message("______________________RCPP______________________")
message("RCPPINCL=> $$RCPPINCL")
#RCPPLIBS = 		$$system(sh $$R_LIBS/getRflag.sh $$R_LIBS -rcpp.ld)
#message($$RCPPLIBS)

## for some reason when building with Qt we get this each time
##   /usr/local/lib/R/site-library/Rcpp/include/Rcpp/module/Module_generated_ctor_signature.h:25: warning: unused parameter ‘classname
## so we turn unused parameter warnings off
## no longer needed with Rcpp 0.9.3 or later
#RCPPWARNING =		-Wno-unused-parameter

## include headers and libraries for RInside embedding classes
## Incluye los headers y las libreerías de RInside clases embebidas
RINSIDEINCL = 		$$system(sh $$R_LIBS/getRflag.sh $$R_LIBS -rinside.cxx)
RINSIDELIBS = 		$$system(sh $$R_LIBS/getRflag.sh $$R_LIBS -rinside.ld)

message("____________________________RInside____________________________")
message("RINSIDEINCL => $$RINSIDEINCL")
message("RINSIDELIBS => $$RINSIDELIBS")

## compiler etc settings used in default make rules
QMAKE_CXXFLAGS += $$RCPPWARNING $$RCPPFLAGS $$RCPPINCL $$RINSIDEINCL
QMAKE_LIBS += $$RLDFLAGS $$RBLAS $$RLAPACK $$RINSIDELIBS # $$RRPATH # $$RCPPLIBS

## QMAKE_CLEAN += scop_classificator Makefile

