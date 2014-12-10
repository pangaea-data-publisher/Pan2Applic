#ifndef SELECTENVIRONMENTDIALOG_H
#define SELECTENVIRONMENTDIALOG_H

#include <QDialog>

#include "ui_selectenvironmentdialog.h"

class SelectEnvironmentDialog : public QDialog, public Ui::SelectEnvironmentDialog
{
Q_OBJECT

public:
    SelectEnvironmentDialog( QWidget *parent = 0 );
};

#endif
