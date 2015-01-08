// ***********************************************************************************************
// *                                                                                             *
// * setMetadata.cpp                                                                             *
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

int MainWindow::setMetadata( const QString& s_Parameter,
                             bool& b_containsDate, QString& s_Date,
                             bool& b_containsTime, QString& s_Time,
                             bool& b_containsElevation, QString& s_Elevation,
                             bool& b_containsEventLabel, QString& s_EventLabel,
                             bool& b_containsCruiseLabel, QString& s_CruiseLabel,
                             bool& b_containsAreaName, QString& s_Area,
                             bool& b_containsGear, QString& s_Gear,
                             bool& b_containsDataLatitude, QString& s_DataLatitude,
                             bool& b_containsDataLongitude, QString& s_DataLongitude)
{
//----------------------------------------------------------------------------------------------
// Event label

    if ( ( s_Parameter.simplified().toLower() == "event" ) || ( s_Parameter.simplified().toLower() == "event label" ) )
	{
		b_containsEventLabel	= TRUE;
		s_EventLabel			= "xxx";
		return( EVENT );		
	}

    if ( ( s_Parameter.simplified().toLower() == "labelevent" ) || ( s_Parameter.simplified().toLower() == "station" ) )
	{
		b_containsEventLabel	= TRUE;
		s_EventLabel			= "xxx";
		return( EVENT );
	}

    if ( ( s_Parameter.simplified().toLower() == "sample code/label" ) || ( s_Parameter.simplified().toLower() == "label" ) )
	{
		b_containsEventLabel	= TRUE;
		s_EventLabel			= "xxx";
		return( EVENT );
	}

//----------------------------------------------------------------------------------------------
// Latitude

    if ( ( s_Parameter.simplified().toLower() == "lat (°n)" ) || ( s_Parameter == "latitude [degrees_north]" ) ) // ODV Spreadsheet
	{
		b_containsDataLatitude	= TRUE;
		s_DataLatitude			= "xxx";
		return( LATITUDE );
	}

    if ( ( s_Parameter.simplified().toLower().startsWith( "latitude") ) || ( s_Parameter == "1600" ) )
	{
		b_containsDataLatitude	= TRUE;
		s_DataLatitude			= "xxx";
		return( LATITUDE );
	}

//----------------------------------------------------------------------------------------------
// Longitude

    if ( ( s_Parameter.simplified().toLower() == "lon (°e)" ) || ( s_Parameter.simplified().toLower() == "longitude [degrees_east]" ) ) // ODV Spreadsheet
	{
		b_containsDataLongitude	= TRUE;
		s_DataLongitude			= "xxx";
		return( LONGITUDE );
	}

    if ( ( s_Parameter.simplified().toLower().startsWith( "longitude") ) || ( s_Parameter == "1601" ) )
	{
		b_containsDataLongitude	= TRUE;
		s_DataLongitude			= "xxx";
		return( LONGITUDE );
	}

//----------------------------------------------------------------------------------------------
// Elevation

    if ( ( s_Parameter.simplified().toLower() == "elevation of event" ) || ( s_Parameter.simplified().toLower() == "elevationevent" ) || ( s_Parameter.simplified().toLower() == "elevation" ) || ( s_Parameter.simplified().toLower() == "elevation [m]" ) ) // || ( s_Parameter.simplified().toLower() == "bot. depth [m]" ) || ( s_Parameter.simplified().toLower() == "bathy depth [m]" ) )
	{
		b_containsElevation	= TRUE;
		s_Elevation			= "xxx";
		return( ELEVATIONEVENT );		
	}

//----------------------------------------------------------------------------------------------
// Campaign

    if ( ( s_Parameter.simplified().simplified().toLower() == "cruise" ) || ( s_Parameter.simplified().toLower() == "campaign" ) || ( s_Parameter.simplified().toLower() == "campaign label" ) || ( s_Parameter.simplified().toLower() == "expedition" ) )
	{
		b_containsCruiseLabel	= TRUE;
		s_CruiseLabel			= "xxx";
		return( CRUISE );
	}

//----------------------------------------------------------------------------------------------
// Area

    if ( s_Parameter.simplified().toLower() == "area" )
	{
		b_containsAreaName	= TRUE;
		s_Area				= "xxx";
		return( AREA );
	}

//----------------------------------------------------------------------------------------------
// Gear type	

    if ( ( s_Parameter.simplified().toLower() == "type" ) || ( s_Parameter.simplified().toLower() == "gear" ) )
	{
		b_containsGear	= TRUE;
		s_Gear			= "xxx";
		return( GEAR );
	}

//----------------------------------------------------------------------------------------------
// Date

    if ( ( s_Parameter.simplified().toLower() ==  "date" ) || ( s_Parameter.simplified().toLower() ==  "dateevent" ) || ( s_Parameter.simplified().toLower() == "mon/day/yr" ) )
	{
		b_containsDate	= TRUE;
		s_Date			= "xxx";
		return( DATE );
	}

//----------------------------------------------------------------------------------------------
// Time

    if ( ( s_Parameter.simplified().toLower() == "time" ) || ( s_Parameter.simplified().toLower() == "timeevent" ) || ( s_Parameter.simplified().toLower() == "hh:mm") )
	{
		b_containsTime	= TRUE;
		s_Time			= "xxx";
		return( TIME );
	}

//----------------------------------------------------------------------------------------------
// Date/Time

    if ( ( s_Parameter.simplified().toLower() == "date/time" ) || ( s_Parameter.simplified().toLower() == "date/time of event" ) || ( s_Parameter.simplified().toLower() == "timeevent" ) || ( s_Parameter.toLower() ==  "date/time (hourly average)" ) )
	{
		b_containsDate	= TRUE;
		s_Date			= "xxx";
		b_containsTime	= TRUE;
		s_Time			= "xxx";
		return( DATE );
	}

//----------------------------------------------------------------------------------------------
// Error

	if ( s_Parameter.length() ==  0 )
		return( ERROR );		

    if ( s_Parameter.simplified().toLower() ==  "qf" ) // ODV Spreadsheet
		return( ERROR );		

    if ( ( s_Parameter.simplified().toLower() == "dateevent2" ) || ( s_Parameter.simplified().toLower() == "timeevent2" ) || ( s_Parameter.simplified().toLower() == "remarks" ) )
		return( ERROR );		

//----------------------------------------------------------------------------------------------
// unknown

	return( 0 ); // is parameter
}
