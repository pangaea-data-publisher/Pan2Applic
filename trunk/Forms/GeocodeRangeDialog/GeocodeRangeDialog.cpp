// ***********************************************************************************************
// *                                                                                             *
// * GeocodeRangeDialog.cpp - Set the options for the geocode range                              *
// *                                                                                             *
// * Dr. Rainer Sieger - 2009-03-23                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtGui>

#include "Application.h"
#include "Globals.h"
#include "GeocodeRangeDialog.h"

GeocodeRangeDialog::GeocodeRangeDialog( QWidget *parent ) : QDialog( parent )
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

int MainWindow::doGeocodeRangeDialog()
{
    int i_DialogResult = ERROR;

    GeocodeRangeDialog *dialog = new GeocodeRangeDialog();

    dialog->RangeMinimum_lineEdit->setText( QString( "%1" ).arg( gf_GEOCODE_min, 0, 'f', 1 ) );
    dialog->RangeMaximum_lineEdit->setText( QString( "%1" ).arg( gf_GEOCODE_max, 0, 'f', 1 ) );

    dialog->OK_pushButton->setWhatsThis( "Close dialog" );
    dialog->Cancel_pushButton->setWhatsThis( "Cancel dialog" );

    dialog->move( posDialog );

// ******************************************************************************************************

    if ( dialog->exec() == QDialog::Accepted )
    {
        gf_GEOCODE_min = dialog->RangeMinimum_lineEdit->text().toFloat();
        gf_GEOCODE_max = dialog->RangeMaximum_lineEdit->text().toFloat();

        if ( gf_GEOCODE_min > gf_GEOCODE_max )
        {
            float D = gf_GEOCODE_max;
            gf_GEOCODE_max = gf_GEOCODE_min;
            gf_GEOCODE_min = D;
        }

        i_DialogResult = NOERROR;
    }

    posDialog = dialog->pos();

    delete dialog;

    return( i_DialogResult );
}
