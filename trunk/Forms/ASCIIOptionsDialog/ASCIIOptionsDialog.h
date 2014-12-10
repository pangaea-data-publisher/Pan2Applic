#ifndef ASCIIOPTIONSDIALOG_H
#define ASCIIOPTIONSDIALOG_H

#include <QDialog>

#include "ui_ASCIIOptionsDialog.h"

class ASCIIOptionsDialog : public QDialog, public Ui::ASCIIOptionsDialog
{
Q_OBJECT

public:
    ASCIIOptionsDialog( QWidget *parent = 0 );
};

#endif
