// ***********************************************************************************************
// *                                                                                             *
// * SetGeocodeRangeDialog.cpp - Set the options for the geocode range                           *
// *                                                                                             *
// * Dr. Rainer Sieger - 2009-03-23                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtGui>

#include "Application.h"
#include "Globals.h"
#include "SetGeocodeRangeDialog.h"

SetGeocodeRangeDialog::SetGeocodeRangeDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    RangeMinimum_lineEdit->setValidator( new QDoubleValidator( -1E10, 1E10, 5, RangeMinimum_lineEdit ) );
    RangeMaximum_lineEdit->setValidator( new QDoubleValidator( -1E10, 1E10, 5, RangeMaximum_lineEdit ) );

    connect(OK_pushButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(Cancel_pushButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(UseGeocodeRange_checkBox, SIGNAL(clicked()), this, SLOT(enableDialogEntries()));
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

void SetGeocodeRangeDialog::enableDialogEntries()
{
    if ( UseGeocodeRange_checkBox->isChecked() == TRUE )
    {
        RangeMinimum_lineEdit->setEnabled( TRUE );
        RangeMaximum_lineEdit->setEnabled( TRUE );
    }
    else
    {
        RangeMinimum_lineEdit->setEnabled( FALSE );
        RangeMaximum_lineEdit->setEnabled( FALSE );
    }
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

int MainWindow::doSetGeocodeRangeDialog()
{
    int i_DialogResult = ERROR;

    SetGeocodeRangeDialog *dialog = new SetGeocodeRangeDialog();

    dialog->UseGeocodeRange_checkBox->setChecked( gb_setGeocode );

    dialog->RangeMinimum_lineEdit->setText( QString( "%1" ).arg( gf_GEOCODE_min, 0, 'f', 3 ) );
    dialog->RangeMaximum_lineEdit->setText( QString( "%1" ).arg( gf_GEOCODE_max, 0, 'f', 3 ) );

    dialog->AskBeforeBuild_checkBox->setChecked( gb_askBeforeBuildGeocodeRange );

    dialog->OK_pushButton->setWhatsThis( "Close dialog" );
    dialog->Cancel_pushButton->setWhatsThis( "Cancel dialog" );

    dialog->move( posDialog );

    dialog->enableDialogEntries();

// ******************************************************************************************************

    if ( dialog->exec() == QDialog::Accepted )
    {
        gb_setGeocode = dialog->UseGeocodeRange_checkBox->isChecked();

        gf_GEOCODE_min = dialog->RangeMinimum_lineEdit->text().toFloat();
        gf_GEOCODE_max = dialog->RangeMaximum_lineEdit->text().toFloat();

        if ( gf_GEOCODE_min > gf_GEOCODE_max )
        {
            float D = gf_GEOCODE_max;
            gf_GEOCODE_max = gf_GEOCODE_min;
            gf_GEOCODE_min = D;
        }

        gb_askBeforeBuildGeocodeRange = dialog->AskBeforeBuild_checkBox->isChecked();

        i_DialogResult = NOERROR;
    }

    posDialog = dialog->pos();

    delete dialog;

    return( i_DialogResult );
}
