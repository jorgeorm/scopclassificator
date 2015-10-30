#include "mainwindow.h"
#include <QApplication>
#include <QProcess>
#include <RInside.h>

#ifdef Q_OS_LINUX
void rserveStop(){
    QProcess psRserve;
    psRserve.start("bash", QStringList()<<"-c"<<"ps -A | grep 'Rserve'");
    psRserve.waitForBytesWritten();
    psRserve.waitForFinished();


    if(! psRserve.readAll().isEmpty()){
        QProcess kill;
        kill.start("killall", QStringList()<<"-9"<<"Rserve");
        kill.waitForBytesWritten();
        kill.waitForFinished();
    }
}

void rserveStart(RInside &R)
{
    rserveStop();

    R.parseEvalQ("library('Rserve')");
    R.parseEvalQ("Rserve(args='--no-save')");

}
#else
void rserveStart(RInside R)
{
    R.parseEvalQ("library('Rserve')");
    R.parseEvalQ("Rserve(args='--no-save')");
}
#endif

int main(int argc, char *argv[])
{
    RInside R(argc, argv);  		// creates an embedded R instance

    rserveStart(R);

    QApplication a(argc, argv);
    MainWindow w(R);
    w.show();

    rserveStop();

    return a.exec();
}
