// ***********************************************************************************************
// *                                                                                             *
// * ACSIIOptionsDialog.cpp - Set the options for the ASCII mode                                 *
// *                                                                                             *
// * Dr. Rainer Sieger - 2009-03-23                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtGui>

#include "Application.h"
#include "Globals.h"
#include "ASCIIOptionsDialog.h"

ASCIIOptionsDialog::ASCIIOptionsDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    connect(OK_pushButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(Cancel_pushButton, SIGNAL(clicked()), this, SLOT(reject()));
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

void MainWindow::doASCIIOptionsDialog()
{
    i_DialogResult = 0;

    ASCIIOptionsDialog *dialog = new ASCIIOptionsDialog();

    switch ( i_DateTimeFormatTextFile )
    {
        case BUILDDATE:
            dialog->ASCII_Day_radioButton->setChecked( TRUE );
            break;
        case BUILDDATETIME:
            dialog->ASCII_ISO_radioButton->setChecked( TRUE );
            break;
        default:
            dialog->ASCII_realISO_radioButton->setChecked( TRUE );
    }

    dialog->OK_pushButton->setWhatsThis( "Close dialog" );
    dialog->Cancel_pushButton->setWhatsThis( "Cancel dialog" );

    dialog->move( posDialog );

// ******************************************************************************************************

	if ( dialog->exec() == QDialog::Accepted )
	{
        if ( dialog->ASCII_Day_radioButton->isChecked() == TRUE )
            i_DateTimeFormatTextFile = BUILDDATE;
        if ( dialog->ASCII_ISO_radioButton->isChecked() == TRUE )
            i_DateTimeFormatTextFile = BUILDDATETIME;
        if ( dialog->ASCII_realISO_radioButton->isChecked() == TRUE )
            i_DateTimeFormatTextFile = BUILDISODATETIME;

        i_DialogResult = dialog->result();
	}

    posDialog = dialog->pos();

    delete dialog;
}
