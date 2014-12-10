// ***********************************************************************************************
// *                                                                                             *
// * SetGeocodeRangeDateTimeDialog.cpp - Set the options for the geocode range                   *
// *                                                                                             *
// * Dr. Rainer Sieger - 2010-11-23                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtWidgets>

#include "Application.h"
#include "SetGeocodeRangeDateTimeDialog.h"

SetGeocodeRangeDateTimeDialog::SetGeocodeRangeDateTimeDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    RangeMinimum_dateTimeEdit->setDisplayFormat( tr( "yyyy-MM-ddThh:mm" ) );
    RangeMaximum_dateTimeEdit->setDisplayFormat( tr( "yyyy-MM-ddThh:mm" ) );

    connect(OK_pushButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(Cancel_pushButton, SIGNAL(clicked()), this, SLOT(reject()));
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

int MainWindow::doSetGeocodeRangeDateTimeDialog()
{
    int err = _ERROR_;

    SetGeocodeRangeDateTimeDialog dialog( this );

    dialog.RangeMinimum_dateTimeEdit->setDate( QDate::fromString( gs_GEOCODE_min.section( "T", 0, 0 ), tr( "yyyy-MM-dd" ) ) );
    dialog.RangeMinimum_dateTimeEdit->setTime( QTime::fromString( gs_GEOCODE_min.section( "T", 1, 1 ), tr( "hh:mm:ss" ) ) );
    dialog.RangeMaximum_dateTimeEdit->setDate( QDate::fromString( gs_GEOCODE_max.section( "T", 0, 0 ), tr( "yyyy-MM-dd" ) ) );
    dialog.RangeMaximum_dateTimeEdit->setTime( QTime::fromString( gs_GEOCODE_max.section( "T", 1, 1 ), tr( "hh:mm:ss" ) ) );

    dialog.OK_pushButton->setWhatsThis( "Close dialog" );
    dialog.Cancel_pushButton->setWhatsThis( "Cancel dialog" );

    dialog.move( posDialog );
    dialog.resize( dialog.sizeHint() );
    dialog.show();

// ******************************************************************************************************

    switch ( dialog.exec() )
    {
    case QDialog::Accepted:
        if ( dialog.RangeMinimum_dateTimeEdit->dateTime() < dialog.RangeMaximum_dateTimeEdit->dateTime() )
        {
            gs_GEOCODE_min = dialog.RangeMinimum_dateTimeEdit->dateTime().toString( Qt::ISODate );
            gs_GEOCODE_max = dialog.RangeMaximum_dateTimeEdit->dateTime().toString( Qt::ISODate );
        }
        else
        {
            gs_GEOCODE_min = dialog.RangeMaximum_dateTimeEdit->dateTime().toString( Qt::ISODate );
            gs_GEOCODE_max = dialog.RangeMinimum_dateTimeEdit->dateTime().toString( Qt::ISODate );
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
