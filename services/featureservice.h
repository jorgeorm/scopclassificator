#ifndef FEATURESERVICE_H
#define FEATURESERVICE_H

#include <models/featuredefinition.h>
#include <models/scopentry.h>
#include <models/utils/matrix.h>


class FeatureService
{
//--------------------------------PUBLIC SECTION--------------------------------
public:
    FeatureService();

    /**
    * @brief rawMetricsMatrix
    * @param featureConfig
    * @param entry
    * @return
    */
    Matrix<float> *rawMetricsMatrix(FeatureDefinition *featureConfig, SCOPEntry *entry);

    /**
    * @brief localFeaturesMatrix
    * @param featureConfig
    * @param entry
    * @return
    */
    Matrix<float> *localFeaturesMatrix(FeatureDefinition *featureConfig, SCOPEntry *entry);


    /**
     * @brief normalizeMatrix
     * @param matrix
     * @param norm
     * @return
     */
    Matrix<float> *normalizeMatrix(const Matrix<float> *matrix, double norm = 0);


    /**
     * @brief saveCalculatedMatrix Stores a matrix into a specified file
     * @param matrix
     * @param fileName
     */
    void saveCalculatedMatrix(Matrix <float> *matrix, QString fileName);


    /**
     * @brief loadCalculatedMatrix Loads a matrix from a specific file
     * @param filePath
     * @return
     */
    Matrix <float> *loadCalculatedMatrix(QString filePath);

    /**
     * @brief concatenateFeatureFiles
     * @param files
     * @return Path to file that has been concatenated
     */
    QString concatenateFeatureFiles(const QStringList &files);
//--------------------------------PROTECTED SECTION--------------------------------

protected:

    /**
     * @brief calculateMetricsMatrix
     * @param entry
     * @return
     */
    Matrix<float> *calculateMetricsMatrix(
            FeatureDefinition::GeneratorMethod method,
            SCOPEntry *entry,
            double treshold = std::numeric_limits<double>::max());

    /**
     * @brief calculateLocalFeatures
     * @param featureConfig
     * @param entry
     * @return
     */
    Matrix<float> *calculateLocalFeatures(FeatureDefinition *featureConfig, SCOPEntry *entry);

    /**
     * @brief mixMetricMatrices
     * @param metric
     * @param auxMetric
     * @return
     */
    Matrix<float> *mixMetricMatrices(Matrix<float> *metric, Matrix<float> *auxMetric);

};

#endif // FEATURESERVICE_H
