#ifndef GLOBALOPTIONSDIALOG_H
#define GLOBALOPTIONSDIALOG_H

#include <QDialog>

#include "ui_globaloptionsdialog.h"

class GlobalOptionsDialog : public QDialog, public Ui::GlobalOptionsDialog
{
Q_OBJECT

public:
    GlobalOptionsDialog( QWidget *parent = 0 );
};

#endif
