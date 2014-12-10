#ifndef GEOCODERANGEDIALOG_H
#define GEOCODERANGEDIALOG_H

#include <QDialog>

#include "ui_GeocodeRangeDialog.h"

class GeocodeRangeDialog : public QDialog, public Ui::GeocodeRangeDialog
{
Q_OBJECT

public:
    GeocodeRangeDialog( QWidget *parent = 0 );
};

#endif
