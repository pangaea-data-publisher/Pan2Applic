#ifndef TEXTOPTIONSDIALOG_H
#define TEXTOPTIONSDIALOG_H

#include <QDialog>

#include "ui_TextOptionsDialog.h"

class TextOptionsDialog : public QDialog, public Ui::TextOptionsDialog
{
Q_OBJECT

public:
    TextOptionsDialog( QWidget *parent = 0 );
    QString getPersonalDir();

private slots:
    void browseFilenameDialog();
};

#endif
