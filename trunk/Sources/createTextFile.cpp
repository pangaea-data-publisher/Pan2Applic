// ***********************************************************************************************
// *                                                                                             *
// * createTextFile.cpp - Create a text file                                                     *
// *                                                                                             *
// * Dr. Rainer Sieger - 2009-04-24                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include "Application.h"

/*! @brief Liest Zeilen aus einer Datei, verarbeitet sie und schreibt sie in eine neue Datei. */

int MainWindow::createTextFile( QStringList &sl_MetadataList, QStringList &sl_ParameterList, const QString &s_FilenameOut,
                                const int i_CodecInput, const int i_CodecOutput, const int i_EOL, const int b_OutputFormat,
                                const int i_Env, const int i_DateTimeFormat, const bool b_showShortName, const bool b_showMethod,
                                const bool b_showComment, const QString &s_MissingValue, const int i_FieldAlignment,
                                const int i_FieldWidth, const int i_FieldDelimiter, const bool b_setGeocode, const float f_GEOCODE_min,
                                const float f_GEOCODE_max, const QString &s_GEOCODE_min, const QString &s_GEOCODE_max,
                                const bool b_setGearID, const int i_GearID, const int i_NumOfFiles )
{
    QVector<int>    vi_pos;
    QVector<int>    vi_SpecialPos;

    int             i_GeocodePos        = -1;
    int             i_GearIDPos         = -1;
    int             i_File              = 0;
    int             stopProgress        = 0;
    int             i_stopFileProgress  = 0;
    int             err                 = _NOERROR_;

    int             i                   = 0;
    int             n                   = 0;

    QString         s_Environment       = "";
    QString         s_FieldDelimiter    = "\t";
    QString         s_FilenameIn        = sl_MetadataList.at( 1 ).section( "\t", _FILENAMEPOS, _FILENAMEPOS );
    QString         s_EOL               = setEOLChar( i_EOL );

    QStringList     sld_ParameterList;
    QStringList     sl_Input;
    QStringList     sl_Output;

    QDateTime       dt_GEOCODE_min( QDateTime::fromString( (QString) s_GEOCODE_min, Qt::ISODate ) );
    QDateTime       dt_GEOCODE_max( QDateTime::fromString( (QString) s_GEOCODE_max, Qt::ISODate ) );

//-----------------------------------------------------------------------------------------------------------------------

    if ( sl_ParameterList.count() == 0 )
        return( -60 );

//-----------------------------------------------------------------------------------------------------------------------

    QFile ftext( s_FilenameOut );

    if ( ftext.open( QIODevice::WriteOnly | QIODevice::Text) == false )
        return( -20 );

    QTextStream tout( &ftext );

    switch ( i_CodecOutput )
    {
    case _SYSTEM_:
        break;
    case _LATIN1_:
        tout.setCodec( QTextCodec::codecForName( "ISO 8859-1" ) );
        break;
    case _APPLEROMAN_:
        tout.setCodec( QTextCodec::codecForName( "Apple Roman" ) );
        break;
    default:
        tout.setCodec( QTextCodec::codecForName( "UTF-8" ) );
        break;
    }

    switch ( b_OutputFormat )
    {
        case _FORMAT_FORMATEDTEXTFILE:
            switch ( i_FieldAlignment )
            {
                case QTextStream::AlignLeft:
                    tout.setFieldAlignment( QTextStream::AlignLeft );
                    break;
                case QTextStream::AlignRight:
                    tout.setFieldAlignment( QTextStream::AlignRight );
                    break;
            }

            s_FieldDelimiter = " ";
            break;

        case _FORMAT_UNFORMATEDTEXTFILE:
            switch ( i_FieldDelimiter )
            {
                case _COMMA_:
                    s_FieldDelimiter = ",";
                    break;
                case _SEMICOLON_:
                    s_FieldDelimiter = ";";
                    break;
                case _SPACE_:
                    s_FieldDelimiter = " ";
                    break;
                default:
                    s_FieldDelimiter = "\t";
                    break;
            }

            tout.setFieldAlignment( QTextStream::AlignLeft );
            break;

        default:
            s_FieldDelimiter = "\t";
            tout.setFieldAlignment( QTextStream::AlignLeft );
            break;
    }

//-----------------------------------------------------------------------------------------------------------------------

    initFileProgress( i_NumOfFiles, s_FilenameIn, tr( "Creating text file..." ) );

//-----------------------------------------------------------------------------------------------------------------------
// find position of date/time parameters

    getSpecialPos( sl_ParameterList, vi_SpecialPos );

//-----------------------------------------------------------------------------------------------------------------------
// write header

    buildHeader( sl_Output, sl_ParameterList, i_Env, b_OutputFormat, b_showShortName, b_showMethod, b_showComment );
    i_GeocodePos = getGeocodePosition( sl_ParameterList, i_Env );
    i_GearIDPos  = getGearIDPosition( sl_ParameterList );

    switch ( b_OutputFormat )
    {
        case _FORMAT_FORMATEDTEXTFILE:
            tout.setFieldWidth( i_FieldWidth );
            tout << sl_Output.at( 0 );

            for ( int j=1; j<sl_Output.count(); ++j )
                tout << " " << sl_Output.at( j );

            tout.setFieldWidth( 0 );
            break;

        case _FORMAT_UNFORMATEDTEXTFILE:
            tout << sl_Output.at( 0 );

            for ( int j=1; j<sl_Output.count(); ++j )
                tout << s_FieldDelimiter << sl_Output.at( j );

            break;

        default:
            tout << "Output format not given. Contact rsieger@pangaea.de";
            break;
    }

    tout << s_EOL;

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

                initProgress( i_NumOfFiles, s_FilenameIn, tr( "Text converter is working..." ), n );

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
// read data

                while ( ( ++i < n ) && ( stopProgress != _APPBREAK_ ) )
                {
                    sl_Output.clear();

                    for ( int j=0; j<sl_ParameterList.count(); ++j )
                    {
                        if ( vi_pos[j] >= 0 )
                        {   // Parameter
                            switch ( vi_SpecialPos[j] )
                            {
                            case _ISDATETIME:
                                sl_Output.append( buildDateTimeStr( sl_Input.at( i ).section( "\t", vi_pos[j], vi_pos[j] ), _FORMAT_ISO ) );
                                break;
                            default:
                                sl_Output.append( sl_Input.at( i ).section( "\t", vi_pos[j], vi_pos[j] ) );
                                break;
                            }
                        }
                        else
                        {   // Metadata
                            if ( vi_pos[j] == -9 )
                                sl_Output.append( buildDateTimeStr( sl_MetadataList.at( i_File ), -vi_pos[j], i_DateTimeFormat ) );
                            else
                                sl_Output.append( sl_MetadataList.at( i_File ).section( "\t", -vi_pos[j], -vi_pos[j] ) );
                        }
                    }

//-----------------------------------------------------------------------------------------------------------------------
// write data line

                    if ( writeDataLine( b_setGeocode, b_setGearID, i_GeocodePos, i_GearIDPos, sl_Output, i_Env, dt_GEOCODE_min, dt_GEOCODE_max, f_GEOCODE_min, f_GEOCODE_max, i_GearID ) == true )
                    {
                        switch ( b_OutputFormat )
                        {
                            case _FORMAT_FORMATEDTEXTFILE:
                                tout.setFieldWidth( i_FieldWidth );
                                tout << sl_Output.at( 0 );

                                for ( int j=1; j<sl_Output.count(); ++j )
                                {
                                    if ( sl_Output.at( j ).isEmpty() == true )
                                        tout << " " << s_MissingValue;
                                    else
                                        tout << " " << sl_Output.at( j );
                                }

                                tout.setFieldWidth( 0 );
                                break;

                            case _FORMAT_UNFORMATEDTEXTFILE:
                                tout << sl_Output.at( 0 );

                                for ( int j=1; j<sl_Output.count(); ++j )
                                {
                                    if ( sl_Output.at( j ).isEmpty() == true )
                                        tout << s_FieldDelimiter << s_MissingValue;
                                    else
                                        tout << s_FieldDelimiter << sl_Output.at( j );
                                }
                                break;

                            default:
                                tout << "Output format not given. Contact rsieger@pangaea.de";
                                break;
                        }

                        tout << s_EOL;
                    }

                    stopProgress = incProgress( i_NumOfFiles, i );

//-----------------------------------------------------------------------------------------------------------------------

                }

                resetProgress( i_NumOfFiles );
            }
        }

        i_stopFileProgress = incFileProgress( i_NumOfFiles, i_File++ );
    }

//-----------------------------------------------------------------------------------------------------------------------

    resetFileProgress( i_NumOfFiles );

    ftext.close();

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2009-10-17

/*! @brief Steuert den Converter */

void MainWindow::doCreateUnformatedTextFile()
{
    doCreateTextFile( _FORMAT_UNFORMATEDTEXTFILE, false );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void MainWindow::doCreateUnformatedTextFileCuratorMode()
{
    doCreateTextFile( _FORMAT_UNFORMATEDTEXTFILE, true );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void MainWindow::doCreateFormatedTextFile()
{
    doCreateTextFile( _FORMAT_FORMATEDTEXTFILE, false );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void MainWindow::doCreateTextFile( const int i_Format, const bool b_CuratorMode )
{
    int         err               = _CHOOSEABORTED_;

    int         i_File            = 0;
    int         i_Env             = 0;
    int         stopProgress      = 0;

    int         i_EnvArray[_MAX_NUM_OF_ENV+1];

    QStringList	sl_MetadataList;
    QStringList	sl_ParameterList;
    QStringList sl_selectedParameters;

// **********************************************************************************************

    for ( int i=0; i<=_MAX_NUM_OF_ENV; ++i )
        i_EnvArray[i] = -1;

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        if ( setFilename( i_Format, b_CuratorMode, gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), gs_FilenameText ) == QDialog::Accepted )
        {
            sl_MetadataList.append( tr( "File name\tMetaInfo\tHeader lines\tEnvironment\tCampaign\tEvent label\tGear\tDate\tTime\tDate/Time\tDate/Time start\tLatitude\tLongitude\tElevation\tArea\tDOI\tCitation\tDatePos\tTimePos\tDateTimePos\tElevationPos\tEventLabelPos\tCampaignLabelPos\tAreaNamePos\tGearNamePos\tLatitudePos\tLongitudePos" ) );

            initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Analyze files..." ) );

            err = _NOERROR_;

            while ( ( i_File < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
            {
                setStatusBarFileInProgress( gsl_FilenameList.at( i_File ) );

                if ( b_CuratorMode == true )
                    err = analyzeDataset( gsl_FilenameList.at( i_File ), gi_CodecInput, false, true, true, sl_MetadataList, sl_ParameterList, i_EnvArray );
                else
                    err = analyzeDataset( gsl_FilenameList.at( i_File ), gi_CodecInput, gb_showShortName, gb_showMethod, gb_showComment, sl_MetadataList, sl_ParameterList, i_EnvArray );

                stopProgress = incFileProgress( gsl_FilenameList.count(), i_File++ );
            }

            resetFileProgress( gsl_FilenameList.count() );
        }
    }

// **********************************************************************************************
// Select environment dialog

    if ( ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
    {
        i_Env = doSelectEnvironmentDialog( gb_setGeocode, i_EnvArray, sl_MetadataList );

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
    {
        if ( b_CuratorMode == true )
            err = doSelectParametersDialog( i_Env, sl_ParameterList, false, true, true, _MAX_NUM_OF_PARAMETERS, gb_sortParameterList, sl_selectedParameters );
        else
            err = doSelectParametersDialog( i_Env, sl_ParameterList, gb_showShortName, gb_showMethod, gb_showComment, _MAX_NUM_OF_PARAMETERS, gb_sortParameterList, sl_selectedParameters );
    }

// **********************************************************************************************
// create text file

    if ( ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
    {
        if ( b_CuratorMode == true )
            err = createTextFile( sl_MetadataList, sl_selectedParameters, gs_FilenameText, gi_CodecInput, gi_CodecOutput, gi_Extension, i_Format, i_Env, _FORMAT_ISO, false, true, true, "", _TABSTOP_, gi_FieldWidth, gi_FieldDelimiter, false, gf_GEOCODE_min, gf_GEOCODE_max, gs_GEOCODE_min, gs_GEOCODE_max, false, gi_GearID, sl_MetadataList.count()-1 );
        else
            err = createTextFile( sl_MetadataList, sl_selectedParameters, gs_FilenameText, gi_CodecInput, gi_CodecOutput, gi_Extension, i_Format, i_Env, gi_DateTimeFormat, gb_showShortName, gb_showMethod, gb_showComment, gs_MissingValue, gi_FieldAlignment, gi_FieldWidth, gi_FieldDelimiter, gb_setGeocode, gf_GEOCODE_min, gf_GEOCODE_max, gs_GEOCODE_min, gs_GEOCODE_max, gb_setGearID, gi_GearID, sl_MetadataList.count()-1 );
    }

// **********************************************************************************************

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Converter was canceled" ), false, false );

    onError( err );
}
