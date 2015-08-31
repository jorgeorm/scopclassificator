#include "archivosutils.h"
#include <QTextStream>
#include <fstream>
#include <iostream>

using namespace std;

ArchivosUtils::ArchivosUtils()
{
}

bool ArchivosUtils::copiarQFile(QFile *archivo, QString pathSalida){
        QTextStream istream( archivo );
        ofstream flujoSalida;

        bool b_inputOpen = archivo->open(QFile::ReadOnly);

        flujoSalida.open(qPrintable(pathSalida));
        bool b_outputOpen = flujoSalida.is_open();

        if(b_inputOpen && b_outputOpen){

            do{
                flujoSalida<<qPrintable(istream.readLine());
                flujoSalida<<"\n";
            }while( !istream.atEnd() );

            flujoSalida.close();

            istream.flush();
            archivo->flush();
            archivo->close();

            return true;

        }else {
            cout<<"(EE) copiarQFile(): El archivo ";
            if(!b_inputOpen) {
                cout<<qPrintable(archivo->fileName())<<" ";
            }
            if(!b_outputOpen) {
                cout <<qPrintable( pathSalida )<<" ";
            }
            cout<<"no pudo ser abierto"<<endl;
        }

        return false;

}

bool ArchivosUtils::copiarArchivo(QString pathEntrada, QString pathSalida){
    ifstream flujoEntrada;
    ofstream flujoSalida;
    char buffer [200];

    bool b_inputOpen, b_outputOpen;

    flujoEntrada.open(qPrintable(pathEntrada));
    b_inputOpen = flujoEntrada.is_open();

    flujoSalida.open( qPrintable( pathSalida ) );
    b_outputOpen = flujoSalida.is_open();

    if(b_inputOpen && b_outputOpen){
        while( !flujoEntrada.eof() ){
            flujoEntrada>>buffer;
            flujoSalida<<buffer;
        }

        flujoSalida.close();
        flujoEntrada.close();
        return true;
    }

    return false;
}
