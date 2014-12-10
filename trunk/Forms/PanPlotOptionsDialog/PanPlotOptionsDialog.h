#ifndef PANPLOTOPTIONSDIALOG_H
#define PANPLOTOPTIONSDIALOG_H

#include <QDialog>

#include "ui_panplotoptionsdialog.h"

class PanPlotOptionsDialog : public QDialog, public Ui::PanPlotOptionsDialog
{
Q_OBJECT

public:
    PanPlotOptionsDialog( QWidget *parent = 0 );
    QString getProgramDir();

private slots:
    void browseFilenameDialog();
};

#endif
