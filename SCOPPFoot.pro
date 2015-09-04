#-------------------------------------------------
#
# Project created by QtCreator 2013-03-16T16:07:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GUI_SCOPPFoot
TEMPLATE = app


SOURCES += main.cpp\
        GUI_SCOPPFoot.cpp \
    control/SCOPPFoot.cpp \
    modelo/pdbhandler.cpp \
    libs/bioLib/BioC++.cpp \
    libs/rLib/Rconnection.cc \
    modelo/matriz.cpp \
    gui_forms/FormDataset.cpp \
    control/GestorDataset.cpp \
    gui_forms/FormFeatures.cpp \
    gui_forms/ClusterFormFactory.cpp \
    gui_forms/clustering_forms/CLARAForm.cpp \
    control/RParser.cpp \
    control/FeaturesProcesor.cpp \
    control/Procesors/RServClient.cpp \
    libs/rLib/sisockV2.cpp \
    libs/rLib/Rsrv.cpp \
    control/FootprintCalculator.cpp \
    utils/arrayutils.cpp \
    control/Procesors/claraprocesor.cpp \
    control/Procesors/hclustprocesor.cpp \
    utils/archivosutils.cpp \
    utils/guiutils.cpp \
    gui_forms/dialogformconfig.cpp \
    control/gestorconfiguracion.cpp \
    modelo/configuracion.cpp \
    gui_forms/clustering_forms/hclustform.cpp \
    utils/randomutils.cpp


HEADERS  += GUI_SCOPPFoot.h \
    control/SCOPPFoot.h \
    modelo/pdbhandler.h \
    libs/bioLib/BioC++.h \
    libs/rLib/Rconnection.h \
    libs/rLib/Rsrv.h \
    libs/rLib/config.h \
    modelo/matriz.h \
    gui_forms/FormDataset.h \
    control/GestorDataset.h \
    gui_forms/FormFeatures.h \
    gui_forms/ClusterFormFactory.h \
    gui_forms/clustering_forms/CLARAForm.h \
    control/RParser.h \
    control/FeaturesProcesor.h \
    control/Procesors/RServClient.h \
    libs/rLib/sisocks.h \
    libs/rLib/sisockV2.h \
    control/FootprintCalculator.h \
    utils/arrayutils.h \
    control/Procesors/claraprocesor.h \
    control/Procesors/hclustprocesor.h \
    utils/archivosutils.h \
    utils/guiutils.h \
    gui_forms/dialogformconfig.h \
    control/gestorconfiguracion.h \
    modelo/configuracion.h \
    gui_forms/clustering_forms/hclustform.h \
    utils/randomutils.h

FORMS    += GUI_SCOPPFoot.ui \
    gui_forms/FormDataset.ui \
    gui_forms/FormFeatures.ui \
    gui_forms/clustering_forms/CLARAForm.ui \
    gui_forms/dialogformconfig.ui \
    gui_forms/clustering_forms/hclustform.ui

RESOURCES += \
    resources/img/imagenes.qrc \
    gui_forms/forms.qrc \
    adds/rScripts.qrc

OTHER_FILES += DoxyGenCfg \
    Leeme.txt \
    adds/heatMap.R\
    adds/escala.R\
    resources/img/cargarModelo.png\
    adds/testBigData.R \
    utils/pdbhandler.py \
    adds/ffClara.R \
    adds/clarans.R

# Configuración para librerías de R, Rcpp, RInside

#QT += svg ## Para poder hacer uso de las gráficas en SVG en la aplicación
## Se debe comentar si se necesita una versión diferente de R,
## y setear la variable R_HOME como una variable de ambiente
R_HOME =                $$system(R RHOME)
R_LIBS_USER=            $$system(echo $HOME/R/x86_64-pc-linux-gnu-library/3.0)


## Incluye los headers y librerías para R
RCPPFLAGS = 		$$system(sh adds/getRcppFlags.sh)
RLDFLAGS = 		$$system(sh adds/getRldFlags.sh)
RBLAS = 		$$system($$R_HOME/bin/R CMD config BLAS_LIBS)
RLAPACK = 		$$system($$R_HOME/bin/R CMD config LAPACK_LIBS)

## Si se necesita setear el paath a R manualmente descomentar
## y editar lo que aparece abajo
# RRPATH =		-Wl,-rpath,$$R_HOME/lib

## Incluye los headers y librerías para las clases interfaz para Rcpp
RCPPINCL = 		$$system(sh adds/getRcpp.sh $$R_LIBS_USER -rcpp.cxx)
RCPPLIBS = 		$$system(sh adds/getRcpp.sh $$R_LIBS_USER -rcpp.ld)

## Por alguna razón cuando se construey con Qt obtenemos esto cada vez
## así que ponemos no utilizado el parametro de warnings off.
RCPPWARNING =		-Wno-unused-parameter

## Incluye los headers y las libreerías de RInside clases embebidas
RINSIDEINCL = 		$$system(sh adds/getRcpp.sh $$R_LIBS_USER -rinside.cxx)
RINSIDELIBS = 		$$system(sh adds/getRcpp.sh $$R_LIBS_USER -rinside.ld)

## compiler etc settings used in default make rules
## Parámetros utilizados y demás en las reglas por defecto del make

QMAKE_CXXFLAGS +=       $$RCPPWARNING \
                $$RCPPFLAGS \
                $$RCPPINCL \
                $$RINSIDEINCL
LIBS += -lcrypt
QMAKE_LIBS +=         $$RLDFLAGS \
                $$RBLAS \
                $$RLAPACK \
                $$RCPPLIBS \
                $$RINSIDELIBS
## Targets adicionales de limpieza
#CONFIG += uitools
#QMAKE_CLEAN += TG-Debug/*
