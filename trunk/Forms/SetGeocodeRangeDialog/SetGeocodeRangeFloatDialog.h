#ifndef SETGEOCODERANGEFLOATDIALOG_H
#define SETGEOCODERANGEFLOATDIALOG_H

#include <QDialog>

#include "ui_setgeocoderangefloatdialog.h"

class SetGeocodeRangeFloatDialog : public QDialog, public Ui::SetGeocodeRangeFloatDialog
{
Q_OBJECT

public:
    SetGeocodeRangeFloatDialog( QWidget *parent = 0 );
};

#endif
