#ifndef FEATUREDEFINITION_H
#define FEATUREDEFINITION_H

#include <QObject>

class FeatureDefinition : public QObject
{
    Q_OBJECT
    Q_PROPERTY(GeneratorMethod method READ method WRITE setMethod NOTIFY methodChanged)
    Q_PROPERTY(int sqrtSize READ sqrtSize WRITE setSqrtSize NOTIFY sizeChanged)
    Q_PROPERTY(double treshold READ treshold WRITE setTreshold NOTIFY tresholdChanged)
    Q_ENUMS (GeneratorMethod)

public:
    explicit FeatureDefinition(QObject *parent = 0);

    enum GeneratorMethod {Distance, Angle, Mixture, Undefined};

    GeneratorMethod method() const;
    void setMethod(const GeneratorMethod &method);

    int sqrtSize() const;
    void setSqrtSize(int sqrtSize);

    double treshold() const;
    void setTreshold(double treshold);

    int calculatedSize() const;

signals:
    void methodChanged();
    void sizeChanged();
    void tresholdChanged();

public slots:

private:
    GeneratorMethod _method;
    int _sqrtSize;
    double _treshold;
};

#endif // FEATUREDEFINITION_H
