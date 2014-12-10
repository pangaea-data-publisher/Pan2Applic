// ***********************************************************************************************
// *                                                                                             *
// * formatedTextOptionsDialog.cpp - Set the options for the Text mode                           *
// *                                                                                             *
// * Dr. Rainer Sieger - 2009-08-16                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtWidgets>

#include "Application.h"
#include "formatedTextOptionsDialog.h"

formatedTextOptionsDialog::formatedTextOptionsDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    connect(OK_pushButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(Cancel_pushButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(BrowseTextFilename_pushButton, SIGNAL(clicked()), this, SLOT(browseFilenameDialog()));
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

int MainWindow::doFormatedTextOptionsDialog()
{
    int err = _ERROR_;

    QFileInfo fi( gs_FilenameText );
    QDir di( fi.absolutePath() );

    if ( di.exists() == false )
        gs_FilenameText = getDocumentDir() + "/zz_" + fi.baseName() + ".txt";

    formatedTextOptionsDialog dialog( this );

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

    switch ( gi_FieldAlignment )
    {
        case QTextStream::AlignRight:
            dialog.alignRight_radioButton->setChecked( true );
            break;
        default:
            dialog.alignLeft_radioButton->setChecked( true );
            break;
    }

    dialog.fieldWidth_lineEdit->setText( QString( "%1" ).arg( gi_FieldWidth ) );
    dialog.missingValue_lineEdit->setText( gs_MissingValue );

    dialog.TextFilename_lineEdit->setText( QDir::toNativeSeparators( gs_FilenameText ) );

    dialog.OK_pushButton->setWhatsThis( "Close dialog" );
    dialog.Cancel_pushButton->setWhatsThis( "Cancel dialog" );
    dialog.BrowseTextFilename_pushButton->setWhatsThis( "Browse for the text file" );

    dialog.missingValue_lineEdit->selectAll();

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

        if ( dialog.alignLeft_radioButton->isChecked() == true )
            gi_FieldAlignment = QTextStream::AlignLeft;

        if ( dialog.alignRight_radioButton->isChecked() == true )
            gi_FieldAlignment = QTextStream::AlignRight;

        gi_FieldWidth = (int) dialog.fieldWidth_lineEdit->text().toInt();

        if ( gi_FieldWidth < 0 )
            gi_FieldWidth = 1;

        if ( gi_FieldWidth > 100 )
            gi_FieldWidth = 100;

        gs_MissingValue = dialog.missingValue_lineEdit->text();

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

void formatedTextOptionsDialog::browseFilenameDialog()
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

QString formatedTextOptionsDialog::getDocumentDir()
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
