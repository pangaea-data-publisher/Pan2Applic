// ***********************************************************************************************
// *                                                                                             *
// * createShapefile.cpp - Create a shapefile                                                    *
// *                                                                                             *
// * Dr. Rainer Sieger - 2009-11-17 (In Erinnerung an Dr. Soenke Neben)                           *
// *                                                                                             *
// ***********************************************************************************************

#include "Application.h"
#include "shapefil.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2009-11-19

/*! @brief Liest Zeilen aus einer Datei, verarbeitet sie und schreibt sie in eine neue Datei. */

int MainWindow::createShapefileData( QStringList& sl_MetadataList, QStringList& sl_ParameterList, const structDataFormat DF[],
                                     const QString& s_Filename, const int i_CodecInput, const int i_Env, const bool b_setGeocode,
                                     const float f_GEOCODE_min, const float f_GEOCODE_max, const QString& s_GEOCODE_min,
                                     const QString& s_GEOCODE_max, const bool b_setGearID, const int i_GearID, const int i_NumOfFiles )
{
    int             iRecord				= 0;

    SHPHandle       hSHP				= NULL;
    DBFHandle       hDBF				= NULL;

//-----------------------------------------------------------------------------------------------------------------------

    QVector<int>    vi_pos;
    QVector<int>    vi_SpecialPos;

    int             i_File              = 0;
    int             stopProgress        = 0;
    int             i_stopFileProgress  = 0;
    int             i_GeocodePos        = -1;
    int             i_GearIDPos         = -1;
    int             i_LatitudePos       = -1;
    int             i_LongitudePos      = -1;

    int             err                 = _NOERROR_;

    int             i                   = 0;
    int             n                   = 0;

    QString         s_Environment       = "";
    QString         s_FilenameIn        = sl_MetadataList.at( 1 ).section( "\t", _FILENAMEPOS, _FILENAMEPOS );
    QString         s_DOI               = sl_MetadataList.at( 1 ).section( "\t", _DOIPOS, _DOIPOS );

    QStringList     sld_ParameterList;
    QStringList     sl_Input;
    QStringList     sl_Output;

    QFileInfo       fi( s_Filename );

    QDateTime       dt_GEOCODE_min( QDateTime::fromString( (QString) s_GEOCODE_min, Qt::ISODate ) );
    QDateTime       dt_GEOCODE_max( QDateTime::fromString( (QString) s_GEOCODE_max, Qt::ISODate ) );

//-----------------------------------------------------------------------------------------------------------------------

    if ( sl_ParameterList.count() == 0 )
        return( -60 );

//-----------------------------------------------------------------------------------------------------------------------

    hDBF  = DBFCreate( QDir::toNativeSeparators( fi.absolutePath() + "/" + fi.baseName() + ".dbf" ).toLatin1() );
    hSHP  = SHPCreate( QDir::toNativeSeparators( fi.absolutePath() + "/" + fi.baseName() + ".shp" ).toLatin1(), SHPT_POINT );

//-----------------------------------------------------------------------------------------------------------------------

    initFileProgress( i_NumOfFiles, gsl_FilenameList.at( 0 ), tr( "Creating shapefile..." ) );

//-----------------------------------------------------------------------------------------------------------------------
// find positions

    getSpecialPos( sl_ParameterList, vi_SpecialPos );

    i_LatitudePos  = getFirstPos( sl_ParameterList, "Latitude" );
    i_LongitudePos = getFirstPos( sl_ParameterList, "Longitude" );

    if ( ( i_LatitudePos == -1 ) && ( i_LongitudePos == -1 ) )
        return( -70 );

    if ( i_LatitudePos == -1 )
        return( -71 );

    if ( i_LongitudePos == -1 )
        return( -72 );

//-----------------------------------------------------------------------------------------------------------------------

    i_GeocodePos   = getGeocodePosition( sl_ParameterList, i_Env );
    i_GearIDPos    = getGearIDPosition( sl_ParameterList );

//-----------------------------------------------------------------------------------------------------------------------

    writeProjectionFile( fi.absolutePath() + tr( "/" ) + fi.baseName() + tr( ".prj" ) );
    writeDBFHeader( hDBF, sl_ParameterList, DF );

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

                initProgress( i_NumOfFiles, s_FilenameIn, tr( "Shapefile converter is working..." ), n );

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
                        vi_pos.append( containsParameter( true, sld_ParameterList, sl_ParameterList.at( j ), false, false, false ) );

                    if ( ++i < n )
                        stopProgress = incProgress( i_NumOfFiles, i );
                }
                else
                {
                    sld_ParameterList = buildParameterList( s_Environment, sl_Input.at( i ) );  // not a PANGAEA dataset

                    for ( int j=0; j<sl_ParameterList.count(); ++j )
                        vi_pos.append( containsParameter( false, sld_ParameterList, sl_ParameterList.at( j ), false, false, false ) );
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
                                sl_Output.append( buildDateTimeStr( sl_MetadataList.at( i_File ), -vi_pos[j], _FORMAT_ISO ) );
                            else
                                sl_Output.append( sl_MetadataList.at( i_File ).section( "\t", -vi_pos[j], -vi_pos[j] ) );
                        }
                    }

//-----------------------------------------------------------------------------------------------------------------------
// write data line

                    if ( writeDataLine( b_setGeocode, b_setGearID, i_GeocodePos, i_GearIDPos, sl_Output, i_Env, dt_GEOCODE_min, dt_GEOCODE_max, f_GEOCODE_min, f_GEOCODE_max, i_GearID ) == true )
                        iRecord = writeShapfileDataEntry( hDBF, hSHP, sl_Output, DF, i_LatitudePos, i_LongitudePos, s_DOI, iRecord );

                    stopProgress = incProgress( i_NumOfFiles, i );
                }

                resetProgress( i_NumOfFiles );
            }
        }

        i_stopFileProgress = incFileProgress( i_NumOfFiles, i_File++ );
    }

    resetFileProgress( i_NumOfFiles );

    SHPClose( hSHP );
    DBFClose( hDBF );

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2009-11-18

/*! @brief Liest Zeilen aus einer Datei, verarbeitet sie und schreibt sie in eine neue Datei. */

int MainWindow::createShapefileMetadata( QStringList& sl_MetadataList, const QString& s_Filename, const int i_Codec, const int i_NumOfFiles )
{
    int             iRecord				= 0;

    SHPHandle       hSHP				= NULL;
    DBFHandle       hDBF				= NULL;

//-----------------------------------------------------------------------------------------------------------------------

    int             i                   = 0;
    int             stopProgress      = 0;
    int             err                 = _NOERROR_;

    QFileInfo       fi( s_Filename );

//-----------------------------------------------------------------------------------------------------------------------

    hDBF  = DBFCreate( QDir::toNativeSeparators( fi.absolutePath() + "/" + fi.baseName() + ".dbf" ).toLatin1() );
    hSHP  = SHPCreate( QDir::toNativeSeparators( fi.absolutePath() + "/" + fi.baseName() + ".shp" ).toLatin1(), SHPT_POINT );

//-----------------------------------------------------------------------------------------------------------------------

    initFileProgress( i_NumOfFiles, gsl_FilenameList.at( 0 ), tr( "Creating shapefile..." ) );

//-----------------------------------------------------------------------------------------------------------------------

    writeProjectionFile( fi.absolutePath() + tr( "/" ) + fi.baseName() + tr( ".prj" ) );
    writeDBFHeader( hDBF );

    while ( ( ++i <= i_NumOfFiles ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
    {
        if ( sl_MetadataList.at( i ).section( "\t", _METAINFOPOS, _METAINFOPOS ) == "1" )
            iRecord = writeShapfileMetadataEntry( hDBF, hSHP, sl_MetadataList.at( i ), iRecord );
        else
            iRecord = writeShapfileMetadataEntry( hDBF, hSHP, i_Codec, sl_MetadataList.at( i ), iRecord, i_NumOfFiles );

        stopProgress = incFileProgress( i_NumOfFiles, i );
    }

    resetFileProgress( i_NumOfFiles );

    SHPClose( hSHP );
    DBFClose( hDBF );

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2009-11-19

int MainWindow::writeShapfileDataEntry( DBFHandle hDBF, SHPHandle hSHP, const QStringList& sl_Output, const structDataFormat DF[], const int i_Latitude, const int i_Longitude, const QString& s_DOI, const int iRecord )
{
    SHPObject	*psObject;

    int		nVertices		= 1;
    int		nParts          = 1;

    bool    b_hasDOI        = false;
    QString sd_DOI          = "";

    int		panParts[2];
    double	padfX[2];             // Longitude
    double	padfY[2];             // Latitude

// **********************************************************************************************

    panParts[0] = 0;

    if ( i_Latitude > -1 )
        padfY[0] = sl_Output.at( i_Latitude ).toDouble();
    else
        padfY[0] = 0;

    if ( i_Longitude > -1 )
        padfX[0] = sl_Output.at( i_Longitude ).toDouble();
    else
        padfX[0] = 0;

    psObject = SHPCreateObject( SHPT_POINT, -1, nParts, panParts, NULL, nVertices, padfX, padfY, NULL, NULL );

    SHPWriteObject( hSHP, -1, psObject );
    SHPDestroyObject( psObject );

    for ( int i=0; i<sl_Output.count(); ++i )
    {
       switch ( DF[i].type )
       {
         case _ISDOUBLE:
           DBFWriteDoubleAttribute( hDBF, iRecord, i, sl_Output.at( i ).toDouble() );
           break;

         case _ISDOI:
           b_hasDOI = true;
           sd_DOI   = sl_Output.at( i );
           DBFWriteStringAttribute( hDBF, iRecord, i, sd_DOI.replace( "http://doi.pangaea.de/", "doi:" ).toLatin1() );
           break;

         default:
           DBFWriteStringAttribute( hDBF, iRecord, i, sl_Output.at( i ).toLatin1() );
           break;
       }
    }

    if ( b_hasDOI == false )
        DBFWriteStringAttribute( hDBF, iRecord, sl_Output.count(), s_DOI.toLatin1() );

    return( iRecord + 1 );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2009-11-18

int MainWindow::writeShapfileMetadataEntry( DBFHandle hDBF, SHPHandle hSHP, const int i_Codec, const QString& s_Metadata, const int iRecordStart, const int i_NumOfFiles )
{
    QStringList     sl_DataMetadataList;

    structURL       URL[_MAX_NUM_OF_URLS+1];

    int iRecord     = iRecordStart;
    int err         = _NOERROR_;

// **********************************************************************************************

    if ( ( err = getDataMetadata( i_Codec, _FORMAT_SHAPE_DATA, s_Metadata, sl_DataMetadataList, URL, i_NumOfFiles ) ) == _NOERROR_ )
    {
        for( int i=0; i<sl_DataMetadataList.count(); ++i )
            iRecord = writeShapfileMetadataEntry( hDBF, hSHP, sl_DataMetadataList.at( i ), iRecord );
    }

    return( iRecord );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2009-11-18

int MainWindow::writeShapfileMetadataEntry( DBFHandle hDBF, SHPHandle hSHP, const QString& s_Metadata, const int iRecord )
{
    SHPObject	*psObject;

    int		nVertices		= 1;
    int		nParts          = 1;

    int		panParts[2];
    double	padfX[2];             // Longitude
    double	padfY[2];             // Latitude

// **********************************************************************************************

    panParts[0] = 0;
    padfY[0]    = s_Metadata.section( "\t", _LATITUDEPOS, _LATITUDEPOS ).toDouble();  // Latitude
    padfX[0]    = s_Metadata.section( "\t", _LONGITUDEPOS, _LONGITUDEPOS ).toDouble();  // Longitude
    psObject    = SHPCreateObject( SHPT_POINT, -1, nParts, panParts, NULL, nVertices, padfX, padfY, NULL, NULL );

    SHPWriteObject( hSHP, -1, psObject );
    SHPDestroyObject( psObject );

    DBFWriteStringAttribute( hDBF, iRecord, 0, s_Metadata.section( "\t", _EVENTLABELPOS, _EVENTLABELPOS ).toLatin1() );        // Event label
    DBFWriteStringAttribute( hDBF, iRecord, 1, s_Metadata.section( "\t", _AREANAMEPOS, _AREANAMEPOS ).toLatin1() );            // Area
    DBFWriteStringAttribute( hDBF, iRecord, 2, s_Metadata.section( "\t", _CAMPAIGNLABELPOS, _CAMPAIGNLABELPOS ).toLatin1() );  // Campaign
    DBFWriteDoubleAttribute( hDBF, iRecord, 3, s_Metadata.section( "\t", _LATITUDEPOS, _LATITUDEPOS ).toDouble() );           // Latitude
    DBFWriteDoubleAttribute( hDBF, iRecord, 4, s_Metadata.section( "\t", _LONGITUDEPOS, _LONGITUDEPOS ).toDouble() );         // Longitude
    DBFWriteStringAttribute( hDBF, iRecord, 5, s_Metadata.section( "\t", _DATEPOS, _DATEPOS ).toLatin1() );                    // Date
    DBFWriteStringAttribute( hDBF, iRecord, 6, s_Metadata.section( "\t", _TIMEPOS, _TIMEPOS ).toLatin1() );                    // Time
    DBFWriteDoubleAttribute( hDBF, iRecord, 7, s_Metadata.section( "\t", _ELEVATIONPOS, _ELEVATIONPOS ).toDouble() );         // Elevation
    DBFWriteStringAttribute( hDBF, iRecord, 8, s_Metadata.section( "\t", _GEARNAMEPOS, _GEARNAMEPOS ).toLatin1() );            // Device
    DBFWriteStringAttribute( hDBF, iRecord, 9, s_Metadata.section( "\t", _DOIPOS, _DOIPOS ).toLatin1() );                      // DOI

    return( iRecord + 1 );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2009-11-18

int MainWindow::writeDBFHeader( DBFHandle hDBF )
{
    DBFAddField( hDBF, "STATION", FTString, 80, 0 );	// Event label
    DBFAddField( hDBF, "AREA", FTString, 80, 0 );   	// Area
    DBFAddField( hDBF, "CAMPAIGN", FTString, 80, 0 );	// Campaign
    DBFAddField( hDBF, "LATITUDE", FTDouble, 10, 5 );   // Latitude
    DBFAddField( hDBF, "LONGITUDE", FTDouble, 10, 5 );  // Longitude
    DBFAddField( hDBF, "DATE", FTString, 10, 0 );       // Date
    DBFAddField( hDBF, "TIME", FTString, 10, 0 );       // Time
    DBFAddField( hDBF, "ELEVATION", FTDouble, 5, 0 );	// Elevation [m]
    DBFAddField( hDBF, "DEVICE", FTString, 80, 0 );     // Device
    DBFAddField( hDBF, "DOI", FTString, 80, 0 );        // DOI

    return( 10 ); // Number of header entries
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2009-11-18

int MainWindow::writeDBFHeader( DBFHandle hDBF, const QStringList& sl_ParameterList, const structDataFormat DF[] )
{
    QString s_Parameter = "xxx";

    bool    b_hasDOI    = false;

// **********************************************************************************************

    for ( int i=0; i<sl_ParameterList.count(); ++i )
    {
        if ( sl_ParameterList.at( i ).contains( _PSEPARATOR_ ) == true )
            s_Parameter = sl_ParameterList.at( i ).section( _PSEPARATOR_, 2, 2 ).section( " [", 0, 0 ).toUpper();
        else
            s_Parameter = sl_ParameterList.at( i ).section( " [", 0, 0 ).toUpper();

        s_Parameter.replace( " ", "" );
        s_Parameter.replace( ",", "" );
        s_Parameter.replace( "/", "" );

        switch ( DF[i].type )
        {
            case _ISDOUBLE:
                DBFAddField( hDBF, s_Parameter.toLatin1(), FTDouble, DF[i].width, DF[i].precision );
                break;

            case _ISDOI:
                DBFAddField( hDBF, "DOI", FTString, DF[i].width, 0 );
                b_hasDOI = true;
                break;

            default:
                DBFAddField( hDBF, s_Parameter.toLatin1(), FTString, DF[i].width, 0 );
                break;
        }
    }

    if ( b_hasDOI == false )
    {
        DBFAddField( hDBF, "DOI", FTString, 80, 0 );
        return( sl_ParameterList.count() );
    }

    return( sl_ParameterList.count() + 1 );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2009-11-18

int MainWindow::writeProjectionFile( const QString& s_Filename )
{
    QFile fprj( s_Filename );

    if ( fprj.open( QIODevice::WriteOnly | QIODevice::Text) == false )
        return( -20 );

    QTextStream tprj( &fprj );

    tprj << "GEOGCS[\"GCS_WGS_1984\",DATUM[\"D_WGS_1984\",SPHEROID[\"WGS_1984\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],UNIT[\"Degree\",0.0174532925199433]]";

    fprj.close();

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2009-11-19

void MainWindow::setDefaultFormats( const QStringList& sl_ParameterList, structDataFormat DF[] )
{
    for ( int i=0; i<sl_ParameterList.count(); ++i )
    {
        if ( sl_ParameterList.at( i ).contains( " [" ) == true )
        {
            DF[i].type      = _ISDOUBLE;
            DF[i].width     = 12;
            DF[i].precision = 3;
        }
        else
        {
            DF[i].type      = _ISSTRING;
            DF[i].width     = 80;
            DF[i].precision = 0;
        }

        if ( sl_ParameterList.at( i ).contains( "salinity", Qt::CaseInsensitive ) == true )
        {
            DF[i].type      = _ISDOUBLE;
            DF[i].width     = 12;
            DF[i].precision = 3;
        }

        if ( sl_ParameterList.at( i ).contains( "latitude", Qt::CaseInsensitive ) == true )
        {
            DF[i].type      = _ISDOUBLE;
            DF[i].width     = 10;
            DF[i].precision = 5;
        }

        if ( sl_ParameterList.at( i ).contains( "longitude", Qt::CaseInsensitive ) == true )
        {
            DF[i].type      = _ISDOUBLE;
            DF[i].width     = 10;
            DF[i].precision = 5;
        }

        if ( sl_ParameterList.at( i ).contains( "elevation", Qt::CaseInsensitive ) == true )
        {
            DF[i].type      = _ISDOUBLE;
            DF[i].width     = 5;
            DF[i].precision = 0;
        }

        if ( sl_ParameterList.at( i ).contains( "doi", Qt::CaseInsensitive ) == true )
        {
            DF[i].type      = _ISDOI;
            DF[i].width     = 80;
            DF[i].precision = 0;
        }

        if ( sl_ParameterList.at( i ).contains( "origin of values", Qt::CaseInsensitive ) == true )
        {
            DF[i].type      = _ISDOI;
            DF[i].width     = 80;
            DF[i].precision = 0;
        }
    }
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2009-11-17

/*! @brief Steuert den Converter */

void MainWindow::doCreateShapefileMetadata()
{
    int         i_File              = 0;
    int         err                 = 0;
    int         stopProgress      = 0;

    int         i_EnvArray[_MAX_NUM_OF_ENV+1];

    QStringList	sl_MetadataList;
    QStringList	sl_ParameterList;

// **********************************************************************************************

    for ( int i=0; i<=_MAX_NUM_OF_ENV; ++i )
        i_EnvArray[i] = -1;

    sl_MetadataList.append( tr( "File name\tMetaInfo\tHeader lines\tEnvironment\tCampaign\tEvent label\tGear\tDate\tTime\tDate/Time\tDate/Time start\tLatitude\tLongitude\tElevation\tArea\tDOI\tCitation\tDatePos\tTimePos\tDateTimePos\tElevationPos\tEventLabelPos\tCampaignLabelPos\tAreaNamePos\tGearNamePos\tLatitudePos\tLongitudePos" ) );

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        err = setFilename( _FORMAT_SHAPE_METADATA, gb_CuratorMode, gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), gs_FilenameShapefile );

        if ( err == _NOERROR_ )
        {
            initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Creating shapefile..." ) );

            while ( ( i_File < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
            {
                setStatusBarFileInProgress( gsl_FilenameList.at( i_File ) );

                err = analyzeDataset( gsl_FilenameList.at( i_File ), gi_CodecInput, false, false, false, sl_MetadataList, sl_ParameterList, i_EnvArray );

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

    if ( ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
        err = createShapefileMetadata( sl_MetadataList, gs_FilenameShapefile, gi_CodecInput, sl_MetadataList.count()-1 );

// **********************************************************************************************

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Converter was canceled" ), false, false );

// **********************************************************************************************

    onError( err );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2009-11-19

/*! @brief Steuert den Converter */

void MainWindow::doCreateShapefileData()
{
    int                 err                 = _NOERROR_;

    int                 i_File              = 0;
    int                 i_Env               = 0;
    int                 stopProgress      = 0;

    int                 i_EnvArray[_MAX_NUM_OF_ENV+1];

    structDataFormat    DF[_MAX_NUM_OF_PARAMETERS+1];

    QStringList         sl_MetadataList;
    QStringList         sl_ParameterList;
    QStringList         sl_selectedParameters;

// **********************************************************************************************

    for ( int i=0; i<=_MAX_NUM_OF_ENV; ++i )
        i_EnvArray[i] = -1;

    for ( int i=0; i<=_MAX_NUM_OF_PARAMETERS; ++i )
    {
        DF[i].type      = _ISSTRING;
        DF[i].width     = 20;
        DF[i].precision = 0;
    }

// **********************************************************************************************

    sl_MetadataList.append( tr( "File name\tMetaInfo\tHeader lines\tEnvironment\tCampaign\tEvent label\tGear\tDate\tTime\tDate/Time\tDate/Time start\tLatitude\tLongitude\tElevation\tArea\tDOI\tCitation\tDatePos\tTimePos\tDateTimePos\tElevationPos\tEventLabelPos\tCampaignLabelPos\tAreaNamePos\tGearNamePos\tLatitudePos\tLongitudePos" ) );

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        err = setFilename( _FORMAT_SHAPE_DATA, gb_CuratorMode, gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), gs_FilenameShapefile );

        if ( err == _NOERROR_ )
        {
            initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Creating shapefile..." ) );

            while ( ( i_File < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
            {
                setStatusBarFileInProgress( gsl_FilenameList.at( i_File ) );

                err = analyzeDataset( gsl_FilenameList.at( i_File ), gi_CodecInput, false, false, false, sl_MetadataList, sl_ParameterList, i_EnvArray );

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
// Set data format

    if ( ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
        setDefaultFormats( sl_selectedParameters, DF );

// **********************************************************************************************
// create shapefile

    if ( ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
        err = createShapefileData( sl_MetadataList, sl_selectedParameters, DF, gs_FilenameShapefile, gi_CodecInput, i_Env, gb_setGeocode, gf_GEOCODE_min, gf_GEOCODE_max, gs_GEOCODE_min, gs_GEOCODE_max, gb_setGearID, gi_GearID, sl_MetadataList.count()-1 );

// **********************************************************************************************

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Converter was canceled" ), false, false );

// **********************************************************************************************

    onError( err );
}
