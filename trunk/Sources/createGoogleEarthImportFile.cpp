// ***********************************************************************************************
// *                                                                                             *
// * createGoogleEarthImportFile.cpp - Create a Google Earth import file (KML)                   *
// *                                                                                             *
// * Dr. Rainer Sieger - 2009-03-24                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include "Application.h"

/*! @brief Liest Zeilen aus einer Datei, verarbeitet sie und schreibt sie in eine neue Datei. */

int MainWindow::createGoogleEarthImportFile( QStringList& sl_MetadataList, const QString& s_FilenameOut, int i_CodecInput,
                                             const bool b_displayEventLabel, const bool b_displayDescription, const int i_IconSize,
                                             const int i_IconColor, const int i_TracklineWidth, const int i_TracklineColor,
                                             const int i_NumOfFiles )
{
    int             i                       = 1;
    int             stopProgress            = 0;
    int             err                     = _NOERROR_;
    int             i_Start                 = 0;
    int             i_NumOfOpenKMLFolders   = 0;

    float           f_IconSize              = (float) i_IconSize/100.;

    QString         s_Campaign              = "";

    QStringList     sl_DataMetadataList;

    structURL       URL[_MAX_NUM_OF_URLS+1];

//-----------------------------------------------------------------------------------------------------------------------

    QFile fkml( s_FilenameOut );

    if ( fkml.open( QIODevice::WriteOnly | QIODevice::Text) == false )
        return( -20 );

//-----------------------------------------------------------------------------------------------------------------------

    initFileProgress( i_NumOfFiles, gsl_FilenameList.at( 0 ), tr( "Creating Google Earth import file..." ) );

//-----------------------------------------------------------------------------------------------------------------------

    clearURL( URL );

//-----------------------------------------------------------------------------------------------------------------------

    err = openKMLFile( fkml );

//-----------------------------------------------------------------------------------------------------------------------
// first file

    if ( sl_MetadataList.at( 1 ).section( "\t", _METAINFOPOS, _METAINFOPOS ) == "1" )
    {
        // no metadata in data
        s_Campaign = sl_MetadataList.at( 1 ).section( "\t", _CAMPAIGNLABELPOS, _CAMPAIGNLABELPOS );

        i_NumOfOpenKMLFolders += openKMLFolder( fkml, s_Campaign );

        if ( f_IconSize > 0. )
            err = writeKMLEntry( fkml, sl_MetadataList, b_displayEventLabel, b_displayDescription, f_IconSize, i_IconColor, URL, 1 );
    }
    else
    {
        // metadata in data
        err = getDataMetadata( i_CodecInput, _FORMAT_KMLFILE, sl_MetadataList.at( 1 ), sl_DataMetadataList, URL, i_NumOfFiles );

        s_Campaign = sl_DataMetadataList.at( 0 ).section( "\t", _CAMPAIGNLABELPOS, _CAMPAIGNLABELPOS );

        i_NumOfOpenKMLFolders += openKMLFolder( fkml, s_Campaign );

        if ( f_IconSize > 0. )
            err = writeKMLEntry( fkml, sl_DataMetadataList, b_displayEventLabel, b_displayDescription, f_IconSize, i_IconColor, URL, 0 );

        i_Start = 0;

        for ( int j=1; j<sl_DataMetadataList.count(); ++j )
        {
            if ( s_Campaign != sl_DataMetadataList.at( j ).section( "\t", _CAMPAIGNLABELPOS, _CAMPAIGNLABELPOS ) )
            {
                s_Campaign = sl_DataMetadataList.at( j ).section( "\t", _CAMPAIGNLABELPOS, _CAMPAIGNLABELPOS );

                if ( i_NumOfOpenKMLFolders == 2 )
                    i_NumOfOpenKMLFolders -= closeKMLFolder( fkml, 4 );

                if ( ( sl_DataMetadataList.count() > 1 ) && ( i_TracklineWidth > 0 ) )
                    i_Start = writeKMLTrack( fkml, sl_DataMetadataList, i_TracklineWidth, i_TracklineColor, i_Start, j );

                if ( i_NumOfOpenKMLFolders == 1 )
                    i_NumOfOpenKMLFolders -= closeKMLFolder( fkml, 2 );

                i_NumOfOpenKMLFolders += openKMLFolder( fkml, s_Campaign );
            }

            if ( f_IconSize > 0. )
                err = writeKMLEntry( fkml, sl_DataMetadataList, b_displayEventLabel, b_displayDescription, f_IconSize, i_IconColor, URL, j );
        }

        if ( i_NumOfOpenKMLFolders == 2 )
            i_NumOfOpenKMLFolders -= closeKMLFolder( fkml, 4 );

        if ( ( sl_DataMetadataList.count() > 1 ) && ( i_TracklineWidth > 0 ) )
            writeKMLTrack( fkml, sl_DataMetadataList, i_TracklineWidth, i_TracklineColor, i_Start, sl_DataMetadataList.count() );
    }

//-----------------------------------------------------------------------------------------------------------------------
// all files

    while ( ( ++i < sl_MetadataList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
    {
        clearURL( URL );

        if ( sl_MetadataList.at( i ).section( "\t", _METAINFOPOS, _METAINFOPOS ) == "1" )
        {
            // no metadata in data
            if ( s_Campaign != sl_MetadataList.at( i ).section( "\t", _CAMPAIGNLABELPOS, _CAMPAIGNLABELPOS ) )
            {
                if ( i_NumOfOpenKMLFolders == 2 )
                    i_NumOfOpenKMLFolders -= closeKMLFolder( fkml, 4 );

                if ( i_NumOfOpenKMLFolders == 1 )
                    i_NumOfOpenKMLFolders -= closeKMLFolder( fkml, 2 );

                s_Campaign = sl_MetadataList.at( i ).section( "\t", _CAMPAIGNLABELPOS, _CAMPAIGNLABELPOS );

                i_NumOfOpenKMLFolders += openKMLFolder( fkml, s_Campaign );
            }

            if ( f_IconSize > 0. )
                err = writeKMLEntry( fkml, sl_MetadataList, b_displayEventLabel, b_displayDescription, f_IconSize, i_IconColor, URL, i );
        }
        else
        {
            // metadata in data
            err = getDataMetadata( i_CodecInput, _FORMAT_KMLFILE, sl_MetadataList.at( i ), sl_DataMetadataList, URL, i_NumOfFiles );

            if ( s_Campaign != sl_DataMetadataList.at( 0 ).section( "\t", _CAMPAIGNLABELPOS, _CAMPAIGNLABELPOS ) )
            {
                s_Campaign = sl_DataMetadataList.at( 0 ).section( "\t", _CAMPAIGNLABELPOS, _CAMPAIGNLABELPOS );

                if ( i_NumOfOpenKMLFolders == 2 )
                    i_NumOfOpenKMLFolders -= closeKMLFolder( fkml, 4 );

                if ( i_NumOfOpenKMLFolders == 1 )
                    i_NumOfOpenKMLFolders -= closeKMLFolder( fkml, 2 );

                i_NumOfOpenKMLFolders += openKMLFolder( fkml, s_Campaign );
            }

            if ( f_IconSize > 0. )
                err = writeKMLEntry( fkml, sl_DataMetadataList, b_displayEventLabel, b_displayDescription, f_IconSize, i_IconColor, URL, 0 );

            i_Start = 0;

            for ( int j=1; j<sl_DataMetadataList.count(); ++j )
            {
                if ( s_Campaign != sl_DataMetadataList.at( j ).section( "\t", _CAMPAIGNLABELPOS, _CAMPAIGNLABELPOS ) )
                {
                    s_Campaign = sl_DataMetadataList.at( j ).section( "\t", _CAMPAIGNLABELPOS, _CAMPAIGNLABELPOS );

                    if ( i_NumOfOpenKMLFolders == 2 )
                        i_NumOfOpenKMLFolders -= closeKMLFolder( fkml, 4 );

                    if ( ( sl_DataMetadataList.count() > 1 ) && ( i_TracklineWidth > 0 ) )
                        i_Start = writeKMLTrack( fkml, sl_DataMetadataList, i_TracklineWidth, i_TracklineColor, i_Start, j );

                    if ( i_NumOfOpenKMLFolders == 1 )
                        i_NumOfOpenKMLFolders -= closeKMLFolder( fkml, 2 );

                    i_NumOfOpenKMLFolders += openKMLFolder( fkml, s_Campaign );
                }

                if ( f_IconSize > 0. )
                    err = writeKMLEntry( fkml, sl_DataMetadataList, b_displayEventLabel, b_displayDescription, f_IconSize, i_IconColor, URL, j );
            }

            if ( i_NumOfOpenKMLFolders == 2 )
                i_NumOfOpenKMLFolders -= closeKMLFolder( fkml, 4 );

            if ( ( sl_DataMetadataList.count() > 1 ) && ( i_TracklineWidth > 0 ) )
                writeKMLTrack( fkml, sl_DataMetadataList, i_TracklineWidth, i_TracklineColor, i_Start, sl_DataMetadataList.count() );
        }

        stopProgress = incFileProgress( i_NumOfFiles, i );
    }

//-----------------------------------------------------------------------------------------------------------------------

    resetFileProgress( i_NumOfFiles );

    if ( i_NumOfOpenKMLFolders == 2 )
        i_NumOfOpenKMLFolders -= closeKMLFolder( fkml, 4 );

    if ( i_NumOfOpenKMLFolders == 1 )
        i_NumOfOpenKMLFolders -= closeKMLFolder( fkml, 2 );

    err = closeKMLFile( fkml );

    fkml.close();

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void MainWindow::clearURL( structURL URL[] )
{
    for ( int i=0; i<=_MAX_NUM_OF_URLS; ++i )
    {
        URL[i].position = -1;
        URL[i].TitleURL = "";
    }
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

int MainWindow::openKMLFile( QFile& fkml )
{
    QTextStream tkml( &fkml );
    tkml.setCodec("UTF-8");

    tkml << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    tkml << "<kml xmlns=\"http://earth.google.com/kml/2.0\">\n";
    tkml << "<Document>\n";
    tkml << "  <ScreenOverlay>\n";
    tkml << "    <name>Pan2Applic logo</name>\n";
    tkml << "    <Icon><href>http://www.pangaea.de/PICS/Pan2Applic_logo.png</href></Icon>\n";
    tkml << "    <overlayXY x=\"0\" y=\"1\" xunits=\"fraction\" yunits=\"fraction\"/>\n";
    tkml << "    <screenXY x=\"5\" y=\"5\" xunits=\"pixels\" yunits=\"insetPixels\"/>\n";
    tkml << "    <size x=\"200\" y=\"88\" xunits=\"pixel\" yunits=\"pixel\"/>\n";
    tkml << "  </ScreenOverlay>\n";
    tkml << "  <Style id=\"red\"><IconStyle><Icon><href>http://www.pangaea.de/PICS/circle-red.png</href></Icon></IconStyle></Style>\n";
    tkml << "  <Style id=\"blue\"><IconStyle><Icon><href>http://www.pangaea.de/PICS/circle-blue.png</href></Icon></IconStyle></Style>\n";
    tkml << "  <Style id=\"green\"><IconStyle><Icon><href>http://www.pangaea.de/PICS/circle-green.png</href></Icon></IconStyle></Style>\n";
    tkml << "  <Style id=\"orange\"><IconStyle><Icon><href>http://www.pangaea.de/PICS/circle-orange.png</href></Icon></IconStyle></Style>\n";
    tkml << "  <Style id=\"yellow\"><IconStyle><Icon><href>http://www.pangaea.de/PICS/circle-yellow.png</href></Icon></IconStyle></Style>\n";
//  tkml << "  <name>Document name</name>\n";
//  tkml << "  <description>Document description</description>\n";

    return( 0 );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

int MainWindow::closeKMLFile( QFile& fkml )
{
    QTextStream tkml( &fkml );
    tkml.setCodec("UTF-8");

    tkml << "</Document>\n";
    tkml << "</kml>\n";

    return( 0 );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

int MainWindow::openKMLFolder( QFile& fkml, const QString& s_Campaign )
{
    QTextStream tkml( &fkml );
    tkml.setCodec("UTF-8");

    tkml << "  <Folder>\n";

    if ( ( s_Campaign != "xxx" ) && ( s_Campaign != "unknown campaign" ) )
    {
        tkml << "    <name>Campaign: " << s_Campaign.section( " (", 0, 0 ) << "</name>\n";
        tkml << "    <description><![CDATA[<a href=\"http://www.pangaea.de/search?q=" << s_Campaign.section( " (", 0, 0 ) << "\">search all Datasets</a>]]></description>\n";
    }
    else
    {
        tkml << "    <name>Campaign unknown</name>\n";
    }

    tkml << "    <Folder>\n";
    tkml << "      <name>Events</name>\n";
    tkml << "      <open>0</open>\n";

    return( 2 );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

int MainWindow::closeKMLFolder( QFile& fkml, const int n )
{
    QTextStream tkml( &fkml );
    tkml.setCodec("UTF-8");

    switch ( n )
    {
      case 4:
        tkml << "    </Folder>\n";
        break;
      default:
        tkml << "  </Folder>\n";
        break;
    }


    return( 1 );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

int MainWindow::writeKMLTrack( QFile& fkml, const QStringList& sl_MetadataList, const int i_TracklineWidth, const int i_TracklineColor, const int i_Start, const int i_End )
{
    if ( i_Start+1 < i_End )
    {
        QTextStream tkml( &fkml );
        tkml.setCodec("UTF-8");

        tkml << "    <Placemark><name>Trackline</name>" << endl;

        switch ( i_TracklineColor )
        {
            case _RED:
                tkml << "      <Style><LineStyle><color>ff0000ff</color>";
                break;
            case _GREEN:
                tkml << "      <Style><LineStyle><color>ff00ff00</color>";
                break;
            case _BLUE:
                tkml << "      <Style><LineStyle><color>ffff0000</color>";
                break;
            case _ORANGE:
                tkml << "      <Style><LineStyle><color>ff0099ff</color>";
                break;
            case _YELLOW:
                tkml << "      <Style><LineStyle><color>ff33ffff</color>";
                break;
            default:
                tkml << "      <Style><LineStyle><color>ff0099ff</color>";
                break;
        }

        tkml << "<width>" << (float) i_TracklineWidth/10. << "</width></LineStyle></Style>" << endl;
        tkml << "      <LineString>" << endl;
        tkml << "        <tessellate>1</tessellate>" << endl;
        tkml << "        <coordinates>" << endl;

        for ( int i=i_Start; i<i_End; i++ )
        {
            double d_Latitude  = sl_MetadataList.at( i ).section( "\t", _LATITUDEPOS, _LATITUDEPOS ).toDouble();
            double d_Longitude = sl_MetadataList.at( i ).section( "\t", _LONGITUDEPOS, _LONGITUDEPOS ).toDouble();
            double d_Elevation  = sl_MetadataList.at( i ).section( "\t", _ELEVATIONPOS, _ELEVATIONPOS ).toDouble();

            if ( d_Elevation < 0 )
                d_Elevation = 0.0;

            if ( d_Longitude > 180 )
                d_Longitude -= 360;

            tkml << "          " << QString( "%1,%2,%3" ).arg( d_Longitude, 0, 'f', 5 ).arg( d_Latitude, 0, 'f', 5 ).arg( d_Elevation, 0, 'f', 1 ) << endl;
        }

        tkml << "        </coordinates>" << endl;
        tkml << "      </LineString>" << endl;
        tkml << "    </Placemark>" << endl;
    }

    return( i_End );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

int MainWindow::writeKMLEntry( QFile& fkml, const QStringList& sl_MetadataList, const bool b_displayEventLabel, const bool b_displayDescription, const float f_IconSize, const int i_IconColor, structURL URL[], const int i )
{
    double d_Latitude      = sl_MetadataList.at( i ).section( "\t", _LATITUDEPOS, _LATITUDEPOS ).toDouble();   // Latitude
    double d_Longitude     = sl_MetadataList.at( i ).section( "\t", _LONGITUDEPOS, _LONGITUDEPOS ).toDouble(); // Longitude
    double d_Elevation     = sl_MetadataList.at( i ).section( "\t", _ELEVATIONPOS, _ELEVATIONPOS ).toDouble(); // Elevation

    QString s_CampaignLabel = sl_MetadataList.at( i ).section( "\t", _CAMPAIGNLABELPOS, _CAMPAIGNLABELPOS );   // Campaign
    QString s_EventLabel    = sl_MetadataList.at( i ).section( "\t", _EVENTLABELPOS, _EVENTLABELPOS );         // Event label
    QString s_GearName      = sl_MetadataList.at( i ).section( "\t", _GEARNAMEPOS, _GEARNAMEPOS );             // Gear name
    QString s_Date          = sl_MetadataList.at( i ).section( "\t", _DATEPOS, _DATEPOS );                     // Date
    QString s_Time          = sl_MetadataList.at( i ).section( "\t", _TIMEPOS, _TIMEPOS );                     // Time
    QString s_DateTime      = sl_MetadataList.at( i ).section( "\t", _DATETIMEPOS, _DATETIMEPOS );             // Date/Time
    QString s_DateTimeStart = sl_MetadataList.at( i ).section( "\t", _DATETIMESTARTPOS, _DATETIMESTARTPOS );   // Date/Time start
    QString s_AreaName      = sl_MetadataList.at( i ).section( "\t", _AREANAMEPOS, _AREANAMEPOS );             // Area
    QString s_DOI           = sl_MetadataList.at( i ).section( "\t", _DOIPOS, _DOIPOS );                       // DOI
    QString s_Citation      = sl_MetadataList.at( i ).section( "\t", _CITATIONPOS, _CITATIONPOS );             // Citation

// **********************************************************************************************

    if ( s_Time == s_Date )
        s_Time = "xxx";

    if ( d_Longitude > 180 )
        d_Longitude -= 360;

    QString s_Longitude = QString( "%1" ).arg( d_Longitude, 0, 'f', 5 );
    QString s_Latitude  = QString( "%1" ).arg( d_Latitude, 0, 'f', 5 );
    QString s_Elevation = QString( "%1" ).arg( d_Elevation, 0, 'f', 1 );

// **********************************************************************************************

    if ( i>0 )
    {
        if ( sl_MetadataList.at( i-1 ).section( "\t", _EVENTLABELPOS, _EVENTLABELPOS ) == sl_MetadataList.at( i ).section( "\t", _EVENTLABELPOS, _EVENTLABELPOS ) )
            s_EventLabel.append( " (line " + sl_MetadataList.at( i ).section( "\t", _NUMOFHEADERLINESPOS, _NUMOFHEADERLINESPOS ) + ")" );
    }
    else
    {
        if ( sl_MetadataList.count() > 1 )
        {
            if ( sl_MetadataList.at( 0 ).section( "\t", _EVENTLABELPOS, _EVENTLABELPOS ) == sl_MetadataList.at( 1 ).section( "\t", _EVENTLABELPOS, _EVENTLABELPOS ) )
                s_EventLabel.append( " (line " + sl_MetadataList.at( i ).section( "\t", _NUMOFHEADERLINESPOS, _NUMOFHEADERLINESPOS ) + ")" );
        }
    }

    QTextStream tkml( &fkml );
    tkml.setCodec("UTF-8");

    tkml << "      <Placemark>";

    if ( b_displayEventLabel == true )
        tkml << "<name>" << s_EventLabel << "</name>";

    if ( b_displayDescription == true )
    {
        tkml << "<description>";

        if ( ( s_Citation.isEmpty() == false ) && ( s_Citation != "xxx" ) )
        {
            if ( b_displayEventLabel == false )
                tkml << "<b>" << s_EventLabel << "</b><br />";

            if ( ( s_DOI.isEmpty() == false ) && ( s_DOI != "xxx" ) )
            {
                tkml << "<![CDATA[<a href=\"";

                if ( s_DOI.startsWith( "doi:" ) == true )
                    tkml << "http://doi.pangaea.de/" << s_DOI.section( "doi:", 1, 1 ).section( ",", 0, 0 );  // published or DOI registration in progress
                else
                    tkml << "http://www.pangaea.de/search?q=dataset" << s_DOI.section( ": ", 1, 1 ).section( ",", 0, 0 ); // unpublished datset

                if ( s_DOI.section( ", ", 1, 1 ).isEmpty() == true )
                    tkml << "\">" << s_DOI.section( ",", 0, 0 ) << "</a>]]><br />";
                else
                    tkml << "\">" << s_DOI.section( ",", 0, 0 ) << "</a>]]>, " << s_DOI.section( ", ", 1, 1 ) << "<br />";
            }

            tkml << s_Citation;
            tkml << "<br />";
        }
        else
        {
            if ( b_displayEventLabel == false )
                tkml << "Event: " << s_EventLabel << " * ";

            if ( ( s_CampaignLabel.isEmpty() == false ) && ( s_CampaignLabel != "xxx" ) && ( s_CampaignLabel != "unknown campaign" ) )
                tkml << "Campaign: " << s_CampaignLabel << " * ";

            tkml << "Latitude: " << s_Latitude << " * ";
            tkml << "Longitude: " << s_Longitude << " * ";

            if ( ( s_AreaName.isEmpty() == false ) && ( s_AreaName != "xxx" ) )
                tkml << "Area: " << s_AreaName << " * ";

            if ( ( s_GearName.isEmpty() == false ) && ( s_GearName != "xxx" ) )
                tkml << "Gear: " << s_GearName << " * ";

            if ( ( s_Date.isEmpty() == false ) && ( s_Date != "xxx" ) )
            {
                if ( s_Date.contains( ":" ) == true )
                    tkml << "Date/Time: " << s_Date << " * ";
                else
                    tkml << "Date: " << s_Date << " * ";
            }

            if ( ( s_Time.isEmpty() == false ) && ( s_Time != "xxx" ) )
                tkml << "Time: " << s_Time << " * ";

            if ( ( s_Elevation.isEmpty() == false ) && ( s_Elevation != "xxx" ) )
                tkml << "Elevation: " << s_Elevation << " m";
            else
                tkml << "Elevation: unknown";

            tkml << "<br /><![CDATA[<a href=\"";
            tkml << "http://www.pangaea.de/search?q=" << s_EventLabel.section( " (line ", 0, 0 ); // Event
            tkml << "\">search data</a>]]><br />";
        }

        for ( int j=1; j<=_MAX_NUM_OF_URLS; ++j )
        {

            if ( URL[j].position >= 0 )
            {
                if ( sl_MetadataList.at( i ).section( "\t", _CITATIONPOS+j, _CITATIONPOS+j ).isEmpty() == false )
                    tkml << "<![CDATA[<a href=\"" << sl_MetadataList.at( i ).section( "\t", _CITATIONPOS+j, _CITATIONPOS+j ) << "\">" << URL[j].TitleURL << "</a>]]><br />";
            }
        }

        tkml << "</description>";
    }

    tkml << "<View>";
    tkml << "<longitude>" << s_Longitude << "</longitude>";
    tkml << "<latitude>" << s_Latitude << "</latitude>";
    tkml << "</View>";

    switch ( i_IconColor )
    {
        case _RED:
            tkml << "<styleUrl>#red</styleUrl>";
            break;
        case _GREEN:
            tkml << "<styleUrl>#green</styleUrl>";
            break;
        case _BLUE:
            tkml << "<styleUrl>#blue</styleUrl>";
            break;
        case _ORANGE:
            tkml << "<styleUrl>#orange</styleUrl>";
            break;
        case _YELLOW:
            tkml << "<styleUrl>#yellow</styleUrl>";
            break;
        default:
            tkml << "<styleUrl>#red</styleUrl>";
            break;
    }

    tkml << "<Style><IconStyle>" << QString( "<scale>%1</scale>" ).arg( f_IconSize ) << "</IconStyle></Style>";
    tkml << "<Point><coordinates>" << s_Longitude << "," << s_Latitude << ",0</coordinates></Point>";
    tkml << "</Placemark>\n";

    return( 0 );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 02.08.2003

/*! @brief Steuert den Converter */

void MainWindow::doCreateGoogleEarthImportFile()
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
        err = setFilename( _FORMAT_KMLFILE, gb_CuratorMode, gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), gs_FilenameGoogleEarth );

        if ( err == _NOERROR_ )
        {
            initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Creating Google Earth import file..." ) );

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
        err = createGoogleEarthImportFile( sl_MetadataList, gs_FilenameGoogleEarth, gi_CodecInput, gb_displayEventLabel, gb_displayDescription, gi_IconSize, gi_IconColor, gi_TracklineWidth, gi_TracklineColor, sl_MetadataList.count()-1 );

// **********************************************************************************************

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Converter was canceled" ), false, false );

// **********************************************************************************************
// start Google Earth

    if ( ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) && ( gb_startGoogleEarth == true ) )
        err = startGoogleEarth( gs_FilenameGoogleEarthProgram, gs_FilenameGoogleEarth );

// **********************************************************************************************

    onError( err );
}
