#ifndef FEATURESERVICE_H
#define FEATURESERVICE_H

#include <models/featuredefinition.h>
#include <models/scopentry.h>
#include <models/utils/matrix.h>


/**
 * @brief The FeatureService class has the logic to generate and handle feature operations
 */
class FeatureService
{
//--------------------------------PUBLIC SECTION--------------------------------
public:
    FeatureService();


    /**
     * @brief rawMetricsMatrix Generates a matrix of distances or a matrix of angles depending on what is specified in the feature definition
     * @param method
     * @param entry
     * @param treshold
     * @return
     */
    Matrix<float> *rawMetricsMatrix(FeatureDefinition::GeneratorMethod method,
                                    SCOPEntry *entry, const double treshold = 0);

    /**
    * @brief localFeaturesMatrix Generates a local feature Matrix using the feature configuration
    * @param featureConfig Specifies how's gonna be generated the feature
    * @param entry SCOPEntry used to generate the feature matrix
    * @return
    */
    Matrix<float> *localFeaturesMatrix(FeatureDefinition *featureConfig, SCOPEntry *entry);


    /**
     * @brief scaleMatrixByNumber Generates a new Matrix which will be scaled by the value specified
     * @param matrix matrix which is gonna be scaled
     * @param norm scalar value
     * @return scaled matrix
     */
    Matrix<float> *scaleMatrixByNumber(const Matrix<float> *matrix, double norm = 0);


    /**
     * @brief saveCalculatedMatrix Stores a feature matrix into a specified file
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

    /**
     * @brief euclideanDistance Calculates heuclidean distance between two features
     * @param feat1
     * @param feat2
     * @param featSize
     * @return Euclidean distance as float
     */
    float euclideanDistance(float *feat1, float *feat2, unsigned featSize);
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
            const double treshold = std::numeric_limits<double>::max());

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
