#include "featuregraphicsservice.h"
#include <QFile>
#include <QStringBuilder>
#include <Rcpp.h>

FeatureGraphicsService::FeatureGraphicsService() {

}

QString FeatureGraphicsService::generateHeatmap(
        QString protName,Matrix <float> *data){

    QString svgFilePath = "/tmp/" % protName % ".svg" ;
    QString rscriptTmp = "/tmp/heatMap_" % protName % ".R";

    QFile rFile(rscriptTmp);

    if(rFile.open(QIODevice::WriteOnly)){
        QTextStream stream(& rFile);

        QString setLibPath(".libPaths( c( .libPaths(), \"lib/R\") )");
        QString loadR("r = getOption(\"repos\"); ");
        QString loadCran("r[\"CRAN\"] = \"https://cran.rstudio.com\"; ");
        QString setOption("options(repos = r); ");
        QString deleteR("rm(r);");
        QString packages("installedPackages <- rownames (installed.packages()); ");
        QString ifPlotlyDependency("if(! (\"ggplot2\" %in% installedPackages)){ ");
        QString dependency1("install.packages(\"ggplot2\", dependencies = TRUE); ");
        QString dependency2("install.packages(\"RColorBrewer\", dependencies = TRUE); }");

        stream << setLibPath << endl;
        stream << loadR << endl;
        stream << loadCran << endl;
        stream << setOption << endl;
        stream << deleteR << endl;
        stream << packages << endl;
        stream << ifPlotlyDependency << endl;
        stream << dependency1 << endl;
        stream << dependency2 << endl;


        stream << "svgTmp <- \"" << svgFilePath << "\"" << endl;

        stream << "metrics <- matrix( c( ";
        for(unsigned i = 0; i < data->rows(); ++i){
            for(unsigned j = 0; j < data->cols(); ++j){
                stream<<data->operator ()(i, j);
                if (j < data->cols() - 1) stream << ", ";
            }
            if (i < data->rows() - 1) stream << ", ";
        }

        stream << "), nrow = " << data->rows() << ", ncol = " << data->cols();
        stream << ", byrow = T, dimnames = list( residueA = NULL, residueB = NULL) )" << endl;

        QString graphCode = "data.frame <- as.data.frame(as.table(metrics)); require(RColorBrewer); colorList <- rev(brewer.pal(n = 11, \"Spectral\")); colorList <- colorList[1:7]; colorPFun <- colorRampPalette(colorList); colorPSize <- 256; colorP <- colorPFun(colorPSize); require(ggplot2); (heatMap <- ggplot(data.frame, aes(x = residueA, y = residueB, fill = Freq))+ geom_tile()+ scale_fill_gradient2(low = colorP[1], mid = colorP[colorPSize / 2], high = colorP[colorPSize], midpoint = (max(data.frame$Freq) + min(data.frame$Freq)) / 2, name = \"Metric\")) + scale_x_discrete(breaks=NULL) + scale_y_discrete(breaks=NULL); ggsave(svgTmp, dpi = 300); dev.off()";

        stream << graphCode << endl;

        rFile.close();


        QString rCmd = "R CMD BATCH " % rscriptTmp;
        int res = system(rCmd.toStdString().c_str());

        if (res != 0) {
            qDebug() << "There was an error with the command " << graphCode;
            return "";
        }
    }

    return svgFilePath;           	// we need to simplify the svg file for display by Qt
}

QString FeatureGraphicsService::filterSVGFile(QString cairoSvgFile) {
    // cairoDevice creates richer SVG than Qt can display
    // but per Michaele Lawrence, a simple trick is to s/symbol/g/ which we do here
    QString svgFilePath = "/tmp/image.svg";

    QFile infile(cairoSvgFile);
    infile.open(QFile::ReadOnly);
    QFile outfile(svgFilePath);
    outfile.open(QFile::WriteOnly | QFile::Truncate);

    QTextStream in(&infile);
    QTextStream out(&outfile);
    QRegExp rx1("<symbol");
    QRegExp rx2("</symbol");
    while (!in.atEnd()) {
        QString line = in.readLine();
        line.replace(rx1, "<g"); // so '<symbol' becomes '<g ...'
        line.replace(rx2, "</g");// and '</symbol becomes '</g'
        out << line << "\n";
    }
    infile.close();
    outfile.close();

    return svgFilePath;
}
