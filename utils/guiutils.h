#ifndef GUIUTILS_H
#define GUIUTILS_H

#include <QLayout>

class GUIUtils
{
public:
    GUIUtils();

    /**
     * @brief limpiarLayout desocupa un layout para ingresarle nuevos elementos
     * @param <QLayout *> pqly_layout putnero a layout que hay que modificar
     */
    static void limpiarLayout(QLayout *pqly_layout);
};

#endif // GUIUTILS_H
