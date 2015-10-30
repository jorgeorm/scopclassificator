#include "algorithmservice.h"
#include <QStringBuilder>
#include <QString>

AlgorithmService::AlgorithmService(){

    QString clara_checkInstallNLoad = QString("if(! (\"cluster\" %in% installedPackages)){\n") %
            QString("    install.packages(\"cluster\", dependencies = TRUE);\n") %
            QString("}\nlibrary('cluster')\n");
    _preAlgorithms.insert("CLARA", clara_checkInstallNLoad);
    _algorithms.insert("CLARA", "clust <- clara (x, k=");
    _postAlgorithms.insert("CLARA", "clust.matrix <- clust$medoids;");

}

QString AlgorithmService::algorithm(QString algName){
    return (_algorithms.contains(algName) ?
                _algorithms.value(algName) :
                "");
}

QString AlgorithmService::preAlgorithm(QString algName){
    QString aboutInstall =
            QString("#! /usr/bin/Rscript --vanilla --default-packages=utils\n") %
            QString(".libPaths (c (.libPaths (), \"lib/R\"))\n") %
            QString("r = getOption (\"repos\");\n") %
            QString("r[\"CRAN\"] = \"https://cran.rstudio.com\";\n") %
            QString("options(repos = r);\n") %
            QString("rm(r);\n") %
            QString("installedPackages <- rownames (installed.packages());\n");



    return aboutInstall %
            (_preAlgorithms.contains(algName) ?
                _preAlgorithms.value(algName) :
                "");
}

QString AlgorithmService::postAlgorithm(QString algName, QString outputFile) {
    return (_postAlgorithms.contains(algName) ?
                _postAlgorithms.value(algName) : "") %
            "\nwrite.csv(clust.matrix, file = \"" % outputFile % "\");";
}
