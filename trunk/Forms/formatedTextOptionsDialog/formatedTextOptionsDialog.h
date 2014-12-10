#ifndef FORMATEDTextOptionsDialog_H
#define FORMATEDTextOptionsDialog_H

#include <QDialog>

#include "ui_formatedtextoptionsdialog.h"

class formatedTextOptionsDialog : public QDialog, public Ui::formatedTextOptionsDialog
{
Q_OBJECT

public:
    formatedTextOptionsDialog( QWidget *parent = 0 );
    QString getDocumentDir();

private slots:
    void browseFilenameDialog();
};

#endif
