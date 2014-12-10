#ifndef SETGEARIDDIALOG_H
#define SETGEARIDDIALOG_H

#include <QDialog>

#include "ui_setgeariddialog.h"

class SetGearIDDialog : public QDialog, public Ui::SetGearIDDialog
{
Q_OBJECT

public:
    SetGearIDDialog( QWidget *parent = 0 );
};

#endif
