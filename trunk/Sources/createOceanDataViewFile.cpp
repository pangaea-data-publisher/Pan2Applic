// ***********************************************************************************************
// *                                                                                             *
// * createOceanDataViewFile.cpp - Create a Ocean Data view file                                 *
// *                                                                                             *
// * Dr. Rainer Sieger - 2009-09-14                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include "Application.h"

/*! @brief Liest Zeilen aus einer Datei, verarbeitet sie und schreibt sie in eine neue Datei. */

int MainWindow::createOceanDataViewFile( QStringList& sl_MetadataList, QStringList& sl_ParameterList, const QString& s_FilenameOut,
                                         const int i_CodecInput, const int i_Env, const int i_DeviceTypeOceanDataView,
                                         const bool b_showShortName, const bool b_showMethod, const bool b_showComment,
                                         const bool b_usePANGAEAQualityFlags, const bool b_setGeocode, const bool b_addLineNo,
                                         const float f_GEOCODE_min, const float f_GEOCODE_max, const QString& s_GEOCODE_min,
                                         const QString& s_GEOCODE_max, const bool b_setGearID, const int i_GearID, const int i_NumOfFiles )
{
    QVector<int>    vi_pos;
    QVector<int>    vi_posQV;
    QVector<int>    vi_SpecialPos;

    int             i_NumOfRemovedLines = 0;
    int             i_NumOfHeaderItems  = 0;

    int             i_GeocodePos        = -1;
    int             i_GearIDPos         = -1;

    int             i_File              = 0;
    int             i_stopFileProgress  = 0;

    int             stopProgress        = 0;
    int             err                 = _NOERROR_;

    int             i                   = 0;
    int             n                   = 0;

    bool            b_containsDOI       = false;

    QString         s_FieldDelimiter    = "\t";
    QString         s_Environment       = "";
    QString         s_DeviceType        = "*";
    QString         s_Output            = "";
    QString         s_FilenameIn        = sl_MetadataList.at( 1 ).section( "\t", _FILENAMEPOS, _FILENAMEPOS );

    QStringList     sld_ParameterList;
    QStringList     sl_Input;
    QStringList     sl_Output;

    QDateTime       dt_GEOCODE_min( QDateTime::fromString( (QString) s_GEOCODE_min, Qt::ISODate ) );
    QDateTime       dt_GEOCODE_max( QDateTime::fromString( (QString) s_GEOCODE_max, Qt::ISODate ) );

//-----------------------------------------------------------------------------------------------------------------------

    if ( sl_ParameterList.count() == 0 )
        return( -60 );

//-----------------------------------------------------------------------------------------------------------------------

    i_NumOfHeaderItems = buildODVHeader( sl_Output, sl_ParameterList, vi_posQV, i_Env, b_showShortName, b_showMethod, b_showComment, b_usePANGAEAQualityFlags );

    if ( i_NumOfHeaderItems < 0 )
        return( i_NumOfHeaderItems ); // return as error, -70 = Latitude/Longitude not given, -71 = Latitude not given, -72 = Longitude not given

//-----------------------------------------------------------------------------------------------------------------------

    QFile ftext( s_FilenameOut );

    if ( ftext.open( QIODevice::WriteOnly | QIODevice::Text) == false )
        return( -20 );

    QTextStream tout( &ftext );

//-----------------------------------------------------------------------------------------------------------------------

    initFileProgress( i_NumOfFiles, s_FilenameIn, tr( "Creating Ocean Data View file..." ) );

//-----------------------------------------------------------------------------------------------------------------------
// write header

    tout << "//<Version>ODV Spreadsheet V4.0</Version>" << endl;
    tout << "//<Creator>" << "</Creator>" << endl;
    tout << "//<CreateTime>" << QDateTime::currentDateTime().toString( "yyyy-MM-ddThh:mm" ) << "</CreateTime>" << endl;
    tout << "//<Software>" << getApplicationName( true ) << " " << _VERSION_ << "</Software>" << endl;
    tout << "//<Source>" << QDir::toNativeSeparators( s_FilenameOut ) << "</Source>" << endl;
    tout << "//<SourceLastModified>" << QDateTime::currentDateTime().toString( "yyyy-MM-ddThh:mm" ) << "</SourceLastModified>" << endl;
    tout << "//<DataField>GeneralField</DataField>" << endl;
    tout << "//<DataType>GeneralType</DataType>" << endl;
    tout << "//" << endl;

//-----------------------------------------------------------------------------------------------------------------------

    i_GeocodePos   = getGeocodePosition( sl_ParameterList, i_Env );
    i_GearIDPos    = getGearIDPosition( sl_ParameterList );
    b_containsDOI  = sl_Output.contains( "DOI:METAVAR:TEXT:51" );

    tout << sl_Output.join( s_FieldDelimiter ) << endl;

//-----------------------------------------------------------------------------------------------------------------------
// find position of date/time parameters

    getSpecialPos( sl_ParameterList, vi_SpecialPos );

//-----------------------------------------------------------------------------------------------------------------------

    s_DeviceType = setDeviceType( i_DeviceTypeOceanDataView );

//-----------------------------------------------------------------------------------------------------------------------

    i_File = 1;

    while ( ( i_File <= i_NumOfFiles ) && ( err == _NOERROR_ ) && ( i_stopFileProgress != _APPBREAK_ ) )
    {
        if ( containsSelectedEnvironment( _SELECTPARAMETER, sl_MetadataList.at( i_File ).section( "\t", _ENVIRONMENTPOS, _ENVIRONMENTPOS ).toInt(), i_Env ) == true )
        {
            s_FilenameIn = sl_MetadataList.at( i_File ).section( "\t", _FILENAMEPOS, _FILENAMEPOS );

            if ( ( n = readFile( s_FilenameIn, sl_Input, i_CodecInput ) ) > 0 )
            {
                i = 0;

                sld_ParameterList.clear();
                vi_pos.clear();

                initProgress( i_NumOfFiles, s_FilenameIn, tr( "Ocean Data View converter is working..." ), n );

                setStatusBarFileInProgress( s_FilenameIn );

//-----------------------------------------------------------------------------------------------------------------------
// read header and find position of parameter

                s_Environment = sl_MetadataList.at( i_File ).section( "\t", _ENVIRONMENTPOS, _ENVIRONMENTPOS ) + _PSEPARATOR_;

                stopProgress = incProgress( i_NumOfFiles, i );

                if ( sl_Input.at( i ).startsWith( "/* DATA DESCRIPTION:" ) == true ) // file starts with PANGAEA header
                {
                    while ( ( i < n ) && ( sl_Input.at( i ).startsWith( "*/" ) == false ) )
                    {
                        if ( sl_Input.at( i ).startsWith( "Parameter(s):" ) == true )
                        {
                            while ( ( i < n ) && ( ( sl_Input.at( i ).startsWith( "Parameter(s):" ) == true ) || ( sl_Input.at( i ).startsWith( "\t" ) == true ) ) )
                            {
                                sld_ParameterList.append( s_Environment + buildParameter( sl_Input.at( i ).section( "\t", 1, 1 ) ) );

                                if ( ++i < n )
                                    stopProgress = incProgress( i_NumOfFiles, i );
                            }
                        }

                        if ( ++i < n )
                            stopProgress = incProgress( i_NumOfFiles, i );
                    }

                    for ( int j=0; j<sl_ParameterList.count(); ++j )
                        vi_pos.append( containsParameter( true, sld_ParameterList, sl_ParameterList.at( j ), b_showShortName, b_showMethod, b_showComment ) );

                    if ( ++i < n )
                        stopProgress = incProgress( i_NumOfFiles, i );
                }
                else
                {
                    sld_ParameterList = buildParameterList( s_Environment, sl_Input.at( i ) );  // not a PANGAEA dataset

                    for ( int j=0; j<sl_ParameterList.count(); ++j )
                        vi_pos.append( containsParameter( false, sld_ParameterList, sl_ParameterList.at( j ), b_showShortName, b_showMethod, b_showComment ) );
                }

//-----------------------------------------------------------------------------------------------------------------------
// write output string

                if ( ( b_setGeocode == true ) || ( b_setGearID == true ) )
                {
                    while ( ( ++i < n ) && ( stopProgress != _APPBREAK_ ) )
                    {
                        sl_Output = buildODVOutputStr( i+1, b_addLineNo, sl_Input.at( i ), sl_MetadataList.at( i_File ), vi_pos, vi_posQV, vi_SpecialPos, b_usePANGAEAQualityFlags, b_containsDOI, s_DeviceType, s_FieldDelimiter );

                        if ( writeDataLine( b_setGeocode, b_setGearID, i_GeocodePos, i_GearIDPos, sl_Output, i_Env, dt_GEOCODE_min, dt_GEOCODE_max, f_GEOCODE_min, f_GEOCODE_max, i_GearID ) == true )
                        {
                            s_Output = sl_Output.join( s_FieldDelimiter );

                            if ( s_Output.contains( "delete line" ) == false )
                                tout << s_Output  << endl;
                            else
                                ++i_NumOfRemovedLines;
                        }

                        stopProgress = incProgress( i_NumOfFiles, i );
                    }
                }
                else
                {
                    while ( ( ++i < n ) && ( stopProgress != _APPBREAK_ ) )
                    {
                        sl_Output = buildODVOutputStr( i+1, b_addLineNo, sl_Input.at( i ), sl_MetadataList.at( i_File ), vi_pos, vi_posQV, vi_SpecialPos, b_usePANGAEAQualityFlags, b_containsDOI, s_DeviceType, s_FieldDelimiter );
                        s_Output  = sl_Output.join( s_FieldDelimiter );

                        if ( s_Output.contains( "delete line" ) == false )
                            tout << s_Output  << endl;
                        else
                            ++i_NumOfRemovedLines;

                        stopProgress = incProgress( i_NumOfFiles, i );
                    }
                }

                resetProgress( i_NumOfFiles );
            }
        }

        i_stopFileProgress = incFileProgress( i_NumOfFiles, i_File++ );
    }

//-----------------------------------------------------------------------------------------------------------------------

    resetFileProgress( i_NumOfFiles );

    ftext.close();

    return( i_NumOfRemovedLines );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2009-09-14

/*! @brief Steuert den Converter */

void MainWindow::doCreateOceanDataViewFile()
{
    int         i_File              = 0;
    int         err                 = _NOERROR_;
    int         stopProgress        = 0;
    int         i_Env               = 0;
    int         i_NumOfRemovedLines = 0;

    int         i_EnvArray[_MAX_NUM_OF_ENV+1];

    QStringList	sl_MetadataList;
    QStringList	sl_ParameterList;
    QStringList sl_selectedParameters;

// **********************************************************************************************

    for ( int i=0; i<=_MAX_NUM_OF_ENV; ++i )
        i_EnvArray[i] = -1;

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        err = setFilename( _FORMAT_ODV, gb_CuratorMode, gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), gs_FilenameOceanDataView );

        if ( err == _NOERROR_ )
        {
            sl_MetadataList.append( tr( "File name\tMetaInfo\tHeader lines\tEnvironment\tCampaign\tEvent label\tGear\tDate\tTime\tDate/Time\tDate/Time start\tLatitude\tLongitude\tElevation\tArea\tDOI\tCitation\tDatePos\tTimePos\tDateTimePos\tElevationPos\tEventLabelPos\tCampaignLabelPos\tAreaNamePos\tGearNamePos\tLatitudePos\tLongitudePos" ) );

            initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Analyze files..." ) );

            while ( ( i_File < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
            {
                setStatusBarFileInProgress( gsl_FilenameList.at( i_File ) );

                err = analyzeDataset( gsl_FilenameList.at( i_File ), gi_CodecInput, gb_showShortName, gb_showMethod, gb_showComment, sl_MetadataList, sl_ParameterList, i_EnvArray );

                stopProgress = incFileProgress( gsl_FilenameList.count(), i_File++ );
            }

            resetFileProgress( gsl_FilenameList.count() );
        }
    }
    else
    {
        err = _CHOOSEABORTED_;
    }

// **********************************************************************************************
// Select environment dialog

    if ( ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
    {
        i_Env = doSelectEnvironmentDialog( gb_setGeocode, i_EnvArray, sl_MetadataList );

        // Geocode found?
        if ( i_Env < 1 )
            err = -41;

        // Set geocode range
        if ( ( gb_setGeocode == true ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
            err = setGeocodeRange( i_Env );

        // set gear ID
        if ( ( gb_setGearID == true ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
            err = setGearIDDialog();
    }

// **********************************************************************************************
// Select parameter dialog

    if ( ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
        err = doSelectParametersDialog( i_Env, sl_ParameterList, gb_showShortName, gb_showMethod, gb_showComment, _MAX_NUM_OF_PARAMETERS, gb_sortParameterList, sl_selectedParameters );

// **********************************************************************************************
// Create Ocean Data View file

    if ( ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
        i_NumOfRemovedLines = createOceanDataViewFile( sl_MetadataList, sl_selectedParameters, gs_FilenameOceanDataView, gi_CodecInput, i_Env, gi_DeviceTypeOceanDataView, gb_showShortName, gb_showMethod, gb_showComment, gb_usePANGAEAQualityFlags, gb_setGeocode, gb_addLineNo, gf_GEOCODE_min, gf_GEOCODE_max, gs_GEOCODE_min, gs_GEOCODE_max, gb_setGearID, gi_GearID, sl_MetadataList.count()-1 );

// **********************************************************************************************

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Converter was canceled" ), false, false );

// **********************************************************************************************

    if ( ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) && ( i_NumOfRemovedLines != 0 ) )
    {
        if ( i_NumOfRemovedLines > 0 )
        {
            QString s_Warning = QString( "%1 lines have been removed because\nof missing or invalid position!" ).arg( i_NumOfRemovedLines );
            QMessageBox::warning( this, getApplicationName( true ), s_Warning );
        }

        if ( i_NumOfRemovedLines < 0 )
            err = i_NumOfRemovedLines;
    }

// **********************************************************************************************
// start Ocean Data View

    if ( ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) && ( gb_startOceanDataView == true ) )
    {
        QFileInfo fi( gs_FilenameOceanDataViewProgram );

        if ( ( fi.isExecutable() == true ) && ( fi.exists() == true ) )
        {
            if ( ( gs_FilenameOceanDataViewProgram.contains( "odv4" ) == true ) || ( gs_FilenameOceanDataViewProgram.contains( "Ocean Data View (64bit)" ) == true ) || ( gs_FilenameOceanDataViewProgram.contains( "Ocean Data View (32bit)" ) == true ) )
                err = startProgram( gs_FilenameOceanDataViewProgram, gs_FilenameOceanDataView );
            else
                QMessageBox::warning( this, getApplicationName( true ), "Cannot start ODV!\nPan2Applic supports Ocean Data View 4.x only!\nPlease browse with the ODV options dialog\nto the ODV4 (http://odv.awi.de) program." );
        }
        else
        {
            if ( gs_FilenameOceanDataViewProgram.startsWith( "Browse through the Ocean Data View program file with" ) == true )
                QMessageBox::warning( this, getApplicationName( true ), getApplicationName( true ) + " can't start Ocean Data View.\nYou did not choose the program in the Ocean Data View\noptions dialog. Start again." );
        }
    }

// **********************************************************************************************

    onError( err );
}
