// ***********************************************************************************************
// *                                                                                             *
// * setEnvironment.cpp                                                                          *
// *                                                                                             *
// * Dr. Rainer Sieger - 2009-03-24                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2015-05-28

bool MainWindow::isGeocode( const QString s_Parameter )
{   // | = _PSEPARATOR_

/*
    +	1           DEPTH, sediment/rock        Depth           m
    +	1599        DATE/TIME                   Date/Time
    +	1600        LATITUDE                    Latitude
    +	1601        LONGITUDE                   Longitude
    +	1619        DEPTH, water                Depth water     m
    +	2205        AGE                         Age             kyr BP
    +	2920        DISTANCE                    Distance        cm
    +	4607        ALTITUDE                    Altitude        m
    +	56349       HEIGHT above ground         Height          m
    +	5059        DEPTH, ice/snow             Depth ice/snow  m
    +	6262        ORDINAL NUMBER              No
    +	8128        ELEVATION                   Elevation       m a.s.l.
    +	25539       SECTION, height             Section         m
    +	26097       POINT DISTANCE from start   Dist            m
*/

    if ( s_Parameter.startsWith( "DEPTH, sediment/rock [m]|Depth [m]|" ) == true )
        return( true );

    if ( s_Parameter.startsWith( "DATE/TIME|Date/Time|" ) == true )
        return( true );

    if ( s_Parameter.startsWith( "LATITUDE|Latitude|" ) == true )
        return( true );

    if ( s_Parameter.startsWith( "LONGITUDE|Longitude|" ) == true )
        return( true );

    if ( s_Parameter.startsWith( "DEPTH, water [m]|Depth water [m]|" ) == true )
        return( true );

    if ( s_Parameter.startsWith( "AGE [kyr BP]| [Age kyr BP]|" ) == true )
        return( true );

    if ( s_Parameter.startsWith( "DISTANCE [cm]|Distance [cm]|" ) == true )
        return( true );

    if ( s_Parameter.startsWith( "ALTITUDE [m]|Altitude [m]|" ) == true )
        return( true );

    if ( s_Parameter.startsWith( "HEIGHT above ground [m]|Height [m]|" ) == true )
        return( true );

    if ( s_Parameter.startsWith( "DEPTH, ice/snow [m]|Depth ice/snow [m]|" ) == true )
        return( true );

    if ( s_Parameter.startsWith( "HEIGHT above ground [m]|Height [m]|" ) == true )
        return( true );

    if ( s_Parameter.startsWith( "ORDINAL NUMBER []|No []|" ) == true )
        return( true );

    if ( s_Parameter.startsWith( "ELEVATION [m a.s.l.]|Elevation [m a.s.l.]|" ) == true )
        return( true );

    if ( s_Parameter.startsWith( "SECTION, height [m]|Section [m]|" ) == true )
        return( true );

    if ( s_Parameter.startsWith( "POINT DISTANCE from start [m]|Dist [m]|" ) == true )
        return( true );

    if ( s_Parameter.startsWith( "Date/Time of event|Date/Time|" ) == true )
        return( true );

    if ( s_Parameter.startsWith( "Latitude of event|Latitude|" ) == true )
        return( true );

    if ( s_Parameter.startsWith( "Longitude of event|Longitude|" ) == true )
        return( true );

    if ( s_Parameter.contains( "GEOCODE" ) == true )
        return( true );

    return( false );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

int MainWindow::getEnvironment( const int i_EnvArray[] )
{
    int i_Environment = 0;

    for ( int i=1; i<=_MAX_NUM_OF_ENV; ++i )
    {
        if ( i_EnvArray[i] > 0 )
            i_Environment += i_EnvArray[i];
    }

    return( i_Environment );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

int MainWindow::getEnvironment( const QString &s_ParameterIn, int i_EnvArray[] )
{
    QString s_Parameter = s_ParameterIn;

/* PANGAEA GEOCODE IDs
    ID          Parameter                   Abbreviation    Unit
+	1           DEPTH, rock                 Depth           m
+	1           DEPTH, sediment             Depth           m
+	1           DEPTH, sediment/rock        Depth           m
+	1599        DATE/TIME                   Date/Time
+	1600        LATITUDE                    Latitude
+	1601        LONGITUDE                   Longitude
+	1619        DEPTH, water                Depth water     m
+	2205        AGE                         Age             kyr BP
+	2920        DISTANCE                    Distance        cm
+	4607        ALTITUDE                    Altitude        m
+	56349       HEIGHT above ground         Height          m
+	5059        DEPTH, ice/snow             Depth ice/snow  m
+	6262        ORDINAL NUMBER              No
+	8128        ELEVATION                   Elevation       m a.s.l.
+	25539       SECTION, height             Section         m
+	26097       POINT DISTANCE from start   Dist            m
*/
//----------------------------------------------------------------------------------------------

    s_Parameter.replace( "\"", "" );

    if ( s_Parameter.isEmpty() == true )
        return( -1 );

    if ( s_Parameter.toLower() == "qf" ) // ODV Spreadsheet
        return( 0 );

//----------------------------------------------------------------------------------------------
// Event import

    if ( s_Parameter.toLower() == "labelevent" )
    {
        i_EnvArray[14] = _EVENTIMPORT;
        return( _EVENTIMPORT );
    }

//----------------------------------------------------------------------------------------------
// Water

    if ( ( s_Parameter.toLower() == "depth water [m]" ) || ( s_Parameter.toLower() == "depth, water [m]" ) || ( s_Parameter == "1619" ) )
    {
        i_EnvArray[2] = _WATER;
        return( _WATER );
    }

//----------------------------------------------------------------------------------------------
// Sediment

    if ( ( s_Parameter.toLower() == "depth rock [m]" ) || ( s_Parameter.toLower() == "depth, rock [m]" ) )
    {
        i_EnvArray[3] = _SEDIMENT;
        return( _SEDIMENT );
    }

    if ( ( s_Parameter.toLower() == "depth sediment [m]" ) || ( s_Parameter.toLower() == "depth, sediment [m]" ) )
    {
        i_EnvArray[3] = _SEDIMENT;
        return( _SEDIMENT );
    }

    if ( ( s_Parameter.toLower() == "depth sediment/rock [m]" ) || ( s_Parameter.toLower() == "depth, sediment/rock [m]" ) )
    {
        i_EnvArray[3] = _SEDIMENT;
        return( _SEDIMENT );
    }

    if ( ( s_Parameter.toLower() == "depth sediment [cm]" ) || ( s_Parameter.toLower() == "depth, sediment [cm]" ) )
    {
        i_EnvArray[3] = _SEDIMENT;
        return( _SEDIMENT );
    }

    if ( ( s_Parameter.toLower() == "depth [m]" ) || ( s_Parameter.toLower() == "depth [cm]" ) )
    {
        i_EnvArray[3] = _SEDIMENT;
        return( _SEDIMENT );
    }

    if ( ( s_Parameter == "1" ) || ( s_Parameter == "cm" ) || ( s_Parameter == "m" ) )
    {
        i_EnvArray[3] = _SEDIMENT;
        return( _SEDIMENT );
    }

//----------------------------------------------------------------------------------------------
// Ice/snow

    if ( ( s_Parameter.toLower() == "depth, ice/snow [m]" ) || ( s_Parameter.toLower() == "depth ice/snow [m]" ) )
    {
        i_EnvArray[4] = _ICE;
        return( _ICE );
    }

    if ( ( s_Parameter.toLower() == "depth ice [m]" ) || ( s_Parameter.toLower() == "depth, ice [m]" ) || ( s_Parameter == "5059" ) )
    {
        i_EnvArray[4] = _ICE;
        return( _ICE );
    }

//----------------------------------------------------------------------------------------------
// Distance

    if ( ( s_Parameter.toLower() == "distance [cm]" ) || ( s_Parameter == "2920" ) )
    {
        i_EnvArray[5] = _DISTANCE;
        return( _DISTANCE );
    }

//----------------------------------------------------------------------------------------------
// Point distance

    if ( ( s_Parameter.toLower() == "dist [m]" ) || ( s_Parameter.toLower() == "point distance from start [m]" ) || ( s_Parameter == "26097" ) )
    {
        i_EnvArray[6] = _POINTDISTANCE;
        return( _POINTDISTANCE );
    }

//----------------------------------------------------------------------------------------------
// Air, Altitude

    if ( ( s_Parameter.toLower() == "altitude [m]" ) || ( s_Parameter == "4607" ) )
    {
        i_EnvArray[7] = _AIRA;
        return( _AIRA );
    }

//----------------------------------------------------------------------------------------------
// Air, Height

    if ( ( s_Parameter.toLower() == "height [m]" ) || ( s_Parameter.toLower() == "height above ground [m]" ) || ( s_Parameter == "56349" ) )
    {
        i_EnvArray[8] = _AIRH;
        return( _AIRH );
    }

//----------------------------------------------------------------------------------------------
// Date/Time

    if ( ( s_Parameter.toLower() ==  "date/time" ) || ( s_Parameter.toLower() ==  "datetime" ) || ( s_Parameter.toLower() ==  "date/time of event" ) || ( s_Parameter.toLower() ==  "date/time (hourly average)" ) || ( s_Parameter ==  "1599" ) )
    {
        i_EnvArray[9] = _DATETIME;
        return( _DATETIME );
    }

//----------------------------------------------------------------------------------------------
// Age

    if ( ( s_Parameter.toLower() == "age [kyr bp]" ) || ( s_Parameter.toLower() == "age [kyr]" ) || ( s_Parameter == "2205" ) )
    {
        i_EnvArray[10] = _AGE;
        return( _AGE );
    }

    if ( ( s_Parameter.toLower() == "age [ka bp]" ) || ( s_Parameter.toLower() == "age [ka]" ) )
    {
        i_EnvArray[10] = _AGE;
        return( _AGE );
    }

//----------------------------------------------------------------------------------------------
// Ordinal number

    if ( ( s_Parameter.toLower() == "ordinal number" ) || ( s_Parameter.toLower() == "no" ) || ( s_Parameter.toLower() == "6262" ) )
    {
        i_EnvArray[11] = _ORDINALNUMBER;
        return( _ORDINALNUMBER );
    }

//----------------------------------------------------------------------------------------------
// Elevation

    if ( ( s_Parameter.toLower() ==  "elevation [m a.s.l.]" ) || ( s_Parameter.toLower() ==  "elevation [m]" ) || ( s_Parameter ==  "8128" ) )
    {
        i_EnvArray[12] = _ELEVATION;
        return( _ELEVATION );
    }

//----------------------------------------------------------------------------------------------
// Section

    if ( ( s_Parameter.toLower() ==  "section, height [m]" ) || ( s_Parameter.toLower() ==  "section [m]" ) || ( s_Parameter ==  "25539" ) )
    {
        i_EnvArray[13] = _SECTION;
        return( _SECTION );
    }

//----------------------------------------------------------------------------------------------
// unknown

    return( 0 ); // is parameter or metadata
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

int MainWindow::getNumOfEnvironments( const int i_EnvArray[] )
{
    int i_NumOfEnvironments = 0;

    for ( int i=1; i<=_MAX_NUM_OF_ENV; ++i )
    {
        if ( i_EnvArray[i] > 0 )
            i_NumOfEnvironments++;
    }

    return( i_NumOfEnvironments );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

int MainWindow::getNumOfEnvironments( const QStringList &sl_MetadataList )
{
    int i_NumOfEnvironments = 0;

    for ( int i=1; i<sl_MetadataList.count(); i++ )
    {
        if ( sl_MetadataList.at( i-1 ).section( "\t", 3, 3 ) != sl_MetadataList.at( i ).section( "\t", 3, 3 ) )
            i_NumOfEnvironments++;
    }

    return( i_NumOfEnvironments );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2009-10-11

int MainWindow::getParameterType( const QString &s_Parameter )
{
//----------------------------------------------------------------------------------------------
// Event

        if ( s_Parameter.startsWith( "event", Qt::CaseInsensitive ) == true )
            return( _ISMETADATA );

        if ( s_Parameter.startsWith( "station", Qt::CaseInsensitive ) == true )
            return( _ISMETADATA );

//----------------------------------------------------------------------------------------------
// Area

        if ( s_Parameter.startsWith( "area", Qt::CaseInsensitive ) == true )
            return( _ISMETADATA );

//----------------------------------------------------------------------------------------------
// Campaign

        if ( s_Parameter.startsWith( "campaign", Qt::CaseInsensitive ) == true )
            return( _ISMETADATA );

        if ( s_Parameter.startsWith( "cruise", Qt::CaseInsensitive ) == true )
            return( _ISMETADATA );

//----------------------------------------------------------------------------------------------
// DOI

        if ( s_Parameter.startsWith( "doi", Qt::CaseInsensitive ) == true )
            return( _ISMETADATA );

//----------------------------------------------------------------------------------------------
// Event

        if ( s_Parameter.startsWith( "event", Qt::CaseInsensitive ) == true )
            return( _ISMETADATA );

//----------------------------------------------------------------------------------------------
// Position

    if ( ( s_Parameter.startsWith( "latitude", Qt::CaseInsensitive ) == true ) || ( s_Parameter == "1600" ) )
        return( _ISGEOCODE );

    if ( ( s_Parameter.startsWith( "longitude", Qt::CaseInsensitive ) == true ) || ( s_Parameter == "1601" ) )
        return( _ISGEOCODE );

//----------------------------------------------------------------------------------------------
// Sediment

    if ( ( s_Parameter.toLower() == "depth water [m]" ) || ( s_Parameter.toLower() == "depth, water [m]" ) || ( s_Parameter == "1619" ) )
        return( _ISGEOCODE );

//----------------------------------------------------------------------------------------------
// Sediment

    if ( ( s_Parameter.toLower() == "depth rock [m]" ) || ( s_Parameter.toLower() == "depth, rock [m]" ) )
        return( _ISGEOCODE );

    if ( ( s_Parameter.toLower() == "depth sediment [m]" ) || ( s_Parameter.toLower() == "depth, sediment [m]" ) )
        return( _ISGEOCODE );

    if ( ( s_Parameter.toLower() == "depth sediment/rock [m]" ) || ( s_Parameter.toLower() == "depth, sediment/rock [m]" ) )
        return( _ISGEOCODE );

    if ( ( s_Parameter.toLower() == "depth sediment [cm]" ) || ( s_Parameter.toLower() == "depth, sediment [cm]" ) )
        return( _ISGEOCODE );

    if ( ( s_Parameter.toLower() == "depth [m]" ) || ( s_Parameter.toLower() == "depth [cm]" ) )
        return( _ISGEOCODE );

    if ( ( s_Parameter == "1" ) || ( s_Parameter == "cm" ) || ( s_Parameter == "m" ) )
        return( _ISGEOCODE );

//----------------------------------------------------------------------------------------------
// Ice/snow

    if ( ( s_Parameter.toLower() == "depth, ice/snow [m]" ) || ( s_Parameter.toLower() == "depth ice/snow [m]" ) )
        return( _ISGEOCODE );

    if ( ( s_Parameter.toLower() == "depth ice [m]" ) || ( s_Parameter.toLower() == "depth, ice [m]" ) || ( s_Parameter == "5059" ) )
        return( _ISGEOCODE );

//----------------------------------------------------------------------------------------------
// Distance

    if ( ( s_Parameter.toLower() == "distance [cm]" ) || ( s_Parameter == "2920" ) )
        return( _ISGEOCODE );

//----------------------------------------------------------------------------------------------
// Point distance

    if ( ( s_Parameter.toLower() == "dist [m]" ) || ( s_Parameter.toLower() == "point distance from start [m]" ) || ( s_Parameter == "26097" ) )
        return( _ISGEOCODE );

//----------------------------------------------------------------------------------------------
// Air, Altitude

    if ( ( s_Parameter.toLower() == "altitude [m]" ) || ( s_Parameter == "4607" ) )
        return( _ISGEOCODE );

//----------------------------------------------------------------------------------------------
// Air, Height

    if ( ( s_Parameter.toLower() == "height [m]" ) || ( s_Parameter.toLower() == "height above ground [m]" ) || ( s_Parameter == "56349" ) )
        return( _ISGEOCODE );

//----------------------------------------------------------------------------------------------
// Date/Time

    if ( ( s_Parameter.toLower() ==  "date/time" ) || ( s_Parameter.toLower() ==  "datetime" ) || ( s_Parameter.toLower() ==  "date/time of event" ) || ( s_Parameter.toLower() ==  "date/time (hourly average)" ) || ( s_Parameter ==  "1599" ) )
        return( _ISGEOCODE );

//----------------------------------------------------------------------------------------------
// Age

    if ( ( s_Parameter.toLower() == "age [kyr bp]" ) || ( s_Parameter.toLower() == "age [kyr]" ) || ( s_Parameter == "2205" ) )
        return( _ISGEOCODE );

    if ( ( s_Parameter.toLower() == "age [ka bp]" ) || ( s_Parameter.toLower() == "age [ka]" ) )
        return( _ISGEOCODE );

//----------------------------------------------------------------------------------------------
// Ordinal number

    if ( ( s_Parameter.toLower() == "ordinal number" ) || ( s_Parameter.toLower() == "no" ) || ( s_Parameter.toLower() == "6262" ) )
        return( _ISGEOCODE );

//----------------------------------------------------------------------------------------------
// Elevation

    if ( ( s_Parameter.toLower() ==  "elevation [m a.s.l.]" ) || ( s_Parameter.toLower() ==  "elevation [m]" ) || ( s_Parameter ==  "8128" ) )
        return( _ISGEOCODE );

//----------------------------------------------------------------------------------------------
// Section

    if ( ( s_Parameter.toLower() ==  "section, height [m]" ) || ( s_Parameter.toLower() ==  "section [m]" ) || ( s_Parameter ==  "25539" ) )
        return( _ISGEOCODE );

//----------------------------------------------------------------------------------------------
// unknown

    return( _ISPARAMETER ); // is parameter or metadata
}
