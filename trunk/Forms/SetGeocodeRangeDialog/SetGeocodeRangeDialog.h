#ifndef SETGEOCODERANGEDIALOG_H
#define SETGEOCODERANGEDIALOG_H

#include <QDialog>

#include "ui_SetGeocodeRangeDialog.h"

class SetGeocodeRangeDialog : public QDialog, public Ui::SetGeocodeRangeDialog
{
Q_OBJECT

public:
    SetGeocodeRangeDialog( QWidget *parent = 0 );

public slots:
    void enableDialogEntries();
};

#endif
