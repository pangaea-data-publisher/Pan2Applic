// ***********************************************************************************************
// *                                                                                             *
// * SetEncodingDialog.cpp - Sets encoding of datasets                                           *
// *                                                                                             *
// * Dr. Rainer Sieger - 2012-02-16                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtGui>

#include "Application.h"
#include "Globals.h"
#include "SetEncodingDialog.h"

SetEncodingDialog::SetEncodingDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    connect(OK_pushButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(Cancel_pushButton, SIGNAL(clicked()), this, SLOT(reject()));
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

void MainWindow::doSetEncodingDialog()
{
    SetEncodingDialog *dialog = new SetEncodingDialog();

    dialog->CodecInput_ComboBox->setCurrentIndex( gi_CodecInput );
    dialog->CodecOutput_ComboBox->setCurrentIndex( gi_CodecOutput );
    dialog->EOL_ComboBox->setCurrentIndex( gi_EOL );

    dialog->OK_pushButton->setWhatsThis( "Close dialog" );
    dialog->Cancel_pushButton->setWhatsThis( "Cancel dialog" );

    dialog->move( posDialog );
    dialog->resize( dialog->sizeHint() );

// ******************************************************************************************************

    if ( dialog->exec() == QDialog::Accepted )
    {
        gi_CodecInput  = dialog->CodecInput_ComboBox->currentIndex();
        gi_CodecOutput = dialog->CodecOutput_ComboBox->currentIndex();
        gi_EOL         = dialog->EOL_ComboBox->currentIndex();
    }

    posDialog = dialog->pos();

    delete dialog;
}
