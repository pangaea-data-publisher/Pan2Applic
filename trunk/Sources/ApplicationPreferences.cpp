/****************************************************************
**
** Preferences
** 2008-03-21, Dr. Rainer Sieger
** last change: 2009-03-23
**
****************************************************************/

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

/*! @brief Sichern der Programmeinstellungen */

void MainWindow::savePreferences()
{
    #if defined(Q_OS_LINUX)
        QSettings settings( getPreferenceFilename(), QSettings::IniFormat );
    #endif

    #if defined(Q_OS_MAC)
        QSettings settings( getPreferenceFilename(), QSettings::NativeFormat );
    #endif

    #if defined(Q_OS_WIN)
        QSettings settings( getPreferenceFilename(), QSettings::IniFormat );
    #endif

    settings.beginGroup( QCoreApplication::applicationName() );

    settings.beginGroup( "GeneralSettings" );
    settings.setValue( "NumOfProgramStarts", gi_NumOfProgramStarts );

    settings.setValue( "WindowX", x() );
    settings.setValue( "WindowY", y() );
    settings.setValue( "WindowHeight", height() );
    settings.setValue( "WindowWidth", width() );

    settings.setValue( "DialogX", posDialog.x() );
    settings.setValue( "DialogY", posDialog.y() );
    settings.setValue( "ParameterDialogHeight", sizeParameterDialog.height() );
    settings.setValue( "ParameterDialogWidth", sizeParameterDialog.width() );

    settings.setValue( "Path", gs_Path );

    settings.setValue( "CuratorMode", gb_CuratorMode );
    settings.endGroup();

    // Google Earth
    settings.beginGroup( "GoogleEarth" );
    settings.setValue( "FilenameGoogleEarthProgram", gs_FilenameGoogleEarthProgram );
    settings.setValue( "FilenameGoogleEarth", gs_FilenameGoogleEarth );
    settings.setValue( "StartGoogleEarth", gb_startGoogleEarth );
    settings.setValue( "DisplayEventLabel", gb_displayEventLabel );
    settings.setValue( "DisplayDescription", gb_displayDescription );
    settings.setValue( "IconSize", gi_IconSize );
    settings.setValue( "IconColor", gi_IconColor );
    settings.setValue( "TracklineWidth", gi_TracklineWidth );
    settings.setValue( "TracklineColor", gi_TracklineColor );
    settings.endGroup();

    // Ocean Data View
    settings.beginGroup( "OceanDataView" );
    settings.setValue( "FilenameOceanDataViewProgram", gs_FilenameOceanDataViewProgram );
    settings.setValue( "FilenameOceanDataView", gs_FilenameOceanDataView );
    settings.setValue( "StartOceanDataView", gb_startOceanDataView );
    settings.setValue( "UsePANGAEAQualityFlags", gb_usePANGAEAQualityFlags );
    settings.setValue( "DeviceTypeOceanDataView", gi_DeviceTypeOceanDataView );
    settings.setValue( "AddLineNo", gb_addLineNo );
    settings.endGroup();

    // Shapefile
    settings.beginGroup( "Shapefile" );
    settings.setValue( "FilenameShapefile", gs_FilenameShapefile );
    settings.endGroup();

    // Geocode range
    settings.beginGroup( "GeocodeRange" );
    settings.setValue( "GEOCODE_min_f", gf_GEOCODE_min );
    settings.setValue( "GEOCODE_max_f", gf_GEOCODE_max );
    settings.setValue( "GEOCODE_min_dt", gs_GEOCODE_min );
    settings.setValue( "GEOCODE_max_dt", gs_GEOCODE_max );
    settings.endGroup();

    // Global options
    settings.beginGroup( "GlobalOptions" );
    settings.setValue( "CreateDir", gb_createDir );
    settings.setValue( "SetGeocode", gb_setGeocode );
    settings.setValue( "SetGearID", gb_setGearID );
    settings.setValue( "GearID", gi_GearID );
    settings.setValue( "ShowShortName", gb_showShortName );
    settings.setValue( "ShowMethod", gb_showMethod );
    settings.setValue( "ShowComment", gb_showComment );
    settings.setValue( "DateTimeFormat", gi_DateTimeFormat );
    settings.setValue( "SortParameterList", gb_sortParameterList );
    settings.setValue( "CodecInput", gi_CodecInput );
    settings.setValue( "CodecOutput", gi_CodecOutput );
    settings.setValue( "CodecDownload", gi_CodecDownload );
    settings.setValue( "EOL", gi_EOL );
    settings.endGroup();

    // Textfile, field delimiter and missing value
    settings.beginGroup( "TextFile" );
    settings.setValue( "FilenameText", gs_FilenameText );
    settings.setValue( "MissingValue", gs_MissingValue );
    settings.setValue( "FieldDelimiter", gi_FieldDelimiter );
    settings.setValue( "FieldAlignment", gi_FieldAlignment );
    settings.setValue( "FieldWidth", gi_FieldWidth );
    settings.endGroup();

    // PanGet ID
    settings.beginGroup( "PanGet" );
    settings.setValue( "IDListFile", gs_IDListFile );
    settings.setValue( "DownloadDirectory", gs_DownloadDirectory );
    settings.endGroup();

    settings.endGroup();
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 21.11.2007

/*! @brief Laden der Programmeinstellungen */

void MainWindow::loadPreferences()
{
    #if defined(Q_OS_LINUX)
        gi_CodecInput                       = _UTF8_;               //!< UTF-8
        gi_CodecOutput                      = _UTF8_;
        gi_CodecDownload                    = _UTF8_;
        gi_EOL                              = _UNIX_;
    #endif

    #if defined(Q_OS_MAC)
        gi_CodecInput                       = _APPLEROMAN_;        //!< Apple Roman
        gi_CodecOutput                      = _APPLEROMAN_;
        gi_CodecDownload                    = _APPLEROMAN_;
        gi_EOL                              = _UNIX_;
    #endif

    #if defined(Q_OS_WIN)
        gi_CodecInput                       = _LATIN1_;            //!< Latin-1 = ISO 8859-1
        gi_CodecOutput                      = _LATIN1_;
        gi_CodecDownload                    = _LATIN1_;
        gi_EOL                              = _WIN_;
    #endif

    #if defined(Q_OS_LINUX)
        QSettings settings( getPreferenceFilename(), QSettings::IniFormat );
    #endif

    #if defined(Q_OS_MAC)
        QSettings settings( getPreferenceFilename(), QSettings::NativeFormat );
    #endif

    #if defined(Q_OS_WIN)
        QSettings settings( getPreferenceFilename(), QSettings::IniFormat );
    #endif

    settings.beginGroup( QCoreApplication::applicationName() );

    settings.beginGroup( "GeneralSettings" );
    gi_NumOfProgramStarts = settings.value( "NumOfProgramStarts", 0 ).toInt();

    posWindow.setX( settings.value( "WindowX", 100 ).toInt() );
    posWindow.setY( settings.value( "WindowY", 100 ).toInt() );
    sizeWindow.setHeight( settings.value( "WindowHeight", 250 ).toInt() );
    sizeWindow.setWidth( settings.value( "WindowWidth", 600 ).toInt() );

    posDialog.setX( settings.value( "DialogX", 110 ).toInt() );
    posDialog.setY( settings.value( "DialogY", 110 ).toInt() );
    sizeParameterDialog.setHeight( settings.value( "ParameterDialogHeight", 100 ).toInt() );
    sizeParameterDialog.setWidth( settings.value( "ParameterDialogWidth", 100 ).toInt() );

    gs_Path = settings.value( "Path", getDocumentDir() ).toString();

    gb_CuratorMode = settings.value( "CuratorMode", false ).toBool();
    settings.endGroup();

    // Google Earth
    settings.beginGroup( "GoogleEarth" );
    gs_FilenameGoogleEarthProgram = settings.value( "FilenameGoogleEarthProgram", "Browse through the Google Earth program file with \"Browse ...\"" ).toString();
    gs_FilenameGoogleEarth        = settings.value( "FilenameGoogleEarth", "" ).toString();
    gb_startGoogleEarth           = settings.value( "StartGoogleEarth", false ).toBool();
    gb_displayEventLabel          = settings.value( "DisplayEventLabel", false ).toBool();
    gb_displayDescription         = settings.value( "DisplayDescription", false ).toBool();
    gi_IconSize                   = settings.value( "IconSize", 40 ).toInt();
    gi_IconColor                  = settings.value( "IconColor", _RED ).toInt();
    gi_TracklineWidth             = settings.value( "TracklineWidth", 20 ).toInt();
    gi_TracklineColor             = settings.value( "TracklineColor", _ORANGE ).toInt();
    settings.endGroup();

    // Ocean Data View
    settings.beginGroup( "OceanDataView" );
    gs_FilenameOceanDataViewProgram = settings.value( "FilenameOceanDataViewProgram", "Browse through the Ocean Data View program file with \"Browse ...\"" ).toString();
    gs_FilenameOceanDataView        = settings.value( "FilenameOceanDataView", "" ).toString();
    gb_startOceanDataView           = settings.value( "StartOceanDataView", false ).toBool();
    gb_usePANGAEAQualityFlags       = settings.value( "UsePANGAEAQualityFlags", false ).toBool();
    gi_DeviceTypeOceanDataView      = settings.value( "DeviceTypeOceanDataView", _DEVICETYPEAUTO ).toInt();
    gb_addLineNo                    = settings.value( "AddLineNo", false ).toBool();
    settings.endGroup();

    // Shapefile
    settings.beginGroup( "Shapefile" );
    gs_FilenameShapefile = settings.value( "FilenameShapefile", "" ).toString();
    settings.endGroup();

    // Geocode range
    settings.beginGroup( "GeocodeRange" );
    gf_GEOCODE_min = settings.value( "GEOCODE_min_f", -99999.999 ).toFloat();
    gf_GEOCODE_max = settings.value( "GEOCODE_max_f", 99999.999 ).toFloat();
    gs_GEOCODE_min = settings.value( "GEOCODE_min_dt", QDateTime( QDateTime::currentDateTime() ).toString( Qt::ISODate ) ).toString();
    gs_GEOCODE_max = settings.value( "GEOCODE_max_dt", QDateTime( QDateTime::currentDateTime() ).toString( Qt::ISODate ) ).toString();
    settings.endGroup();

    // Global options
    settings.beginGroup( "GlobalOptions" );
    gb_createDir         = settings.value( "CreateDir", true ).toBool();
    gb_setGeocode        = settings.value( "SetGeocode", false ).toBool();
    gb_setGearID         = settings.value( "SetGearID", false ).toBool();
    gi_GearID            = settings.value( "GearID", 1 ).toInt();
    gb_showShortName     = settings.value( "ShowShortName", false ).toBool();
    gb_showMethod        = settings.value( "ShowMethod", false ).toBool();
    gb_showComment       = settings.value( "ShowComment", false ).toBool();
    gi_DateTimeFormat    = settings.value( "DateTimeFormat", _BUILDISODATETIME ).toInt();
    gb_sortParameterList = settings.value( "SortParameterList", false ).toBool();
    gi_CodecInput        = settings.value( "CodecInput", gi_CodecInput ).toInt();
    gi_CodecOutput       = settings.value( "CodecOutput", gi_CodecInput ).toInt();
    gi_CodecDownload     = settings.value( "CodecDownload", gi_CodecInput ).toInt();
    gi_EOL               = settings.value( "EOL", gi_EOL ).toInt();
    settings.endGroup();

    // Textfile, field delimiter and missing value
    settings.beginGroup( "TextFile" );
    gs_FilenameText   = settings.value( "FilenameText", "" ).toString();
    gs_MissingValue   = settings.value( "MissingValue", "" ).toString();
    gi_FieldDelimiter = settings.value( "FieldDelimiter", _TABSTOP_ ).toInt();
    gi_FieldAlignment = settings.value( "FieldAlignment", QTextStream::AlignLeft ).toInt();
    gi_FieldWidth     = settings.value( "FieldWidth", 20 ).toInt();
    settings.endGroup();

    // PanGet ID
    settings.beginGroup( "PanGet" );
    gs_IDListFile        = settings.value( "IDListFile", ""  ).toString();
    gs_DownloadDirectory = settings.value( "DownloadDirectory", "" ).toString();
    settings.endGroup();

    settings.endGroup();

    // kein Dialog vorhanden, daher hier gesetzt.
    // gb_CuratorMode = true;
    // gb_CuratorMode = false;
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

/*! @brief Ermitteln des Dateinamens fuer die Programmeinstellungen.
*   @return Name der Datei fuer die Programmeinstellungen. Dateiname ist gleich "_ERROR_",
*   wenn Betriebssystem ungleich Windows, Mac OS oder Unix ist.
*/

QString MainWindow::getPreferenceFilename()
{
    QString s_PrefFilename = "";

    #if defined(Q_OS_LINUX)
        s_PrefFilename = QDir::homePath() + QLatin1String( "/.config/" ) + QCoreApplication::organizationName() + "/" + QCoreApplication::applicationName() + "/" + QCoreApplication::applicationName() + QLatin1String( ".conf" );
    #endif

    #if defined(Q_OS_MAC)
        s_PrefFilename = QDir::homePath() + QLatin1String( "/Library/Preferences/" ) + QCoreApplication::organizationDomain().section( ".", 1, 1 ) + "." + QCoreApplication::organizationDomain().section( ".", 0, 0 ) + "." + QCoreApplication::applicationName() + QLatin1String( ".plist" );
    #endif

    #if defined(Q_OS_WIN)
        if ( QCoreApplication::applicationName().toLower().endsWith( "portable" ) )
        {
            s_PrefFilename = QCoreApplication::applicationDirPath() + "/" + QCoreApplication::applicationName() + QLatin1String( ".ini" );
        }
        else
        {
            QSettings cfg( QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName() );
            s_PrefFilename = QFileInfo( cfg.fileName() ).absolutePath() + "/" + QCoreApplication::applicationName() + "/" + QCoreApplication::applicationName() + QLatin1String( ".ini" );
        }
    #endif

    return( s_PrefFilename );
}
