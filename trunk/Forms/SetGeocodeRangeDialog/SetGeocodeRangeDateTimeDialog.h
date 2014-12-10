#ifndef SETGEOCODERANGEDATETIMEDIALOG_H
#define SETGEOCODERANGEDATETIMEDIALOG_H

#include <QDialog>

#include "ui_setgeocoderangedatetimedialog.h"

class SetGeocodeRangeDateTimeDialog : public QDialog, public Ui::SetGeocodeRangeDateTimeDialog
{
Q_OBJECT

public:
    SetGeocodeRangeDateTimeDialog( QWidget *parent = 0 );
};

#endif
