// ***********************************************************************************************
// *                                                                                             *
// * TextOptionsDialog.cpp - Set the options for the Text mode                                 *
// *                                                                                             *
// * Dr. Rainer Sieger - 2009-07-23                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtGui>

#include "Application.h"
#include "Globals.h"
#include "TextOptionsDialog.h"

TextOptionsDialog::TextOptionsDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    connect(OK_pushButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(Cancel_pushButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(BrowseTextFilename_pushButton, SIGNAL(clicked()), this, SLOT(browseFilenameDialog()));
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

int MainWindow::doTextOptionsDialog()
{
    int i_DialogResult = ERROR;

// ***********************************************************************************************************************

    QFileInfo fi( s_FilenameText );
    QDir di( fi.absolutePath() );

    if ( di.exists() == FALSE )
        s_FilenameText = getPersonalDir() + "/zz_" + fi.baseName() + ".txt";

    TextOptionsDialog *dialog = new TextOptionsDialog();

    dialog->ShowShortName_checkBox->setChecked( b_showShortName );
    dialog->ShowMethod_checkBox->setChecked( b_showMethod );
    dialog->ShowComment_checkBox->setChecked( b_showComment );

    switch ( i_DateTimeFormat )
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

    dialog->TextFilename_lineEdit->setText( QDir::toNativeSeparators( s_FilenameText ) );
    dialog->AskBeforeBuild_checkBox->setChecked( b_askBeforeBuildText );

    dialog->OK_pushButton->setWhatsThis( "Close dialog" );
    dialog->Cancel_pushButton->setWhatsThis( "Cancel dialog" );

    dialog->BrowseTextFilename_pushButton->setWhatsThis( "Browse for the text file" );

    dialog->TextFilename_lineEdit->selectAll();

    dialog->move( posDialog );

// ******************************************************************************************************

    if ( dialog->exec() == QDialog::Accepted )
    {
        s_FilenameText  = dialog->TextFilename_lineEdit->text();

        b_showShortName = dialog->ShowShortName_checkBox->isChecked();
        b_showMethod    = dialog->ShowMethod_checkBox->isChecked();
        b_showComment   = dialog->ShowComment_checkBox->isChecked();

        if ( dialog->Day_radioButton->isChecked() == TRUE )
            i_DateTimeFormat = BUILDDATE;

        if ( dialog->ISO_radioButton->isChecked() == TRUE )
            i_DateTimeFormat = BUILDDATETIME;

        if ( dialog->realISO_radioButton->isChecked() == TRUE )
            i_DateTimeFormat = BUILDISODATETIME;

        b_askBeforeBuildText = dialog->AskBeforeBuild_checkBox->isChecked();

        i_DialogResult = NOERROR;
    }

    posDialog = dialog->pos();

    delete dialog;

    return( i_DialogResult );
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

void TextOptionsDialog::browseFilenameDialog()
{
    QString	fn			= "";
    QString file		= this->TextFilename_lineEdit->text();
    QString fileStart	= getPersonalDir();

// ***********************************************************************************************************************

    fileStart.replace( "://", ":/" );

    QFileInfo fi( file );
    QDir di( fi.absolutePath() );

    if ( ( file.isEmpty() == FALSE ) && ( di.exists() == TRUE ) )
        fileStart = file;

    #if defined(Q_WS_WIN)
        fn = QFileDialog::getSaveFileName( this, tr( "Select the output file (*.txt)" ), fileStart, tr( "TXT file (*.txt)" ) );
    #endif

    #if defined(Q_WS_MACX)
        fn = QFileDialog::getSaveFileName( this, tr( "Select the output file (*.txt)" ), fileStart, tr( "TXT file (*.txt)" ) );
    #endif

    #if defined(Q_WS_X11)
        fn = QFileDialog::getSaveFileName( this, tr( "Select the output file (*.txt)" ), fileStart, tr( "TXT file (*.txt)" ) );
    #endif

    if ( fn.isEmpty() == TRUE )
        fn = file;

    this->TextFilename_lineEdit->setText( QDir::toNativeSeparators( fn ) );

    TextFilename_lineEdit->setFocus();
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

QString TextOptionsDialog::getPersonalDir()
{
    QString s_PersonalDir	= QDir::homePath();
    QString s_HomeDir		= QDir::homePath();

    #if defined(Q_WS_WIN)
        QSettings settings( "Microsoft", "Windows" );
        s_PersonalDir = settings.value( "/CurrentVersion/Explorer/Shell Folders/Personal" ).toString();
    #endif

    #if defined(Q_WS_MACX)
        s_PersonalDir = s_HomeDir;
    #endif

    return( QDir::toNativeSeparators( s_PersonalDir ) );
}
