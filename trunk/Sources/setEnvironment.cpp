// ***********************************************************************************************
// *                                                                                             *
// * setEnvironment.cpp                                                                          *
// *                                                                                             *
// * Dr. Rainer Sieger - 2009-03-24                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtGui>

#include "Application.h"
#include "Globals.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

int MainWindow::setEnvironment( const QString& s_Parameter, int i_EnvArray[] )
{
/* PANGAEA GEOCODE IDs
    ID		Parameter					Abbreviation	Unit
+	1		DEPTH, sediment				Depth			m
+	1599	DATE/TIME					Date/Time
+	1600	LATITUDE					Latitude
+	1601	LONGITUDE					Longitude
+	1619	DEPTH, water				Depth water		m
+	2205	AGE							Age				kyr BP
+	2920	DISTANCE					Distance		cm
+	4607	ALTITUDE					Altitude		m
+	56349	HEIGHT above ground			Height          m
+	5059	DEPTH, ice/snow				Depth ice/snow	m
+	6262	ORDINAL NUMBER				No
+	8128	ELEVATION					Elevation		m a.s.l.
+	25539	SECTION, height				Section			m
+	26097	POINT DISTANCE from start	Dist			m
*/
//----------------------------------------------------------------------------------------------

    if ( s_Parameter.isEmpty() == TRUE )
        return( -1 );

    if ( s_Parameter.toLower() == "qf" ) // ODV Spreadsheet
        return( 0 );

//----------------------------------------------------------------------------------------------
// Event import

    if ( s_Parameter.toLower() == "labelevent" )
    {
        i_EnvArray[14] = EVENTIMPORT;
        return( EVENTIMPORT );
    }

//----------------------------------------------------------------------------------------------
// Water

    if ( ( s_Parameter.toLower() == "depth water [m]" ) || ( s_Parameter.toLower() == "depth, water [m]" ) || ( s_Parameter == "1619" ) )
    {
        i_EnvArray[2] = WATER;
        return( WATER );
    }

//----------------------------------------------------------------------------------------------
// Sediment

    if ( ( s_Parameter.toLower() == "depth sediment [m]" ) || ( s_Parameter.toLower() == "depth, sediment [m]" ) )
    {
        i_EnvArray[3] = SEDIMENT;
        return( SEDIMENT );
    }

    if ( ( s_Parameter.toLower() == "depth sediment [cm]" ) || ( s_Parameter.toLower() == "depth, sediment [cm]" ) )
    {
        i_EnvArray[3] = SEDIMENT;
        return( SEDIMENT );
    }

    if ( ( s_Parameter.toLower() == "depth [m]" ) || ( s_Parameter.toLower() == "depth [cm]" ) )
    {
        i_EnvArray[3] = SEDIMENT;
        return( SEDIMENT );
    }

    if ( ( s_Parameter == "1" ) || ( s_Parameter == "cm" ) || ( s_Parameter == "m" ) )
    {
        i_EnvArray[3] = SEDIMENT;
        return( SEDIMENT );
    }

//----------------------------------------------------------------------------------------------
// Ice/snow

    if ( ( s_Parameter.toLower() == "depth, ice/snow [m]" ) || ( s_Parameter.toLower() == "depth ice/snow [m]" ) )
    {
        i_EnvArray[4] = ICE;
        return( ICE );
    }

    if ( ( s_Parameter.toLower() == "depth ice [m]" ) || ( s_Parameter.toLower() == "depth, ice [m]" ) || ( s_Parameter == "5059" ) )
    {
        i_EnvArray[4] = ICE;
        return( ICE );
    }

//----------------------------------------------------------------------------------------------
// Distance

    if ( ( s_Parameter.toLower() == "distance [cm]" ) || ( s_Parameter == "2920" ) )
    {
        i_EnvArray[5] = DISTANCE;
        return( DISTANCE );
    }

//----------------------------------------------------------------------------------------------
// Point distance

    if ( ( s_Parameter.toLower() == "dist [m]" ) || ( s_Parameter.toLower() == "point distance from start [m]" ) || ( s_Parameter == "26097" ) )
    {
        i_EnvArray[6] = POINTDISTANCE;
        return( POINTDISTANCE );
    }

//----------------------------------------------------------------------------------------------
// Air, Altitude

    if ( ( s_Parameter.toLower() == "altitude [m]" ) || ( s_Parameter == "4607" ) )
    {
        i_EnvArray[7] = AIRA;
        return( AIRA );
    }

//----------------------------------------------------------------------------------------------
// Air, Height

    if ( ( s_Parameter.toLower() == "height [m]" ) || ( s_Parameter.toLower() == "height above ground [m]" ) || ( s_Parameter == "56349" ) )
    {
        i_EnvArray[8] = AIRH;
        return( AIRH );
    }

//----------------------------------------------------------------------------------------------
// Date/Time

    if ( ( s_Parameter.toLower() ==  "date/time" ) || ( s_Parameter.toLower() ==  "datetime" ) || ( s_Parameter.toLower() ==  "date/time of event" ) || ( s_Parameter.toLower() ==  "date/time (hourly average)" ) || ( s_Parameter ==  "1599" ) )
    {
        i_EnvArray[9] = DATETIME;
        return( DATETIME );
    }

//----------------------------------------------------------------------------------------------
// Age

    if ( ( s_Parameter.toLower() == "age [kyr bp]" ) || ( s_Parameter.toLower() == "age [kyr]" ) || ( s_Parameter == "2205" ) )
    {
        i_EnvArray[10] = AGE;
        return( AGE );
    }

    if ( ( s_Parameter.toLower() == "age [ka bp]" ) || ( s_Parameter.toLower() == "age [ka]" ) )
    {
        i_EnvArray[10] = AGE;
        return( AGE );
    }

//----------------------------------------------------------------------------------------------
// Ordinal number

    if ( ( s_Parameter.toLower() == "ordinal number" ) || ( s_Parameter.toLower() == "no" ) || ( s_Parameter.toLower() == "6262" ) )
    {
        i_EnvArray[11] = ORDINALNUMBER;
        return( ORDINALNUMBER );
    }

//----------------------------------------------------------------------------------------------
// Elevation

    if ( ( s_Parameter.toLower() ==  "elevation [m a.s.l.]" ) || ( s_Parameter.toLower() ==  "elevation [m]" ) || ( s_Parameter ==  "8128" ) )
    {
        i_EnvArray[12] = ELEVATION;
        return( ELEVATION );
    }

//----------------------------------------------------------------------------------------------
// Section

    if ( ( s_Parameter.toLower() ==  "section, height [m]" ) || ( s_Parameter.toLower() ==  "section [m]" ) || ( s_Parameter ==  "25539" ) )
    {
        i_EnvArray[13] = SECTION;
        return( SECTION );
    }

//----------------------------------------------------------------------------------------------
// unknown

    return( 0 ); // is parameter or metadata
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

int MainWindow::NumOfEnvironments( int i_EnvArray[] )
{
    int i_NumOfEnvironments = 0;

    for ( int i=1; i<=MAX_NUM_OF_ENV; ++i )
    {
        if ( i_EnvArray[i] > 0 )
            i_NumOfEnvironments++;
    }

    return( i_NumOfEnvironments );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

int MainWindow::findEnvironment( int i_EnvArray[] )
{
    int i_Environment = 0;

    for ( int i=1; i<=MAX_NUM_OF_ENV; ++i )
    {
        if ( i_EnvArray[i] > 0 )
            i_Environment += i_EnvArray[i];
    }

    return( i_Environment );
}
