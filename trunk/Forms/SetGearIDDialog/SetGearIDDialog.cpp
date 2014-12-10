// ***********************************************************************************************
// *                                                                                             *
// * SetGearIDDialog.cpp - Set the options for the Gear ID                                       *
// *                                                                                             *
// * Dr. Rainer Sieger - 2012-07-25                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtWidgets>

#include "Application.h"
#include "SetGearIDDialog.h"

SetGearIDDialog::SetGearIDDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    GearID_lineEdit->setValidator( new QIntValidator( 1, 100, GearID_lineEdit ) );

    connect(OK_pushButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(Cancel_pushButton, SIGNAL(clicked()), this, SLOT(reject()));
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

int MainWindow::setGearIDDialog()
{
    int err = _ERROR_;

    SetGearIDDialog dialog( this );

    dialog.GearID_lineEdit->setText( QString( "%1" ).arg( gi_GearID ) );

    dialog.OK_pushButton->setWhatsThis( "Close dialog" );
    dialog.Cancel_pushButton->setWhatsThis( "Cancel dialog" );

    dialog.move( posDialog );
    dialog.resize( dialog.sizeHint() );
    dialog.show();

// ******************************************************************************************************

    switch ( dialog.exec() )
    {
    case QDialog::Accepted:
        gi_GearID = dialog.GearID_lineEdit->text().toInt();

        err       = _NOERROR_;
        break;

    case QDialog::Rejected:
        break;

    default:
        break;
    }

    posDialog = dialog.pos();

    return( err );
}
