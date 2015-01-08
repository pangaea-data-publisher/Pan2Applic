#ifndef SETDATAFORMATDIALOG_H
#define SETDATAFORMATDIALOG_H

#include <QDialog>

#include "ui_setdataformatdialog.h"

class SetDataFormatDialog : public QDialog, public Ui::SetDataFormatDialog
{
Q_OBJECT

public:
    SetDataFormatDialog( QWidget *parent = 0 );
};

#endif // SETDATAFORMATDIALOG_H
