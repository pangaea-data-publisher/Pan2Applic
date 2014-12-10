#ifndef OCEANDATAVIEWOPTIONSDIALOG_H
#define OCEANDATAVIEWOPTIONSDIALOG_H

#include <QDialog>

#include "ui_oceandataviewoptionsdialog.h"

class OceanDataViewOptionsDialog : public QDialog, public Ui::OceanDataViewOptionsDialog
{
Q_OBJECT

public:
    OceanDataViewOptionsDialog( QWidget *parent = 0 );
    QString getProgramDir();
    QString getDocumentDir();

private slots:
    void browseFilenameDialog();
    void browseOutputFilenameDialog();
};

#endif
