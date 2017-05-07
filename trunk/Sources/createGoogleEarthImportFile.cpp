// ***********************************************************************************************
// *                                                                                             *
// * createGoogleEarthImportFile.cpp - Create a Google Earth import file (KML)                   *
// *                                                                                             *
// * Dr. Rainer Sieger - 2009-03-24                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include "Application.h"

/*! @brief Liest Zeilen aus einer Datei, verarbeitet sie und schreibt sie in eine neue Datei. */

int MainWindow::createGoogleEarthImportFile( QStringList &sl_MetadataList, const QString &s_FilenameOut, int i_CodecInput,
                                             const bool b_displayEventLabel, const bool b_displayDescription, const int i_IconSize,
                                             const int i_IconColor, const int i_IconSymbol, const int i_TracklineWidth, const int i_TracklineColor,
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
            err = writeKMLEntry( fkml, sl_MetadataList, b_displayEventLabel, b_displayDescription, f_IconSize, i_IconColor, i_IconSymbol, URL, 1 );
    }
    else
    {
        // metadata in data
        err = getDataMetadata( i_CodecInput, _FORMAT_KMLFILE, sl_MetadataList.at( 1 ), sl_DataMetadataList, URL, i_NumOfFiles );

        s_Campaign = sl_DataMetadataList.at( 0 ).section( "\t", _CAMPAIGNLABELPOS, _CAMPAIGNLABELPOS );

        i_NumOfOpenKMLFolders += openKMLFolder( fkml, s_Campaign );

        if ( f_IconSize > 0. )
            err = writeKMLEntry( fkml, sl_DataMetadataList, b_displayEventLabel, b_displayDescription, f_IconSize, i_IconColor, i_IconSymbol,  URL, 0 );

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
                err = writeKMLEntry( fkml, sl_DataMetadataList, b_displayEventLabel, b_displayDescription, f_IconSize, i_IconColor, i_IconSymbol, URL, j );
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
                err = writeKMLEntry( fkml, sl_MetadataList, b_displayEventLabel, b_displayDescription, f_IconSize, i_IconColor, i_IconSymbol, URL, i );
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
                err = writeKMLEntry( fkml, sl_DataMetadataList, b_displayEventLabel, b_displayDescription, f_IconSize, i_IconColor, i_IconSymbol, URL, 0 );

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
                    err = writeKMLEntry( fkml, sl_DataMetadataList, b_displayEventLabel, b_displayDescription, f_IconSize, i_IconColor, i_IconSymbol, URL, j );
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

    tkml << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
    tkml << "<kml xmlns=\"http://earth.google.com/kml/2.1\">" << endl;
    tkml << "<Document>" << endl;
    tkml << "  <ScreenOverlay>" << endl;
    tkml << "    <name>Pan2Applic logo</name>" << endl;
    tkml << "    <Icon><href>http://epic.awi.de/40953/121/Pan2Applic_logo.png</href></Icon>" << endl;
    tkml << "    <overlayXY x=\"0\" y=\"1\" xunits=\"fraction\" yunits=\"fraction\"/>" << endl;
    tkml << "    <screenXY x=\"5\" y=\"5\" xunits=\"pixels\" yunits=\"insetPixels\"/>" << endl;
    tkml << "    <size x=\"200\" y=\"88\" xunits=\"pixel\" yunits=\"pixel\"/>" << endl;
    tkml << "  </ScreenOverlay>" << endl;
    tkml << "  <Style id=\"circle-blue\"><IconStyle><Icon><href>http://epic.awi.de/40953/1/circle-blue.png</href></Icon></IconStyle></Style>" << endl;
    tkml << "  <Style id=\"circle-green\"><IconStyle><Icon><href>http://epic.awi.de/40953/2/circle-green.png</href></Icon></IconStyle></Style>" << endl;
    tkml << "  <Style id=\"circle-orange\"><IconStyle><Icon><href>http://epic.awi.de/40953/3/circle-orange.png</href></Icon></IconStyle></Style>" << endl;
    tkml << "  <Style id=\"circle-red\"><IconStyle><Icon><href>http://epic.awi.de/40953/4/circle-red.png</href></Icon></IconStyle></Style>" << endl;
    tkml << "  <Style id=\"circle-white\"><IconStyle><Icon><href>http://epic.awi.de/40953/5/circle-white.png</href></Icon></IconStyle></Style>" << endl;
    tkml << "  <Style id=\"circle-yellow\"><IconStyle><Icon><href>http://epic.awi.de/40953/6/circle-yellow.png</href></Icon></IconStyle></Style>" << endl;
    tkml << "  <Style id=\"square-blue\"><IconStyle><Icon><href>http://epic.awi.de/40953/7/square-blue.png</href></Icon></IconStyle></Style>" << endl;
    tkml << "  <Style id=\"square-green\"><IconStyle><Icon><href>http://epic.awi.de/40953/8/square-green.png</href></Icon></IconStyle></Style>" << endl;
    tkml << "  <Style id=\"square-orange\"><IconStyle><Icon><href>http://epic.awi.de/40953/9/square-orange.png</href></Icon></IconStyle></Style>" << endl;
    tkml << "  <Style id=\"square-red\"><IconStyle><Icon><href>http://epic.awi.de/40953/10/square-red.png</href></Icon></IconStyle></Style>" << endl;
    tkml << "  <Style id=\"square-white\"><IconStyle><Icon><href>http://epic.awi.de/40953/11/square-white.png</href></Icon></IconStyle></Style>" << endl;
    tkml << "  <Style id=\"square-yellow\"><IconStyle><Icon><href>http://epic.awi.de/40953/12/square-yellow.png</href></Icon></IconStyle></Style>" << endl;
    tkml << "  <Style id=\"star-blue\"><IconStyle><Icon><href>http://epic.awi.de/40953/13/star-blue.png</href></Icon></IconStyle></Style>" << endl;
    tkml << "  <Style id=\"star-green\"><IconStyle><Icon><href>http://epic.awi.de/40953/14/star-green.png</href></Icon></IconStyle></Style>" << endl;
    tkml << "  <Style id=\"star-orange\"><IconStyle><Icon><href>http://epic.awi.de/40953/15/star-orange.png</href></Icon></IconStyle></Style>" << endl;
    tkml << "  <Style id=\"star-red\"><IconStyle><Icon><href>http://epic.awi.de/40953/16/star-red.png</href></Icon></IconStyle></Style>" << endl;
    tkml << "  <Style id=\"star-white\"><IconStyle><Icon><href>http://epic.awi.de/40953/17/star-white.png</href></Icon></IconStyle></Style>" << endl;
    tkml << "  <Style id=\"star-yellow\"><IconStyle><Icon><href>http://epic.awi.de/40953/18/star-yellow.png</href></Icon></IconStyle></Style>" << endl;
    tkml << "  <Style id=\"triangle-blue\"><IconStyle><Icon><href>http://epic.awi.de/40953/19/triangle-blue.png</href></Icon></IconStyle></Style>" << endl;
    tkml << "  <Style id=\"triangle-green\"><IconStyle><Icon><href>http://epic.awi.de/40953/20/triangle-green.png</href></Icon></IconStyle></Style>" << endl;
    tkml << "  <Style id=\"triangle-orange\"><IconStyle><Icon><href>http://epic.awi.de/40953/21/triangle-orange.png</href></Icon></IconStyle></Style>" << endl;
    tkml << "  <Style id=\"triangle-red\"><IconStyle><Icon><href>http://epic.awi.de/40953/22/triangle-red.png</href></Icon></IconStyle></Style>" << endl;
    tkml << "  <Style id=\"triangle-white\"><IconStyle><Icon><href>http://epic.awi.de/40953/23/triangle-white.png</href></Icon></IconStyle></Style>" << endl;
    tkml << "  <Style id=\"triangle-yellow\"><IconStyle><Icon><href>http://epic.awi.de/40953/24/triangle-yellow.png</href></Icon></IconStyle></Style>" << endl;
//  tkml << "  <name>Document name</name>" << endl;
//  tkml << "  <description>Document description</description>" << endl;

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

int MainWindow::openKMLFolder( QFile& fkml, const QString &s_Campaign )
{
    QTextStream tkml( &fkml );
    tkml.setCodec("UTF-8");

    tkml << "  <Folder>\n";

    if ( ( s_Campaign != "xxx" ) && ( s_Campaign != "unknown campaign" ) )
    {
        tkml << "    <name>Campaign: " << s_Campaign.section( " (", 0, 0 ) << "</name>\n";
        tkml << "    <description><![CDATA[<a href=\"https://pangaea.de/search?q=" << s_Campaign.section( " (", 0, 0 ) << "\">all datasets related to campaign " << s_Campaign.section( " (", 0, 0 ) << "</a>]]></description>\n";
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

int MainWindow::writeKMLTrack( QFile& fkml, const QStringList &sl_MetadataList, const int i_TracklineWidth, const int i_TracklineColor, const int i_Start, const int i_End )
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

QString MainWindow::setIconSymbol( const int i_IconSymbol )
{
    QString s_IconSymbol    = "cricle";

    switch ( i_IconSymbol )
    {
        case _CIRCLE:
            s_IconSymbol = "circle";
            break;

        case _STAR:
            s_IconSymbol = "star";
            break;

        case _SQUARE:
            s_IconSymbol = "square";
            break;

        case _TRIANGLE:
            s_IconSymbol = "triangle";
            break;

        default:
            s_IconSymbol = "circle";
            break;
    }

    return( s_IconSymbol );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QString MainWindow::setIconColor( const int i_IconColor )
{
    QString s_IconColor     = "red";

    switch ( i_IconColor )
    {
        case _RED:
            s_IconColor = "red";
            break;

        case _GREEN:
            s_IconColor = "green";
            break;

        case _BLUE:
            s_IconColor = "blue";
            break;

        case _ORANGE:
            s_IconColor = "orange";
            break;

        case _YELLOW:
            s_IconColor = "yellow";
            break;

        case _WHITE:
            s_IconColor = "white";
            break;

        default:
            s_IconColor = "red";
            break;
    }

    return( s_IconColor );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

int MainWindow::writeKMLEntry( QFile& fkml, const QStringList &sl_MetadataList, const bool b_displayEventLabel, const bool b_displayDescription, const float f_IconSize, const int i_IconColor, const int i_IconSymbol, structURL URL[], const int i )
{
    double d_Latitude      = sl_MetadataList.at( i ).section( "\t", _LATITUDEPOS, _LATITUDEPOS ).toDouble();   // Latitude
    double d_Longitude     = sl_MetadataList.at( i ).section( "\t", _LONGITUDEPOS, _LONGITUDEPOS ).toDouble(); // Longitude
    double d_Elevation     = sl_MetadataList.at( i ).section( "\t", _ELEVATIONPOS, _ELEVATIONPOS ).toDouble(); // Elevation

    QString s_CampaignLabel = sl_MetadataList.at( i ).section( "\t", _CAMPAIGNLABELPOS, _CAMPAIGNLABELPOS );   // Campaign
    QString s_EventLabel    = sl_MetadataList.at( i ).section( "\t", _EVENTLABELPOS, _EVENTLABELPOS );         // Event label
    QString s_GearName      = sl_MetadataList.at( i ).section( "\t", _GEARNAMEPOS, _GEARNAMEPOS );             // Gear name
    QString s_Date          = sl_MetadataList.at( i ).section( "\t", _DATEPOS, _DATEPOS );                     // Date
    QString s_Time          = sl_MetadataList.at( i ).section( "\t", _TIMEPOS, _TIMEPOS );                     // Time
    QString s_AreaName      = sl_MetadataList.at( i ).section( "\t", _AREANAMEPOS, _AREANAMEPOS );             // Area
    QString s_DOI           = sl_MetadataList.at( i ).section( "\t", _DOIPOS, _DOIPOS );                       // DOI
    QString s_Citation      = sl_MetadataList.at( i ).section( "\t", _CITATIONPOS, _CITATIONPOS );             // Citation
    QString s_URL           = "";
    QString s_TitleURL      = "";

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
                    tkml << "https://doi.pangaea.de/" << s_DOI.section( "doi:", 1, 1 ).section( ",", 0, 0 );  // published or DOI registration in progress
                else
                    tkml << "https://pangaea.de/search?q=dataset" << s_DOI.section( ": ", 1, 1 ).section( ",", 0, 0 ); // unpublished datset

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
        }

        for ( int j=1; j<=_MAX_NUM_OF_URLS; ++j )
        {

            if ( URL[j].position >= 0 )
            {
                if ( sl_MetadataList.at( i ).section( "\t", _CITATIONPOS+j, _CITATIONPOS+j ).isEmpty() == false )
                {
                    s_TitleURL = URL[j].TitleURL;
                    s_URL      = sl_MetadataList.at( i ).section( "\t", _CITATIONPOS+j, _CITATIONPOS+j );

                    s_URL.replace( "doi:", "https://doi.pangaea.de/" );
                    s_URL.replace( "hdl:", "https://doi.pangaea.de/" );

                    if ( ( s_TitleURL.startsWith( "url", Qt::CaseInsensitive ) ) || ( s_TitleURL.startsWith( "uri", Qt::CaseInsensitive ) ) )
                        s_TitleURL = s_TitleURL.mid( 4 );

                    if ( s_TitleURL.endsWith( "of core", Qt::CaseInsensitive ) == true )
                        s_TitleURL.append( " " +  s_EventLabel.section( " (line ", 0, 0 ) );

                    tkml << "<br /><![CDATA[<a href=\"" << s_URL << "\">" << s_TitleURL << "</a>]]>";
                }
            }
        }

        if ( ( s_Citation.isEmpty() == false ) && ( s_Citation == "xxx" ) )
        {
            tkml << "<br /><![CDATA[<a href=\"";
            tkml << "https://pangaea.de/search?q=" << s_EventLabel.section( " (line ", 0, 0 ); // Event
//          tkml << "\">all data related to</a>]]>";
            tkml << "\">all data related to core " << s_EventLabel.section( " (line ", 0, 0 ) << "</a>]]>";
        }

        tkml << "</description>";
    }

    tkml << "<View>";
    tkml << "<longitude>" << s_Longitude << "</longitude>";
    tkml << "<latitude>" << s_Latitude << "</latitude>";
    tkml << "</View>";


    tkml << "<styleUrl>#" << setIconSymbol( i_IconSymbol ) << "-" << setIconColor( i_IconColor ) << "</styleUrl>";
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
    int         err               = _CHOOSEABORTED_;

    int         i_File            = 0;
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
        if ( setFilename( _FORMAT_KMLFILE, gb_CuratorMode, gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), gs_FilenameGoogleEarth ) == QDialog::Accepted )
        {
            initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Creating Google Earth import file..." ) );

            err = _NOERROR_;

            while ( ( i_File < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
            {
                setStatusBarFileInProgress( gsl_FilenameList.at( i_File ) );

                err = analyzeDataset( gsl_FilenameList.at( i_File ), gi_CodecInput, false, false, false, sl_MetadataList, sl_ParameterList, i_EnvArray );

                stopProgress = incFileProgress( gsl_FilenameList.count(), i_File++ );
            }

            resetFileProgress( gsl_FilenameList.count() );
        }
    }

// **********************************************************************************************

    if ( ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
        err = createGoogleEarthImportFile( sl_MetadataList, gs_FilenameGoogleEarth, gi_CodecInput, gb_displayEventLabel, gb_displayDescription, gi_IconSize, gi_IconColor, gi_IconSymbol, gi_TracklineWidth, gi_TracklineColor, sl_MetadataList.count()-1 );

// **********************************************************************************************

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Google Earth converter was canceled" ), false, false );

// **********************************************************************************************
// start Google Earth

    if ( ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) && ( gb_startGoogleEarth == true ) )
        err = startGoogleEarth( gs_FilenameGoogleEarthProgram, gs_FilenameGoogleEarth );

// **********************************************************************************************

    onError( err );
}
