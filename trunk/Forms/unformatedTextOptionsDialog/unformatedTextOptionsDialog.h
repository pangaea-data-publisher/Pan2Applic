#ifndef UNFORMATEDTEXTOPTIONSDIALOG_H
#define UNFORMATEDTEXTOPTIONSDIALOG_H

#include <QDialog>

#include "ui_unformatedtextoptionsdialog.h"

class unformatedTextOptionsDialog : public QDialog, public Ui::unformatedTextOptionsDialog
{
Q_OBJECT

public:
    unformatedTextOptionsDialog( QWidget *parent = 0 );
    QString getDocumentDir();

private slots:
    void browseFilenameDialog();
};

#endif
