#ifndef SHAPEFILEOPTIONSDIALOG_H
#define SHAPEFILEOPTIONSDIALOG_H

#include <QDialog>

#include "ui_shapefileoptionsdialog.h"

class ShapeFileOptionsDialog : public QDialog, public Ui::ShapeFileOptionsDialog
{
Q_OBJECT

public:
    ShapeFileOptionsDialog( QWidget *parent = 0 );
    QString getDocumentDir();

private slots:
    void browseFilenameDialog();
};

#endif
