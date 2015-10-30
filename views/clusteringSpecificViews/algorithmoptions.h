#ifndef ALGORITHMOPTIONS_H
#define ALGORITHMOPTIONS_H

#include <QWidget>

class AlgorithmOptions : public QWidget
{
    Q_OBJECT
public:
    explicit AlgorithmOptions(QWidget *parent = 0);
    virtual QString optionsAsString() = 0;

    /**
     * @brief clusters
     */
    unsigned clusters() const;
    /**
     * @brief setClusters
     * @param clusters
     */
    void setClusters(const unsigned &clusters);

protected:
    unsigned _clusters;

};

#endif // ALGORITHMOPTIONS_H
