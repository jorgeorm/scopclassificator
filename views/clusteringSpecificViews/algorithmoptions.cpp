#include "algorithmoptions.h"

#include <QWidget>

AlgorithmOptions::AlgorithmOptions(QWidget *parent) : QWidget(parent){

}

unsigned AlgorithmOptions::clusters() const{
    return _clusters;
}

void AlgorithmOptions::setClusters(const unsigned &clusters){
    _clusters = clusters;
}

