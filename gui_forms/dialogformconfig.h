#ifndef DIALOGFORMCONFIG_H
#define DIALOGFORMCONFIG_H

#include <QDialog>
#include "modelo/configuracion.h"
#include <QAbstractButton>

namespace Ui {
class DialogFormConfig;
}

class DialogFormConfig : public QDialog
{
    Q_OBJECT

public:
    explicit DialogFormConfig(QWidget *parent = 0);
    ~DialogFormConfig();

private slots:

    /**
     * @brief on_qdbb_almacenar_clicked Slot para evento sobre los botones
     * @param <QAbstractButton *> button
     */
    void on_qdbb_almacenar_clicked(QAbstractButton *button);

    /**
     * @brief on_tbtn_dirTmp_clicked Slot para evento del botón de herramientas en el path del directorio temporal.
     */
    void on_tbtn_dirTmp_clicked();

    /**
     * @brief on_tbtn_dirFFtmp_clicked Slot para evento del botón de herramientas en el path del directorio temporal ff.
     */
    void on_tbtn_dirFFtmp_clicked();

private:
    Ui::DialogFormConfig *ui;
    Configuracion *config;

    //Métodos privados
    void cargarConfiguracion();

    /**
     * @brief guardarClicked
     */
    void guardarClicked();

    /**
     * @brief cancelarClicked
     */
    void cancelarClicked();

    /**
     * @brief resetearClicked
     */
    void resetearClicked();
};

#endif // DIALOGFORMCONFIG_H
