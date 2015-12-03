#ifndef FEATUREGRAPHICSSERVICE_H
#define FEATUREGRAPHICSSERVICE_H

#include <QString>
#include <models/utils/matrix.h>

class FeatureGraphicsService
{
public:
    FeatureGraphicsService();

    /**
     * @brief generateHeatmap Generates a svg file and gives the route
     * @param protName
     * @param data
     * @return
     */
    QString generateHeatmap(QString protName, Matrix <float> *data);
    /**
     * @brief generateMixedHeatmap Generates a svg file mixing two matrices column by column and gives the route
     * @param protName
     * @param data
     * @param dataAux
     * @return
     */
    QString generateMixedHeatmap(QString protName, Matrix <float> *data, Matrix <float> *dataAux);
private:
    QString filterSVGFile(QString cairoSvgFile);
};

#endif // FEATUREGRAPHICSSERVICE_H
