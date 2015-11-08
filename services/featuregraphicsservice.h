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
private:
    QString filterSVGFile(QString cairoSvgFile);
};

#endif // FEATUREGRAPHICSSERVICE_H
