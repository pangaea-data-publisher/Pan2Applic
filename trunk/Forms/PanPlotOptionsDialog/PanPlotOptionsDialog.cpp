// ***********************************************************************************************
// *                                                                                             *
// * PanPlotOptionsDialog.cpp - Select the PanPlot program                                       *
// *                                                                                             *
// * Dr. Rainer Sieger - 2009-03-23                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtGui>

#include "Application.h"
#include "Globals.h"
#include "PanPlotOptionsDialog.h"

PanPlotOptionsDialog::PanPlotOptionsDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    connect(OK_pushButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(Cancel_pushButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(BrowsePanPlotProgram_pushButton, SIGNAL(clicked()), this, SLOT(browseFilenameDialog()));
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

int MainWindow::doPanPlotOptionsDialog()
{
    int i_DialogResult = ERROR;

// ***********************************************************************************************************************

    PanPlotOptionsDialog *dialog = new PanPlotOptionsDialog();

    dialog->PanPlotProgram_lineEdit->setText( QDir::toNativeSeparators( gs_FilenamePanPlotProgram ) );
    dialog->StartPanPlot_checkBox->setChecked( gb_startPanPlot );

    dialog->OK_pushButton->setWhatsThis( "Close dialog" );
    dialog->Cancel_pushButton->setWhatsThis( "Cancel dialog" );

    dialog->BrowsePanPlotProgram_pushButton->setWhatsThis( "Browse for the PanPlot program file" );

    dialog->PanPlotProgram_lineEdit->selectAll();

    dialog->move( posDialog );
    dialog->resize( dialog->sizeHint() );

// ******************************************************************************************************

    if ( dialog->exec() == QDialog::Accepted )
    {
        gs_FilenamePanPlotProgram = dialog->PanPlotProgram_lineEdit->text();
        gb_startPanPlot           = dialog->StartPanPlot_checkBox->isChecked();

        i_DialogResult = NOERROR;
    }

    posDialog = dialog->pos();

    delete dialog;

    return( i_DialogResult );
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

void PanPlotOptionsDialog::browseFilenameDialog()
{
    int		i_lenght	= 0;

    QString	fn			= "";
    QString file		= this->PanPlotProgram_lineEdit->text();
    QString fileStart	= getProgramDir();

// ***********************************************************************************************************************

    QFileInfo fi( file );

    if ( ( file != "PanPlot" ) && ( file.isEmpty() == FALSE ) )
        fileStart = file;

    #if defined(Q_WS_WIN)
        fn = QFileDialog::getOpenFileName( this, tr( "Select the PanPlot program (*.exe)" ), fileStart, tr( "EXE file (*.exe)" ) );
    #endif

    #if defined(Q_WS_MACX)
        fn = QFileDialog::getOpenFileName( this, tr( "Select the PanPlot program (*.app)" ), fileStart, tr( "APP file (*.app)" ) );
    #endif

    #if defined(Q_WS_X11)
        fn = QFileDialog::getExistingDirectory( this, tr( "Select the PanPlot program" ), fileStart, QFileDialog::DontUseNativeDialog );
    #endif

    if ( fn.isEmpty() == TRUE )
        fn = file;

    i_lenght = fn.length();

    if ( i_lenght < 30 )
        i_lenght = 30;

    this->PanPlotProgram_lineEdit->setText( QDir::toNativeSeparators( fn ) );
    this->PanPlotProgram_lineEdit->setFixedWidth( i_lenght * fontMetrics().width( 'x' ) );
    this->resize( QSize( 1, 1 ).expandedTo( this->minimumSizeHint() ) );

    PanPlotProgram_lineEdit->setFocus();
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

QString PanPlotOptionsDialog::getProgramDir()
{
    QString     s_ProgramDir   = "";

    #if defined(Q_WS_WIN)
        int         i              = 0;
        QStringList sl_Environment = QProcess::systemEnvironment();
        while ( ( s_ProgramDir.isEmpty() == TRUE ) && ( ++i < sl_Environment.count() ) )
            if ( sl_Environment.at( i ).startsWith( "PROGRAMFILES=", Qt::CaseInsensitive ) == TRUE )
                s_ProgramDir = sl_Environment.at( i ).section( "=", 1, 1 );
    #endif

    #if defined(Q_WS_MACX)
        s_ProgramDir = "/Applications";
    #endif

    return( QDir::toNativeSeparators( s_ProgramDir ) );
}
