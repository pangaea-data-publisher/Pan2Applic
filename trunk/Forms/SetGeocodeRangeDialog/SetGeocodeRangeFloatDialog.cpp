// ***********************************************************************************************
// *                                                                                             *
// * SetGeocodeRangeFloatDialog.cpp - Set the options for the geocode range                      *
// *                                                                                             *
// * Dr. Rainer Sieger - 2010-11-23                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtWidgets>

#include "Application.h"
#include "SetGeocodeRangeFloatDialog.h"

SetGeocodeRangeFloatDialog::SetGeocodeRangeFloatDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    RangeMinimum_lineEdit->setValidator( new QDoubleValidator( -1E10, 1E10, 5, RangeMinimum_lineEdit ) );
    RangeMaximum_lineEdit->setValidator( new QDoubleValidator( -1E10, 1E10, 5, RangeMaximum_lineEdit ) );

    connect(OK_pushButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(Cancel_pushButton, SIGNAL(clicked()), this, SLOT(reject()));
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

int MainWindow::doSetGeocodeRangeFloatDialog()
{
    int err = _ERROR_;

    SetGeocodeRangeFloatDialog dialog( this );

    dialog.RangeMinimum_lineEdit->setText( QString( "%1" ).arg( gf_GEOCODE_min, 0, 'f', 3 ) );
    dialog.RangeMaximum_lineEdit->setText( QString( "%1" ).arg( gf_GEOCODE_max, 0, 'f', 3 ) );

    dialog.OK_pushButton->setWhatsThis( "Close dialog" );
    dialog.Cancel_pushButton->setWhatsThis( "Cancel dialog" );

    dialog.move( posDialog );
    dialog.resize( dialog.sizeHint() );
    dialog.show();

// ******************************************************************************************************

    switch ( dialog.exec() )
    {
    case QDialog::Accepted:
        if ( dialog.RangeMinimum_lineEdit->text().toFloat() < dialog.RangeMaximum_lineEdit->text().toFloat() )
        {
            gf_GEOCODE_min = dialog.RangeMinimum_lineEdit->text().toFloat();
            gf_GEOCODE_max = dialog.RangeMaximum_lineEdit->text().toFloat();
        }
        else
        {
            gf_GEOCODE_min = dialog.RangeMaximum_lineEdit->text().toFloat();
            gf_GEOCODE_max = dialog.RangeMinimum_lineEdit->text().toFloat();
        }

        err = _NOERROR_;
        break;

    case QDialog::Rejected:
        break;

    default:
        break;
    }

    posDialog = dialog.pos();

    return( err );
}
