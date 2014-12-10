// ***********************************************************************************************
// *                                                                                             *
// * unformatedTextOptionsDialog.cpp - Set the options for the Text mode                         *
// *                                                                                             *
// * Dr. Rainer Sieger - 2009-08-16                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtWidgets>

#include "Application.h"
#include "unformatedTextOptionsDialog.h"

unformatedTextOptionsDialog::unformatedTextOptionsDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    connect(OK_pushButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(Cancel_pushButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(BrowseTextFilename_pushButton, SIGNAL(clicked()), this, SLOT(browseFilenameDialog()));
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

int MainWindow::doUnformatedTextOptionsDialog()
{
    int err = _ERROR_;

    QFileInfo fi( gs_FilenameText );
    QDir di( fi.absolutePath() );

    if ( di.exists() == false )
        gs_FilenameText = getDocumentDir() + "/zz_" + fi.baseName() + ".txt";

    unformatedTextOptionsDialog dialog( this );

    dialog.ShowShortName_checkBox->setChecked( gb_showShortName );
    dialog.ShowMethod_checkBox->setChecked( gb_showMethod );
    dialog.ShowComment_checkBox->setChecked( gb_showComment );
    dialog.setGeocodeRange_checkBox->setChecked( gb_setGeocode );
    dialog.setGearID_checkBox->setChecked( gb_setGearID );

    dialog.CodecInput_ComboBox->setCurrentIndex( gi_CodecInput );
    dialog.CodecOutput_ComboBox->setCurrentIndex( gi_CodecOutput );
    dialog.EOL_ComboBox->setCurrentIndex( gi_EOL );

    switch ( gi_DateTimeFormat )
    {
        case _BUILDDATE:
            dialog.Day_radioButton->setChecked( true );
            break;
        case _BUILDDATETIME:
            dialog.ISO_radioButton->setChecked( true );
            break;
        default:
            dialog.realISO_radioButton->setChecked( true );
            break;
    }

    switch ( gi_FieldDelimiter )
    {
        case _COMMA_:
            dialog.comma_radioButton->setChecked( true );
            break;
        case _SEMICOLON_:
            dialog.semicolon_radioButton->setChecked( true );
            break;
        case _SPACE_:
            dialog.space_radioButton->setChecked( true );
            break;
        default:
            dialog.tabstop_radioButton->setChecked( true );
            break;
    }

    dialog.missingValue_lineEdit->setText( gs_MissingValue );
    dialog.TextFilename_lineEdit->setText( QDir::toNativeSeparators( gs_FilenameText ) );

    dialog.missingValue_lineEdit->selectAll();

    dialog.OK_pushButton->setWhatsThis( "Close dialog" );
    dialog.Cancel_pushButton->setWhatsThis( "Cancel dialog" );
    dialog.BrowseTextFilename_pushButton->setWhatsThis( "Browse for the text file" );

    dialog.move( posDialog );
    dialog.resize( dialog.sizeHint() );
    dialog.show();

// ******************************************************************************************************

    switch ( dialog.exec() )
    {
    case QDialog::Accepted:
        gs_FilenameText  = dialog.TextFilename_lineEdit->text();

        gb_showShortName = dialog.ShowShortName_checkBox->isChecked();
        gb_showMethod    = dialog.ShowMethod_checkBox->isChecked();
        gb_showComment   = dialog.ShowComment_checkBox->isChecked();
        gb_setGeocode    = dialog.setGeocodeRange_checkBox->isChecked();
        gb_setGearID     = dialog.setGearID_checkBox->isChecked();

        gi_CodecInput    = dialog.CodecInput_ComboBox->currentIndex();
        gi_CodecOutput   = dialog.CodecOutput_ComboBox->currentIndex();
        gi_EOL           = dialog.EOL_ComboBox->currentIndex();

        if ( dialog.Day_radioButton->isChecked() == true )
            gi_DateTimeFormat = _BUILDDATE;

        if ( dialog.ISO_radioButton->isChecked() == true )
            gi_DateTimeFormat = _BUILDDATETIME;

        if ( dialog.realISO_radioButton->isChecked() == true )
            gi_DateTimeFormat = _BUILDISODATETIME;

        gs_MissingValue = dialog.missingValue_lineEdit->text();

        if ( dialog.comma_radioButton->isChecked() == true )
            gi_FieldDelimiter = _COMMA_;

        if ( dialog.semicolon_radioButton->isChecked() == true )
            gi_FieldDelimiter = _SEMICOLON_;

        if ( dialog.space_radioButton->isChecked() == true )
            gi_FieldDelimiter = _SPACE_;

        if ( dialog.tabstop_radioButton->isChecked() == true )
            gi_FieldDelimiter = _TABSTOP_;

        err = _NOERROR_;
        break;

    case QDialog::Rejected:
        break;

    default:
        break;
    }

    posDialog = dialog.pos();

    return( err );
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

void unformatedTextOptionsDialog::browseFilenameDialog()
{
    QString	fn			= "";
    QString file		= this->TextFilename_lineEdit->text();
    QString fileStart	= getDocumentDir();

// ***********************************************************************************************************************

    fileStart.replace( "://", ":/" );

    QFileInfo fi( file );
    QDir di( fi.absolutePath() );

    if ( ( file.isEmpty() == false ) && ( di.exists() == true ) )
        fileStart = file;

    #if defined(Q_OS_WIN)
        fn = QFileDialog::getSaveFileName( this, tr( "Select the output file (*.txt)" ), fileStart, tr( "_TXT_ file (*.txt)" ), 0, QFileDialog::DontUseNativeDialog );
    #endif

    #if defined(Q_OS_MAC)
        fn = QFileDialog::getSaveFileName( this, tr( "Select the output file (*.txt)" ), fileStart, tr( "_TXT_ file (*.txt)" ), 0, QFileDialog::DontUseNativeDialog );
    #endif

    #if defined(Q_OS_LINUX)
        fn = QFileDialog::getSaveFileName( this, tr( "Select the output file (*.txt)" ), fileStart, tr( "_TXT_ file (*.txt)" ), 0, QFileDialog::DontUseNativeDialog );
    #endif

    if ( fn.isEmpty() == true )
        fn = file;

    this->TextFilename_lineEdit->setText( QDir::toNativeSeparators( fn ) );

    TextFilename_lineEdit->setFocus();
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

QString unformatedTextOptionsDialog::getDocumentDir()
{
    #if defined(Q_OS_LINUX)
        return( QDir::homePath() );
    #endif

    #if defined(Q_OS_MAC)
        return( QDir::homePath() );
    #endif

    #if defined(Q_OS_WIN)
        return( QStandardPaths::writableLocation( QStandardPaths::DocumentsLocation ) );
    #endif
}
