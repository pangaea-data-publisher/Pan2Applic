// ***********************************************************************************************
// *                                                                                             *
// * SetCampaignNameDialog.cpp - Sets the campaign name                                          *
// *                                                                                             *
// * Dr. Rainer Sieger - 2009-03-24                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtGui>

#include "Application.h"
#include "Globals.h"
#include "SetCampaignNameDialog.h"

SetCampaignNameDialog::SetCampaignNameDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    connect(OK_pushButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(Cancel_pushButton, SIGNAL(clicked()), this, SLOT(reject()));
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

int MainWindow::doSetCampaignNameDialog()
{
    int i_DialogResult = ERROR;

// ***********************************************************************************************************************

    SetCampaignNameDialog *dialog = new SetCampaignNameDialog();

    dialog->CampaignName_lineEdit->setText( QDir::toNativeSeparators( gs_CampaignName ) );
    dialog->SetAllCampaigns_checkBox->setChecked( gb_setAllCampaigns );

    dialog->OK_pushButton->setWhatsThis( "Close dialog" );
    dialog->Cancel_pushButton->setWhatsThis( "Cancel dialog" );

    dialog->CampaignName_lineEdit->selectAll();

    dialog->move( posDialog );
    dialog->resize( dialog->sizeHint() );

// ******************************************************************************************************

    if ( dialog->exec() == QDialog::Accepted )
    {
        gs_CampaignName    = dialog->CampaignName_lineEdit->text();
        gb_setAllCampaigns = dialog->SetAllCampaigns_checkBox->isChecked();

        i_DialogResult    = QDialog::Accepted;
    }

    posDialog = dialog->pos();

    delete dialog;

    return( i_DialogResult );
}
