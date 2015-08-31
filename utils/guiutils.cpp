#include "guiutils.h"
#include <iostream>
#include <typeinfo>//Para identificar tipos de datos
#include <gui_forms/FormDataset.h>
#include <gui_forms/FormFeatures.h>
#include <gui_forms/clustering_forms/CLARAForm.h>

GUIUtils::GUIUtils()
{
}

void GUIUtils::limpiarLayout(QLayout *pqly_layout){

    QLayoutItem *itemLay = NULL;

    while((itemLay = pqly_layout->takeAt(0))) {
        if (itemLay->layout()) {
            limpiarLayout(itemLay->layout());
            delete itemLay->layout();
        }

        if(QWidget * widget = itemLay->widget()){

            if (FormDataset * datasetForm = qobject_cast<FormDataset *>(widget)) {
                delete datasetForm;
            }else if(FormFeatures *featsForm = qobject_cast<FormFeatures *>(widget)){
                delete featsForm;
            }else if(CLARAForm * form_clara = qobject_cast<CLARAForm *>(widget)){
                delete form_clara;
            }
        }
        delete itemLay;
        itemLay = NULL;
    }

    //std::cout<<"(II) GUIUtils(limpiarLayout): Limpió layout"<<std::endl;

}
