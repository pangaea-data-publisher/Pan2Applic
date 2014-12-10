// ***********************************************************************************************
// *                                                                                             *
// * SetDataFormatDialog.cpp                                                                     *
// *                                                                                             *
// * Dr. Rainer Sieger - 2009-11-19                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtGui>

#include "Application.h"
#include "Globals.h"
#include "SetDataFormatDialog.h"

SetDataFormatDialog::SetDataFormatDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    connect(OK_pushButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(Cancel_pushButton, SIGNAL(clicked()), this, SLOT(reject()));
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

int MainWindow::doSetDataFormatDialog( const QStringList& sl_ParameterList, structDataFormat DF[] )
{
    int i_DialogResult = ERROR;

// ***********************************************************************************************************************

    setDefaultFormats( sl_ParameterList, DF );

    return( NOERROR );

// ***********************************************************************************************************************
// not used at the moment, has to be implemented

    SetDataFormatDialog *dialog = new SetDataFormatDialog();

    dialog->OK_pushButton->setWhatsThis( "Close dialog" );
    dialog->Cancel_pushButton->setWhatsThis( "Cancel dialog" );

    dialog->move( posDialog );
    dialog->resize( dialog->sizeHint() );

// ******************************************************************************************************

    if ( dialog->exec() == QDialog::Accepted )
    {
        setDefaultFormats( sl_ParameterList, DF );

        i_DialogResult = NOERROR;
    }

    posDialog = dialog->pos();

    delete dialog;

    return( i_DialogResult );
}
