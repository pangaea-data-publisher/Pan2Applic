// ***********************************************************************************************
// *                                                                                             *
// * PanMapOptionsDialog.cpp - Set the options for the PanMap mode                               *
// *                                                                                             *
// * Dr. Rainer Sieger - 2009-03-23                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtGui>

#include "Application.h"
#include "Globals.h"
#include "PanMapOptionsDialog.h"

PanMapOptionsDialog::PanMapOptionsDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    connect(OK_pushButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(Cancel_pushButton, SIGNAL(clicked()), this, SLOT(reject()));
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

void MainWindow::doPanMapOptionsDialog()
{
    i_DialogResult = 0;

    PanMapOptionsDialog *dialog = new PanMapOptionsDialog();

    switch ( i_DateTimeFormatPanMap )
    {
        case BUILDDATE:
            dialog->PanMap_Day_radioButton->setChecked( TRUE );
            break;
        case BUILDDATETIME:
            dialog->PanMap_ISO_radioButton->setChecked( TRUE );
            break;
        default:
            dialog->PanMap_realISO_radioButton->setChecked( TRUE );
    }

    dialog->OK_pushButton->setWhatsThis( "Close dialog" );
    dialog->Cancel_pushButton->setWhatsThis( "Cancel dialog" );

    dialog->move( posDialog );

// ******************************************************************************************************

	if ( dialog->exec() == QDialog::Accepted )
	{
        if ( dialog->PanMap_Day_radioButton->isChecked() == TRUE )
            i_DateTimeFormatPanMap = BUILDDATE;
        if ( dialog->PanMap_ISO_radioButton->isChecked() == TRUE )
            i_DateTimeFormatPanMap = BUILDDATETIME;
        if ( dialog->PanMap_realISO_radioButton->isChecked() == TRUE )
            i_DateTimeFormatPanMap = BUILDISODATETIME;

        i_DialogResult = dialog->result();
	}

    posDialog = dialog->pos();

    delete dialog;
}
