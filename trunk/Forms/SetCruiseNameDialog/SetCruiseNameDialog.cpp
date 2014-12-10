// ***********************************************************************************************
// *                                                                                             *
// * SetCruiseNameDialog.cpp - Sets the cruise name                                              *
// *                                                                                             *
// * Dr. Rainer Sieger - 2009-03-24                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtGui>

#include "Application.h"
#include "Globals.h"
#include "SetCruiseNameDialog.h"

SetCruiseNameDialog::SetCruiseNameDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    connect(OK_pushButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(Cancel_pushButton, SIGNAL(clicked()), this, SLOT(reject()));
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

void MainWindow::doSetCruiseNameDialog()
{
    int i_length = s_CruiseName.length();

    if ( i_length < 20 )
        i_length = 20;

    i_DialogResult = 0;

    SetCruiseNameDialog *dialog = new SetCruiseNameDialog();

    dialog->CruiseName_lineEdit->setText( QDir::toNativeSeparators( s_CruiseName ) );
    dialog->SetAllCruises_checkBox->setChecked( b_setAllCruises );

    dialog->CruiseName_lineEdit->setFixedWidth( i_length * fontMetrics().width( 'x' ) );
	dialog->resize(QSize( 1, 1 ).expandedTo( dialog->minimumSizeHint() ) );

    dialog->OK_pushButton->setWhatsThis( "Close dialog" );
    dialog->Cancel_pushButton->setWhatsThis( "Cancel dialog" );

    dialog->CruiseName_lineEdit->selectAll();

    dialog->move( posDialog );
    dialog->show();

// ******************************************************************************************************

	if ( dialog->exec() == QDialog::Accepted )
	{
        posDialog = dialog->pos();

        s_CruiseName    = dialog->CruiseName_lineEdit->text();
        b_setAllCruises = dialog->SetAllCruises_checkBox->isChecked();

        i_DialogResult = dialog->result();
	}

	delete dialog;
}
