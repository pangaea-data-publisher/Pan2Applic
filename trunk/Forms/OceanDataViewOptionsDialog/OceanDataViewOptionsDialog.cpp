// ***********************************************************************************************
// *                                                                                             *
// * OceanDataViewOptionsDialog.cpp - Select the Ocean Data View program                         *
// *                                                                                             *
// * Dr. Rainer Sieger - 2009-03-23                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtWidgets>

#include "Application.h"
#include "OceanDataViewOptionsDialog.h"

OceanDataViewOptionsDialog::OceanDataViewOptionsDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    connect(OK_pushButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(Cancel_pushButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(BrowseOceanDataViewProgram_pushButton, SIGNAL(clicked()), this, SLOT(browseFilenameDialog()));
    connect(BrowseOceanDataViewFilename_pushButton, SIGNAL(clicked()), this, SLOT(browseOutputFilenameDialog()));
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

int MainWindow::doOceanDataViewOptionsDialog()
{
    int i_DialogResult = QDialog::Rejected;

    OceanDataViewOptionsDialog dialog( this );

    if ( gs_FilenameOceanDataViewProgram.startsWith( "Browse through the Ocean Data View program file with" ) == true )
        gb_startOceanDataView = false;

    dialog.OceanDataViewFilename_lineEdit->setText( QDir::toNativeSeparators( gs_FilenameOceanDataView ) );
    dialog.OceanDataViewProgram_lineEdit->setText( QDir::toNativeSeparators( gs_FilenameOceanDataViewProgram ) );
    dialog.StartOceanDataView_checkBox->setChecked( gb_startOceanDataView );

    dialog.CodecInput_ComboBox->setCurrentIndex( gi_CodecInput );

    switch ( gi_DeviceTypeOceanDataView )
    {
        case _DEVICETYPEB:
            dialog.ODV_DeviceTypeB_radioButton->setChecked( true );
            break;
        case _DEVICETYPEC:
            dialog.ODV_DeviceTypeC_radioButton->setChecked( true );
            break;
        default:
            dialog.ODV_DeviceTypeAuto_radioButton->setChecked( true );
            break;
    }

    dialog.ShowShortName_checkBox->setChecked( gb_showShortName );
    dialog.ShowMethod_checkBox->setChecked( gb_showMethod );
    dialog.ShowComment_checkBox->setChecked( gb_showComment );

    dialog.setGeocodeRange_checkBox->setChecked( gb_setGeocode );
    dialog.setGearID_checkBox->setChecked( gb_setGearID );
    dialog.addLineNo_checkBox->setChecked( gb_addLineNo );
    dialog.usePANGAEAQualityFlags_checkBox->setChecked( gb_usePANGAEAQualityFlags );

    dialog.OK_pushButton->setWhatsThis( "Close dialog" );
    dialog.Cancel_pushButton->setWhatsThis( "Cancel dialog" );
    dialog.BrowseOceanDataViewProgram_pushButton->setWhatsThis( "Browse through the Ocean Data View program file" );
    dialog.BrowseOceanDataViewFilename_pushButton->setWhatsThis( "Browse through the ODV output file" );

    dialog.OceanDataViewProgram_lineEdit->selectAll();

    dialog.move( posDialog );
    dialog.resize( dialog.sizeHint() );
    dialog.show();

// ******************************************************************************************************

    switch ( dialog.exec() )
    {
    case QDialog::Accepted:
        gs_FilenameOceanDataViewProgram = dialog.OceanDataViewProgram_lineEdit->text();
        gs_FilenameOceanDataView        = dialog.OceanDataViewFilename_lineEdit->text();

        if ( dialog.ODV_DeviceTypeB_radioButton->isChecked() == true )
           gi_DeviceTypeOceanDataView = _DEVICETYPEB;

        if ( dialog.ODV_DeviceTypeC_radioButton->isChecked() == true )
           gi_DeviceTypeOceanDataView = _DEVICETYPEC;

        if ( dialog.ODV_DeviceTypeAuto_radioButton->isChecked() == true )
           gi_DeviceTypeOceanDataView = _DEVICETYPEAUTO;

        gb_showShortName               = dialog.ShowShortName_checkBox->isChecked();
        gb_showMethod                  = dialog.ShowMethod_checkBox->isChecked();
        gb_showComment                 = dialog.ShowComment_checkBox->isChecked();

        gb_setGeocode                  = dialog.setGeocodeRange_checkBox->isChecked();
        gb_setGearID                   = dialog.setGearID_checkBox->isChecked();
        gb_addLineNo                   = dialog.addLineNo_checkBox->isChecked();
        gb_usePANGAEAQualityFlags      = dialog.usePANGAEAQualityFlags_checkBox->isChecked();
        gb_startOceanDataView          = dialog.StartOceanDataView_checkBox->isChecked();

        gi_CodecInput                  = dialog.CodecInput_ComboBox->currentIndex();

        i_DialogResult = QDialog::Accepted;
        break;

    case QDialog::Rejected:
        i_DialogResult = QDialog::Rejected;
        break;

    default:
        i_DialogResult = QDialog::Rejected;
        break;
    }

    posDialog = dialog.pos();

    return( i_DialogResult );
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

void OceanDataViewOptionsDialog::browseFilenameDialog()
{
    QString	fn			= "";
    QString file		= this->OceanDataViewProgram_lineEdit->text();
    QString fileStart	= getProgramDir();

// ***********************************************************************************************************************

    fileStart.replace( "://", ":/" );

    if ( ( file.startsWith( "Browse" ) == false ) && ( file.isEmpty() == false ) )
        fileStart = file;

    #if defined(Q_OS_WIN)
        fn = QFileDialog::getOpenFileName( this, tr( "Select the Ocean Data view program" ), fileStart, tr( "EXE file (*.exe)" ), 0, QFileDialog::DontUseNativeDialog );
    #endif

    #if defined(Q_OS_LINUX)
        fn = QFileDialog::getOpenFileName( this, tr( "Select the Ocean Data view program" ), fileStart, tr( "All (*)" ), 0, QFileDialog::DontUseNativeDialog );
    #endif

    #if defined(Q_OS_MAC)
        fn = QFileDialog::getExistingDirectory( this, tr( "Select the Ocean Data view program" ), fileStart, QFileDialog::DontUseNativeDialog );
    #endif

    if ( fn.isEmpty() == true )
        fn = file;

    if ( ( fn.contains( "odv4" ) == true ) || ( fn.contains( "Ocean Data View (64bit)" ) == true ) || ( fn.contains( "Ocean Data View (32bit)" ) == true ) )
    {
        this->OceanDataViewProgram_lineEdit->setText( QDir::toNativeSeparators( fn ) );
    }
    else
    {
        QMessageBox::warning( this, QCoreApplication::applicationName(), QCoreApplication::applicationName() + " supports Ocean Data View 4.x only!\nPlease choose odv4 (http://odv.awi.de)." );
        this->OceanDataViewProgram_lineEdit->setText ( tr( "Browse through the Ocean Data View program file with \"Browse ...\"" ) );
    }


    OceanDataViewProgram_lineEdit->setFocus();
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

void OceanDataViewOptionsDialog::browseOutputFilenameDialog()
{
    QString	fn			= "";
    QString file		= this->OceanDataViewFilename_lineEdit->text();
    QString fileStart	= getDocumentDir();

// ***********************************************************************************************************************

    fileStart.replace( "://", ":/" );

    QFileInfo fi( file );
    QDir di( fi.absolutePath() );

    if ( ( file.isEmpty() == false ) && ( di.exists() == true ) )
        fileStart = file;

    #if defined(Q_OS_WIN)
        fn = QFileDialog::getSaveFileName( this, tr( "Select the output file (*.txt)" ), fileStart, tr( "Text file (*.txt)" ), 0, QFileDialog::DontUseNativeDialog );
    #endif

    #if defined(Q_OS_LINUX)
        fn = QFileDialog::getSaveFileName( this, tr( "Select the output file (*.txt)" ), fileStart, tr( "Text file (*.txt)" ), 0, QFileDialog::DontUseNativeDialog );
    #endif

    #if defined(Q_OS_MAC)
        fn = QFileDialog::getSaveFileName( this, tr( "Select the output file (*.txt)" ), fileStart, tr( "Text file (*.txt)" ), 0, QFileDialog::DontUseNativeDialog );
    #endif

    if ( fn.isEmpty() == true )
        fn = file;

    this->OceanDataViewFilename_lineEdit->setText( QDir::toNativeSeparators( fn ) );

    OceanDataViewFilename_lineEdit->setFocus();
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

QString OceanDataViewOptionsDialog::getProgramDir()
{
    #if defined(Q_OS_LINUX)
        return( QDir::homePath() );
    #endif

    #if defined(Q_OS_MAC)
        return( "/Applications" );
    #endif

    #if defined(Q_OS_WIN)
        return( QDir::rootPath() + QLatin1String( "Program Files (x86)") );
    #endif
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

QString OceanDataViewOptionsDialog::getDocumentDir()
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
