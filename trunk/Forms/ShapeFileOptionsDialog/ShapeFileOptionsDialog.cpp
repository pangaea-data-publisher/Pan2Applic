// ***********************************************************************************************
// *                                                                                             *
// * ShapeFileOptionsDialog.cpp - Set the options for the Shape file mode                        *
// *                                                                                             *
// * Dr. Rainer Sieger - 2009-03-23                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtWidgets>

#include "Application.h"
#include "ShapeFileOptionsDialog.h"

ShapeFileOptionsDialog::ShapeFileOptionsDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    connect(OK_pushButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(Cancel_pushButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(BrowseShapefileFilename_pushButton, SIGNAL(clicked()), this, SLOT(browseFilenameDialog()));
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

int MainWindow::doShapeFileOptionsDialog()
{
    int err = _ERROR_;

    QFileInfo fi( gs_FilenameShapefile );
    QDir di( fi.absolutePath() );

    if ( di.exists() == false )
        gs_FilenameShapefile = getDocumentDir() + "/zz_" + fi.baseName() + ".shp";

    ShapeFileOptionsDialog dialog( this );

    dialog.ShowShortName_checkBox->setChecked( gb_showShortName );
    dialog.ShowMethod_checkBox->setChecked( gb_showMethod );
    dialog.ShowComment_checkBox->setChecked( gb_showComment );
    dialog.setGeocodeRange_checkBox->setChecked( gb_setGeocode );
    dialog.setGearID_checkBox->setChecked( gb_setGearID );

    dialog.CodecInput_ComboBox->setCurrentIndex( gi_CodecInput );

    switch ( gi_DateTimeFormat )
    {
        case _BUILDDATE:
            dialog.ShapeFile_Day_radioButton->setChecked( true );
            break;
        case _BUILDDATETIME:
            dialog.ShapeFile_ISO_radioButton->setChecked( true );
            break;
        default:
            dialog.ShapeFile_realISO_radioButton->setChecked( true );
    }

    dialog.ShapefileFilename_lineEdit->setText( QDir::toNativeSeparators( gs_FilenameShapefile ) );

    dialog.OK_pushButton->setWhatsThis( "Close dialog" );
    dialog.Cancel_pushButton->setWhatsThis( "Cancel dialog" );
    dialog.BrowseShapefileFilename_pushButton->setWhatsThis( "Browse for the shapefile file" );

    dialog.move( posDialog );
    dialog.resize( dialog.sizeHint() );
    dialog.show();

// ******************************************************************************************************

    switch ( dialog.exec() )
    {
    case QDialog::Accepted:
        gs_FilenameShapefile = dialog.ShapefileFilename_lineEdit->text();

        gb_showShortName     = dialog.ShowShortName_checkBox->isChecked();
        gb_showMethod        = dialog.ShowMethod_checkBox->isChecked();
        gb_showComment       = dialog.ShowComment_checkBox->isChecked();
        gb_setGeocode        = dialog.setGeocodeRange_checkBox->isChecked();
        gb_setGearID         = dialog.setGearID_checkBox->isChecked();

        gi_CodecInput        = dialog.CodecInput_ComboBox->currentIndex();

        if ( dialog.ShapeFile_Day_radioButton->isChecked() == true )
            gi_DateTimeFormat = _BUILDDATE;

        if ( dialog.ShapeFile_ISO_radioButton->isChecked() == true )
            gi_DateTimeFormat = _BUILDDATETIME;

        if ( dialog.ShapeFile_realISO_radioButton->isChecked() == true )
            gi_DateTimeFormat = _BUILDISODATETIME;

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

void ShapeFileOptionsDialog::browseFilenameDialog()
{
    QString	fn			= "";
    QString file		= this->ShapefileFilename_lineEdit->text();
    QString fileStart	= getDocumentDir();

// ***********************************************************************************************************************

    fileStart.replace( "://", ":/" );

    QFileInfo fi( file );
    QDir di( fi.absolutePath() );

    if ( ( file.isEmpty() == false ) && ( di.exists() == true ) )
        fileStart = file;

    #if defined(Q_OS_WIN)
        fn = QFileDialog::getSaveFileName( this, tr( "Select the output file (*.shp)" ), fileStart, tr( "SHP file (*.shp)" ), 0, QFileDialog::DontUseNativeDialog );
    #endif

    #if defined(Q_OS_MAC)
        fn = QFileDialog::getSaveFileName( this, tr( "Select the output file (*.shp)" ), fileStart, tr( "SHP file (*.shp)" ), 0, QFileDialog::DontUseNativeDialog );
    #endif

    #if defined(Q_OS_LINUX)
        fn = QFileDialog::getSaveFileName( this, tr( "Select the output file (*.shp)" ), fileStart, tr( "SHP file (*.shp)" ), 0, QFileDialog::DontUseNativeDialog );
    #endif

    if ( fn.isEmpty() == true )
        fn = file;

    this->ShapefileFilename_lineEdit->setText( QDir::toNativeSeparators( fn ) );

    ShapefileFilename_lineEdit->setFocus();
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

QString ShapeFileOptionsDialog::getDocumentDir()
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
