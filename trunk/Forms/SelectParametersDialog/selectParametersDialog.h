#ifndef SELECTPARAMETERSDIALOG_H
#define SELECTPARAMETERSDIALOG_H

#include <QDialog>

#include "ui_selectparametersdialog.h"

class SelectParametersDialog : public QDialog, public Ui::SelectParametersDialog
{
Q_OBJECT

public:
    SelectParametersDialog( QStringList list, int n = 1000, bool m = false, QWidget *parent = 0 );
    void enableOKButton();
    void setItemBackground();

private slots:
    void selectMetadata();
    void selectData();
    void selectGeocode();
    void selectEventGeocodeData();
    void deselectAll();
    void sortList();
    void slotLeft2Right();
    void slotRight2Left();

private:
    int         i_maxNumOfItems;
    QStringList sl_ItemList;
    bool        b_CuratorMode;
};

#endif
