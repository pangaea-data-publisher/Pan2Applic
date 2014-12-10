#ifndef SETCRUISENAMEDIALOG_H
#define SETCRUISENAMEDIALOG_H

#include <QDialog>

#include "ui_SetCruiseNameDialog.h"

class SetCruiseNameDialog : public QDialog, public Ui::SetCruiseNameDialog
{
Q_OBJECT

public:
    SetCruiseNameDialog( QWidget *parent = 0 );
};

#endif
