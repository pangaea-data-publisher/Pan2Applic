#ifndef PANMAPOPTIONSDIALOG_H
#define PANMAPOPTIONSDIALOG_H

#include <QDialog>

#include "ui_PanMapOptionsDialog.h"

class PanMapOptionsDialog : public QDialog, public Ui::PanMapOptionsDialog
{
Q_OBJECT

public:
    PanMapOptionsDialog( QWidget *parent = 0 );
};

#endif
