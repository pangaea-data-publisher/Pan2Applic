// ***********************************************************************************************
// *                                                                                             *
// * getMetadata.cpp                                                                             *
// *                                                                                             *
// * Dr. Rainer Sieger - 2009-04-07                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

int MainWindow::getEventMetadata( const QString &s_EventStr,
                                  QString &s_DateTime, QString &s_Elevation,
                                  QString &s_EventLabel, QString &s_CruiseLabel,
                                  QString &s_AreaName, QString &s_Gear,
                                  QString &s_Latitude, QString &s_Longitude,
                                  QStringList &sl_ParameterList )
{
    int     n           = 0;

    int		i_MetaInfo  = 0;

    QString InputStr    = s_EventStr;

// **********************************************************************************************

    InputStr.replace( " * ",               "\t" );

    InputStr.replace( "Event(s):\t",       "1\t" );

    InputStr.replace( "DATE/TIME: ",       "2\t" );
    InputStr.replace( "LATITUDE: ",        "3\t" );
    InputStr.replace( "LONGITUDE: ",       "4\t" );
    InputStr.replace( "ELEVATION: ",       "5\t" );

    InputStr.replace( "DATE/TIME START: ", "6\t" );
    InputStr.replace( "LATITUDE START: ",  "7\t" );
    InputStr.replace( "LONGITUDE START: ", "8\t" );
    InputStr.replace( "ELEVATION START: ", "9\t" );

    InputStr.replace( "DATE/TIME END: ",   "10\t" );
    InputStr.replace( "LATITUDE END: ",    "11\t" );
    InputStr.replace( "LONGITUDE END: ",   "12\t" );
    InputStr.replace( "ELEVATION END: ",   "13\t" );

    InputStr.replace( "CAMPAIGN: ",        "14\t" );
    InputStr.replace( "DEVICE: ",          "15\t" );
    InputStr.replace( "LOCATION: ",        "16\t" );
    InputStr.replace( "BASIS: ",           "17\t" );
    InputStr.replace( "RECOVERY: ",        "18\t" );
    InputStr.replace( "COMMENT: ",         "16\t" );

    n = NumOfSections( InputStr );

    for ( int i=0; i<n; i+=2 )
    {
        i_MetaInfo = InputStr.section( "\t", i, i ).toInt();

        switch( i_MetaInfo )
        {
            case 1:
                s_EventLabel = InputStr.section( "\t", i+1, i+1 ).simplified();
                s_EventLabel = s_EventLabel.section( " ", 0, 0 );
                sl_ParameterList.append( "3@Event * eMetadata" );
                break;
            case 2:
                s_DateTime = InputStr.section( "\t", i+1, i+1 );
                sl_ParameterList.append( "5@Date/Time * eMetadata" );
                break;
            case 3:
                s_Latitude = InputStr.section( "\t", i+1, i+1 );
                sl_ParameterList.append( "6@Latitude * eMetadata" );
                break;
            case 4:
                s_Longitude	= InputStr.section( "\t", i+1, i+1 );
                sl_ParameterList.append( "7@Longitude * eMetadata" );
                break;
            case 5:
                s_Elevation	= InputStr.section( "\t", i+1, i+1 );
                s_Elevation.replace( " m", "" );
                sl_ParameterList.append( "8@Elevation * eMetadata" );
                break;
            case 14:
                s_CruiseLabel = InputStr.section( "\t", i+1, i+1 );
                sl_ParameterList.append( "2@Campaign * eMetadata" );
                break;
            case 15:
                s_Gear = InputStr.section( "\t", i+1, i+1 );
                sl_ParameterList.append( "4@Device * eMetadata" );
                break;
            case 16:
                s_AreaName = InputStr.section( "\t", i+1, i+1 );
                sl_ParameterList.append( "1@Area * eMetadata" );
                break;
            default:
                break;
        }
    }

    sl_ParameterList.sort();

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

int MainWindow::getDataMetadata( const int i_CodecInput, const int i_OutputFormat, const QString &s_MetadataIn, QStringList &sl_DataMetadataList, structURL URL[], const int i_NumOfFiles )
{
    int             n                   = 0;

    int             j                   = 0;
    int             i_line              = 0;
    int             stopProgress      = 0;

    int             i_DatePos           = -1;
    int             i_TimePos           = -1;
    int             i_DateTimePos       = -1;
    int             i_ElevationPos      = -1;
    int             i_EventLabelPos     = -1;
    int             i_CampaignLabelPos  = -1;
    int             i_AreaNamePos       = -1;
    int             i_GearNamePos       = -1;
    int             i_LatitudePos       = -1;
    int             i_LongitudePos      = -1;

    QStringList     sl_Input;

    QString         s_Metadata          = "";
    QString         s_EventLabel        = "";
    QString         s_Header            = "";
    QString         s_LatLon            = "";

    bool            b_containsPosition  = false;

    sl_DataMetadataList.clear();

//-----------------------------------------------------------------------------------------------------------------------

    b_containsPosition = getDataMetadataPosition( s_MetadataIn, i_DatePos, i_TimePos, i_DateTimePos, i_ElevationPos, i_EventLabelPos, i_CampaignLabelPos, i_AreaNamePos, i_GearNamePos, i_LatitudePos, i_LongitudePos );

//-----------------------------------------------------------------------------------------------------------------------

    if ( ( n = readFile( s_MetadataIn.section( "\t", _FILENAMEPOS, _FILENAMEPOS ), sl_Input, i_CodecInput ) ) < 1 )
        return( n );

//-----------------------------------------------------------------------------------------------------------------------

    switch ( i_OutputFormat )
    {
      case _FORMAT_KMLFILE:
        initProgress( i_NumOfFiles, s_MetadataIn.section( "\t", _FILENAMEPOS, _FILENAMEPOS ), tr( "Creating Google Earth import file" ), n );
        break;

      case _FORMAT_SHAPE_METADATA:
      case _FORMAT_SHAPE_DATA:
        initProgress( i_NumOfFiles, s_MetadataIn.section( "\t", _FILENAMEPOS, _FILENAMEPOS ), tr( "Creating shapefile" ), n );
        break;

      default:
        initProgress( i_NumOfFiles, s_MetadataIn.section( "\t", _FILENAMEPOS, _FILENAMEPOS ), tr( "Creating file" ), n );
        break;
    }

//-----------------------------------------------------------------------------------------------------------------------

    i_line = s_MetadataIn.section( "\t", _NUMOFHEADERLINESPOS, _NUMOFHEADERLINESPOS ).toInt()-1;

    if ( sl_Input.at( i_line ) == "*/" )
        ++i_line;

    for ( int i=0; i<NumOfSections( sl_Input.at( i_line ) ); ++i )
    {
        s_Header = sl_Input.at( i_line ).section( "\t", i, i );

        if ( ( s_Header.startsWith( "url", Qt::CaseInsensitive ) == true ) || ( s_Header.startsWith( "uri", Qt::CaseInsensitive ) == true ) || ( s_Header.startsWith( "persistent identifier", Qt::CaseInsensitive ) == true ) )
        {
            if ( ++j <= _MAX_NUM_OF_URLS )
            {
                URL[j].position = i;

                if ( s_Header.contains( "@" ) == true )
                    URL[j].TitleURL = s_Header.section( "@", 1, 1 );
                else
                    URL[j].TitleURL = s_Header;
            }
        }
    }

    stopProgress = incProgress( i_NumOfFiles, i_line );

//-----------------------------------------------------------------------------------------------------------------------

    ++i_line;
    parseData( i_line, sl_Input.at( i_line ), s_MetadataIn, i_AreaNamePos, i_CampaignLabelPos, i_EventLabelPos, i_GearNamePos, i_DatePos, i_TimePos, i_DateTimePos, i_LatitudePos, i_LongitudePos, i_ElevationPos, URL, s_Metadata );
    addToMetadataList( s_Metadata, s_EventLabel, s_LatLon, sl_DataMetadataList );

    stopProgress = incProgress( i_NumOfFiles, i_line );

    if ( b_containsPosition == true )
    {
        while ( ( ++i_line < n ) && ( stopProgress != _APPBREAK_ ) )
        {
            parseData( i_line, sl_Input.at( i_line ), s_MetadataIn, i_AreaNamePos, i_CampaignLabelPos, i_EventLabelPos, i_GearNamePos, i_DatePos, i_TimePos, i_DateTimePos, i_LatitudePos, i_LongitudePos, i_ElevationPos, URL, s_Metadata );
            addToMetadataList( s_Metadata, s_EventLabel, s_LatLon, sl_DataMetadataList );

            stopProgress = incProgress( i_NumOfFiles, i_line );
        }
    }

//-----------------------------------------------------------------------------------------------------------------------

    resetProgress( i_NumOfFiles );

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

bool MainWindow::parseData( const int i_line, const QString &InputStr, const QString &s_MetadataIn, const int i_AreaNamePos, const int i_CampaignLabelPos, const int i_EventLabelPos, const int i_GearNamePos, const int i_DatePos, const int i_TimePos, const int i_DateTimePos, const int i_LatitudePos, const int i_LongitudePos, const int i_ElevationPos, structURL URL[], QString &s_MetadataOut )
{
    s_MetadataOut = s_MetadataIn.section( "\t", _FILENAMEPOS, _METAINFOPOS ) + QString( "\t%1\t" ).arg( i_line ) + s_MetadataIn.section( "\t", _ENVIRONMENTPOS, _ENVIRONMENTPOS ) + "\t";

// **********************************************************************************************
// 4 - Campaign

    if ( i_CampaignLabelPos > -1 ) // 4
        s_MetadataOut.append( InputStr.section( "\t", i_CampaignLabelPos, i_CampaignLabelPos ) );
    else
        s_MetadataOut.append( s_MetadataIn.section( "\t", _CAMPAIGNLABELPOS, _CAMPAIGNLABELPOS ) );

    s_MetadataOut.append( "\t" );

// **********************************************************************************************
// 5 - Event label

    if ( i_EventLabelPos > -1 ) // 5
        s_MetadataOut.append( InputStr.section( "\t", i_EventLabelPos, i_EventLabelPos ) );
    else
        s_MetadataOut.append( s_MetadataIn.section( "\t", _EVENTLABELPOS, _EVENTLABELPOS ) );

    s_MetadataOut.append( "\t" );

// **********************************************************************************************
// 6 - Gear name

    if ( i_GearNamePos > -1 ) // 6
        s_MetadataOut.append( InputStr.section( "\t", i_GearNamePos, i_GearNamePos ) );
    else
        s_MetadataOut.append( s_MetadataIn.section( "\t", _GEARNAMEPOS, _GEARNAMEPOS ) );

    s_MetadataOut.append( "\t" );

// **********************************************************************************************
// 7 - Date, 8 - Time, 9 - Date/Time

    if ( i_DateTimePos > -1 ) // 9
    {
        s_MetadataOut.append( InputStr.section( "\t", i_DateTimePos, i_DateTimePos ).section( "T", 0, 0 ) ); // 7
        s_MetadataOut.append( "\t" );
        s_MetadataOut.append( InputStr.section( "\t", i_DateTimePos, i_DateTimePos ).section( "T", 1, 1 ) ); // 8
        s_MetadataOut.append( "\t" );
        s_MetadataOut.append( InputStr.section( "\t", i_DateTimePos, i_DateTimePos ) );                      // 9
    }
    else
    {
        // Date
        if ( i_DatePos > -1 ) // 7
            s_MetadataOut.append( InputStr.section( "\t", i_DatePos, i_DatePos ) );
        else
            s_MetadataOut.append( s_MetadataIn.section( "\t", _DATEPOS, _DATEPOS ) );

        s_MetadataOut.append( "\t" );

        // Time
        if ( i_TimePos > -1 ) // 8
            s_MetadataOut.append( InputStr.section( "\t", i_TimePos, i_TimePos ) );
        else
            s_MetadataOut.append( s_MetadataIn.section( "\t", _TIMEPOS, _TIMEPOS ) );

        s_MetadataOut.append( "\t" );

        // Date/Time
        if ( i_DatePos > -1 ) // 7
            s_MetadataOut.append( InputStr.section( "\t", i_DatePos, i_DatePos ) );
        else
            s_MetadataOut.append( s_MetadataIn.section( "\t", _DATEPOS, _DATEPOS ) );

        s_MetadataOut.append( "T" );

        if ( i_TimePos > -1 ) // 8
            s_MetadataOut.append( InputStr.section( "\t", i_TimePos, i_TimePos ) );
        else
            s_MetadataOut.append( s_MetadataIn.section( "\t", _TIMEPOS, _TIMEPOS ) );

    }

// **********************************************************************************************
// 10 - Date/Time start

    s_MetadataOut.append( "\t" );
    s_MetadataOut.append( s_MetadataIn.section( "\t", _DATETIMESTARTPOS, _DATETIMESTARTPOS ) ); // 10
    s_MetadataOut.append( "\t" );

// **********************************************************************************************
// 11 - Latitude

    if ( i_LatitudePos > -1 ) // 11
        s_MetadataOut.append( InputStr.section( "\t", i_LatitudePos, i_LatitudePos ) );
    else
        s_MetadataOut.append( s_MetadataIn.section( "\t", _LATITUDEPOS, _LATITUDEPOS ) );

    s_MetadataOut.append( "\t" );

// **********************************************************************************************
// 12 - Longitude

    if ( i_LongitudePos > -1 ) // 12
        s_MetadataOut.append( InputStr.section( "\t", i_LongitudePos, i_LongitudePos ) );
    else
        s_MetadataOut.append( s_MetadataIn.section( "\t", _LONGITUDEPOS, _LONGITUDEPOS ) );

    s_MetadataOut.append( "\t" );

// **********************************************************************************************
// 13 - Elevation

    if ( i_ElevationPos > -1 ) // 13
        s_MetadataOut.append( InputStr.section( "\t", i_ElevationPos, i_ElevationPos ) );
    else
        s_MetadataOut.append( s_MetadataIn.section( "\t", _ELEVATIONPOS, _ELEVATIONPOS ) );

    s_MetadataOut.append( "\t" );

// **********************************************************************************************
// 14 - Area name

    if ( i_AreaNamePos > -1 ) // 14
        s_MetadataOut.append( InputStr.section( "\t", i_AreaNamePos, i_AreaNamePos ) );
    else
        s_MetadataOut.append( s_MetadataIn.section( "\t", _AREANAMEPOS, _AREANAMEPOS ) );

    s_MetadataOut.append( "\t" );

// **********************************************************************************************
// 15 - DOI

    s_MetadataOut.append( s_MetadataIn.section( "\t", _DOIPOS, _DOIPOS ) );
    s_MetadataOut.append( "\t" );

// **********************************************************************************************
// 16 - Citation

    s_MetadataOut.append( s_MetadataIn.section( "\t", _CITATIONPOS, _CITATIONPOS ) );
    s_MetadataOut.append( "\t" );

// **********************************************************************************************
// URLs

    for ( int j=1; j<=_MAX_NUM_OF_URLS; ++j )
    {
        if ( URL[j].position > -1 )
            s_MetadataOut.append( InputStr.section( "\t", URL[j].position, URL[j].position ) );

        s_MetadataOut.append( "\t" );
    }

    return( true );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

bool MainWindow::addToMetadataList( const QString &s_Metadata, QString &s_EventLabel, QString &s_LatLon, QStringList &sl_MetadataList )
{
    QString s_LatLongNew    = s_Metadata.section( "\t", _LATITUDEPOS, _LATITUDEPOS ) + "," + s_Metadata.section( "\t", _LONGITUDEPOS, _LONGITUDEPOS );
    QString s_EventLabelNew = s_Metadata.section( "\t", _EVENTLABELPOS, _EVENTLABELPOS );

    if ( ( s_EventLabel != s_EventLabelNew ) || ( s_LatLon != s_LatLongNew ) )
    {
        sl_MetadataList.append( s_Metadata );

        s_EventLabel = s_EventLabelNew;
        s_LatLon     = s_LatLongNew;

        return( true );
    }

    return( false );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

int MainWindow::getDataMetadataPosition( const bool b_containsPANGAEAHeader, const QString &InputStr, int& i_DatePos, int& i_TimePos, int& i_DateTimePos, int& i_ElevationPos, int& i_EventLabelPos, int& i_CampaignLabelPos, int& i_AreaNamePos, int& i_GearNamePos, int& i_LatitudePos, int& i_LongitudePos, QStringList &sl_ParameterList )
{
    int     n                   = 0;

    QString s_Parameter         = "";

// **********************************************************************************************

    n = NumOfSections( InputStr );

    for ( int i=0; i<n; ++i )
    {
        s_Parameter = InputStr.section( "\t", i, i );

        s_Parameter.replace( "\"", "" );

//----------------------------------------------------------------------------------------------
// Event label

        if ( ( s_Parameter.simplified().toLower() == "sample code/label" ) || ( s_Parameter.simplified().toLower() == "core/sample" ) )
            i_EventLabelPos = i;

        if ( ( s_Parameter.simplified().toLower() == "event" ) || ( s_Parameter.simplified().toLower() == "event label" ) )
            i_EventLabelPos = i;

        if ( ( s_Parameter.simplified().toLower() == "labelevent" ) || ( s_Parameter.simplified().toLower() == "station" ) )
            i_EventLabelPos = i;

//----------------------------------------------------------------------------------------------
// Latitude

        if ( ( s_Parameter.simplified().toLower() == "lat (°n)" ) || ( s_Parameter.simplified().toLower() == "latitude [degrees_north]" ) ) // ODV Spreadsheet
            i_LatitudePos = i;

        if ( ( s_Parameter.simplified().toLower() == "latitude" ) || ( s_Parameter.simplified().toLower() == "latitudeevent" ) || ( s_Parameter == "1600" ) )
            i_LatitudePos = i;

//----------------------------------------------------------------------------------------------
// Longitude

        if ( ( s_Parameter.simplified().toLower() == "lon (°e)" ) || ( s_Parameter.simplified().toLower() == "longitude [degrees_east]" ) ) // ODV Spreadsheet
            i_LongitudePos = i;

        if ( ( s_Parameter.simplified().toLower() == "longitude" ) || ( s_Parameter.simplified().toLower() == "longitudeevent" ) || ( s_Parameter == "1601" ) )
            i_LongitudePos = i;

//----------------------------------------------------------------------------------------------
// Elevation

        if ( ( s_Parameter.simplified().toLower() == "elevation of event" ) || ( s_Parameter.simplified().toLower() == "elevationevent" ) )
            i_ElevationPos = i;

        if ( ( s_Parameter.simplified().toLower() == "elevation" ) || ( s_Parameter.simplified().toLower() == "elevation [m]" ) )
            i_ElevationPos = i;

        if ( s_Parameter.simplified().toLower() == "depth" )  // PanCore
            i_ElevationPos = i;

//----------------------------------------------------------------------------------------------
// Campaign

        if ( ( s_Parameter.simplified().toLower() == "cruise" ) || ( s_Parameter.simplified().toLower() == "campaign" ) || ( s_Parameter.simplified().toLower() == "campaign label" ) || ( s_Parameter.simplified().toLower() == "expedition" ) )
            i_CampaignLabelPos = i;

//----------------------------------------------------------------------------------------------
// Area

        if ( s_Parameter.simplified().toLower() == "area" )
            i_AreaNamePos = i;

//----------------------------------------------------------------------------------------------
// Gear type

        if ( ( s_Parameter.simplified().toLower() == "type" ) || ( s_Parameter.simplified().toLower() == "gear" )  || ( s_Parameter.simplified().toLower() == "device" ) )
            i_GearNamePos = i;

//----------------------------------------------------------------------------------------------
// Date

        if ( ( s_Parameter.simplified().toLower() ==  "date" ) || ( s_Parameter.simplified().toLower() ==  "dateevent" ) || ( s_Parameter.simplified().toLower() ==  "date event" ) )
            i_DatePos = i;

//----------------------------------------------------------------------------------------------
// Time

        if ( ( s_Parameter.simplified().toLower() == "time" ) || ( s_Parameter.simplified().toLower() == "timeevent" ) || ( s_Parameter.simplified().toLower() == "time event" ) )
            i_TimePos = i;

//---------------------------------------------------------------------------------------------
// Date/Time

        if ( ( s_Parameter.simplified().toLower() == "date/time" ) || ( s_Parameter.simplified().toLower() == "date/time of event" ) )
            i_DateTimePos = i;

//---------------------------------------------------------------------------------------------

        if ( b_containsPANGAEAHeader == false )
//          sl_ParameterList.append( s_Parameter.section( "@", 0, 0 ) );
            sl_ParameterList.append( s_Parameter );
    }

    return( sl_ParameterList.count() );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

bool MainWindow::getDataMetadataPosition( const QString &s_MetadataIn, int& i_DatePos, int& i_TimePos, int& i_DateTimePos, int& i_ElevationPos, int& i_EventLabelPos, int& i_CampaignLabelPos, int& i_AreaNamePos, int& i_GearNamePos, int& i_LatitudePos, int& i_LongitudePos )
{
    bool b_containsPosition = false;

    i_DatePos          = s_MetadataIn.section( "\t", _DATADATEPOS, _DATADATEPOS ).toInt();
    i_TimePos          = s_MetadataIn.section( "\t", _DATATIMEPOS, _DATATIMEPOS ).toInt();
    i_DateTimePos      = s_MetadataIn.section( "\t", _DATADATETIMEPOS, _DATADATETIMEPOS ).toInt();
    i_ElevationPos     = s_MetadataIn.section( "\t", _DATAELEVATIONPOST, _DATAELEVATIONPOST ).toInt();
    i_EventLabelPos    = s_MetadataIn.section( "\t", _DATAEVENTLABELPOS, _DATAEVENTLABELPOS ).toInt();
    i_CampaignLabelPos = s_MetadataIn.section( "\t", _DATACAMPAIGNLABELPOS, _DATACAMPAIGNLABELPOS ).toInt();
    i_AreaNamePos      = s_MetadataIn.section( "\t", _DATAAREANAMEPOS, _DATAAREANAMEPOS ).toInt();
    i_GearNamePos      = s_MetadataIn.section( "\t", _DATAGEARNAMEPOS, _DATAGEARNAMEPOS ).toInt();
    i_LatitudePos      = s_MetadataIn.section( "\t", _DATALATITUDEPOS, _DATALATITUDEPOS ).toInt();
    i_LongitudePos     = s_MetadataIn.section( "\t", _DATALONGITUDEPOS, _DATALONGITUDEPOS ).toInt();

    if ( ( i_LatitudePos > -1 ) && ( i_LongitudePos > -1 ) )
        b_containsPosition = true;

    return( b_containsPosition );
}
