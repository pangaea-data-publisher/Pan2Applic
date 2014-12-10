// ***********************************************************************************************
// *                                                                                             *
// * GlobalOptionsDialog.cpp - Set the global options                                            *
// *                                                                                             *
// * Dr. Rainer Sieger - 2009-03-24                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtGui>

#include "Application.h"
#include "Globals.h"
#include "GlobalOptionsDialog.h"

GlobalOptionsDialog::GlobalOptionsDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    connect(OK_pushButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(Cancel_pushButton, SIGNAL(clicked()), this, SLOT(reject()));
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

int MainWindow::doGlobalOptionsDialog()
{
    int i_DialogResult = ERROR;

    GlobalOptionsDialog *dialog = new GlobalOptionsDialog();

    dialog->SetGEOCODE_checkBox->setChecked( gb_setGeocode );
    dialog->SetCampaignName_checkBox->setChecked( gb_setCampaignName );

    dialog->ShowShortName_checkBox->setChecked( gb_showShortName );
    dialog->ShowMethod_checkBox->setChecked( gb_showMethod );
    dialog->ShowComment_checkBox->setChecked( gb_showComment );

    switch ( gi_DateTimeFormat )
    {
        case BUILDDATE:
            dialog->Day_radioButton->setChecked( TRUE );
            break;
        case BUILDDATETIME:
            dialog->ISO_radioButton->setChecked( TRUE );
            break;
        default:
            dialog->realISO_radioButton->setChecked( TRUE );
            break;
    }

    dialog->OK_pushButton->setWhatsThis( "Close dialog" );
    dialog->Cancel_pushButton->setWhatsThis( "Cancel dialog" );

    dialog->move( posDialog );
    dialog->resize( dialog->sizeHint() );

// ******************************************************************************************************

    if ( dialog->exec() == QDialog::Accepted )
    {
//      gb_createDir         = dialog->CreateDirOptions_checkBox->isChecked();
        gb_setGeocode        = dialog->SetGEOCODE_checkBox->isChecked();
        gb_setCampaignName   = dialog->SetCampaignName_checkBox->isChecked();

        gb_showShortName     = dialog->ShowShortName_checkBox->isChecked();
        gb_showMethod        = dialog->ShowMethod_checkBox->isChecked();
        gb_showComment       = dialog->ShowComment_checkBox->isChecked();

        if ( dialog->Day_radioButton->isChecked() == TRUE )
            gi_DateTimeFormat = BUILDDATE;

        if ( dialog->ISO_radioButton->isChecked() == TRUE )
            gi_DateTimeFormat = BUILDDATETIME;

        if ( dialog->realISO_radioButton->isChecked() == TRUE )
            gi_DateTimeFormat = BUILDISODATETIME;

        i_DialogResult = NOERROR;
    }

    posDialog = dialog->pos();

    delete dialog;

    return( i_DialogResult );
}
