/**
 * \file main.cpp
 * \brief Pone a correr la aplicacion e interpreta parametros de entrada
 * Captura parametros de entrada para la configuraci´on de opciones adicionales.
 * \author Jorge Ordoñez Mendez - jorge.ordonez@correounivalle.edu.co
 */
#include "GUI_SCOPPFoot.h"
#include <QApplication>
#include <RInside.h>
#include <QTranslator>
#include <QLibraryInfo>


int main(int argc, char * argv[])
{

    RInside instancia(argc, argv);
//    system("killall -9 Rserve");
//    system(" $R_HOME/bin/R CMD  $R_HOME/bin/Rserve  --no-save");
    QApplication a(argc, argv);


    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
         QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&qtTranslator);

    /* load our custom translations for this application */
    QTranslator myappTranslator;
    myappTranslator.load("myapp_" + QLocale::system().name());
    a.installTranslator(&myappTranslator);

    GUI_SCOPPFoot w(0);
    w.show();
    
    return a.exec();
}
