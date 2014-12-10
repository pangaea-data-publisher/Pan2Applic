// ***********************************************************************************************
// *                                                                                             *
// * buildHeader.cpp                                                                          *
// *                                                                                             *
// * Dr. Rainer Sieger - 2009-08-91                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

int MainWindow::buildHeader( QStringList& sl_Header, QStringList& sl_ParameterList, const int i_Env, const int b_OutputFormat, const bool b_showShortName, const bool b_showMethod, const bool b_showComment )
{
    for ( int i=0; i<sl_ParameterList.count(); ++i )
        sl_Header.append( buildHeaderItem( b_OutputFormat, sl_ParameterList.at( i ), b_showShortName, b_showMethod, b_showComment ) );

// **********************************************************************************************
// add missing header items

    if ( ( sl_Header.contains( "doi", Qt::CaseInsensitive ) == false ) && ( sl_Header.contains( "origin of values", Qt::CaseInsensitive ) == false ) )
    {
        sl_Header.append( tr( "DOI" ) );
        sl_ParameterList.append( QString( "%1" ).arg( i_Env ) + _PSEPARATOR_ + tr( "DOI" ) + _PSEPARATOR_ + tr( "DOI" ) + _PSEPARATOR_ + _PSEPARATOR_ + _PSEPARATOR_  + tr( "Metadata" ) );
    }

// **********************************************************************************************

    return( sl_Header.count() );
}

// **********************************************************************************************
// **********************************************************************************************
// *********************************************************************************************

QString MainWindow::buildHeaderItem( const int b_OutputFormat, const QString& s_Parameter, const bool b_showShortName, const bool b_showMethod, const bool b_showComment )
{
    QString s_HeaderItem    = "";
    QString s_Method        = "";
    QString s_Comment       = "";

// *********************************************************************************************

    if ( s_Parameter.contains( _PSEPARATOR_ ) == false ) // not a PANGAEA dataset
    {
        if ( b_OutputFormat == _FORMAT_FORMATEDTEXTFILE )
            return( tr( "\"" ) + s_Parameter + tr( "\"" ) );
        else
            return( s_Parameter );
    }

// *********************************************************************************************

    if ( b_showMethod == true )
    {
        s_Method = s_Parameter.section( _PSEPARATOR_, 3, 3 );

        if ( s_Method.length() > _MAX_PARAMETER_LENGTH + 3 )
            s_Method = s_Method.left( _MAX_PARAMETER_LENGTH ) + tr( "..." );
    }

// *********************************************************************************************

    if ( b_showComment == true )
    {
        s_Comment = s_Parameter.section( _PSEPARATOR_, 4, 4 );

        if ( s_Comment.length() > _MAX_PARAMETER_LENGTH + 3 )
            s_Comment = s_Comment.left( _MAX_PARAMETER_LENGTH ) + tr ( "..." );
    }

// *********************************************************************************************

    if ( b_showShortName == false )
        s_HeaderItem = s_Parameter.section( _PSEPARATOR_, 1, 1 );
    else
        s_HeaderItem = s_Parameter.section( _PSEPARATOR_, 2, 2 );

    if ( ( s_Method.isEmpty() == true ) && ( s_Comment.isEmpty() == false ) )
        s_HeaderItem.append(  tr( " (" ) + s_Comment + tr( ")" ) );

    if ( ( s_Method.isEmpty() == false ) && ( s_Comment.isEmpty() == true ) )
        s_HeaderItem.append(  tr( " (" ) + s_Method + tr( ")" ) );

    if ( ( s_Method.isEmpty() == false ) && ( s_Comment.isEmpty() == false ) )
        s_HeaderItem.append(  tr( " (" ) + s_Method + tr( " - " ) + s_Comment + tr( ")" ) );

    if ( b_OutputFormat == _FORMAT_FORMATEDTEXTFILE )
        s_HeaderItem = tr( "\"" ) + s_HeaderItem + tr( "\"" );

// **********************************************************************************************

    if ( b_OutputFormat == _FORMAT_ODV )
    {
        s_HeaderItem.replace( " []", "" );
        s_HeaderItem.replace( ";", "," );
    }

    if ( s_HeaderItem.contains( "not given" ) == true )
    {
        s_HeaderItem.replace( " (not given - not given)", "" );
        s_HeaderItem.replace( " - not given)", ")" );
        s_HeaderItem.replace( " (not given - ", " (" );
        s_HeaderItem.replace( " (not given)", "" );
    }

// **********************************************************************************************

    return( s_HeaderItem );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

int MainWindow::buildODVHeader( QStringList& sl_Header, QStringList& sl_ParameterList, QVector<int> &vi_posQV, const int i_Env, const bool b_showShortName, const bool b_showMethod, const bool b_showComment, const bool b_usePANGAEAQualityFlags )
{
    bool b_containsEventLabel = false;

    for ( int i=0; i<sl_ParameterList.count(); ++i )
        if ( ( sl_ParameterList.at( i ).contains( "event", Qt::CaseInsensitive ) == true ) || ( sl_ParameterList.at( i ).contains( "station", Qt::CaseInsensitive ) == true ) )
            b_containsEventLabel = true;

    for ( int i=0; i<sl_ParameterList.count(); ++i )
        sl_Header.append( buildODVHeaderItem( sl_ParameterList.at( i ), b_showShortName, b_showMethod, b_showComment, b_containsEventLabel ) );

// **********************************************************************************************
// add missing header items

    if ( sl_Header.contains( tr( "yyyy-mm-ddThh:mm:ss.sss" ) ) == false )
    {
        sl_Header.append( tr( "yyyy-mm-ddThh:mm:ss.sss" ) );
        sl_ParameterList.append( QString( "%1" ).arg( i_Env ) + _PSEPARATOR_ + tr( "Date/Time" ) + _PSEPARATOR_ + tr( "Date/Time" ) + _PSEPARATOR_ + _PSEPARATOR_ + _PSEPARATOR_ +  tr( "Geocode" )  );
    }

    if ( sl_Header.contains( tr( "Cruise:METAVAR:TEXT:81" ) ) == false )
    {
        sl_Header.append( tr( "Cruise:METAVAR:TEXT:81" ) );
        sl_ParameterList.append( QString( "%1" ).arg( i_Env ) + _PSEPARATOR_ + tr( "Cruise" ) + _PSEPARATOR_ + tr( "Cruise" ) + _PSEPARATOR_ + _PSEPARATOR_ + _PSEPARATOR_ +  tr( "Metadata" )  );
    }

    if ( sl_Header.contains( tr( "Station:METAVAR:TEXT:81" ) ) == false )
    {
        sl_Header.append( tr( "Station:METAVAR:TEXT:81" ) );
        sl_ParameterList.append( QString( "%1" ).arg( i_Env ) + _PSEPARATOR_ + tr( "Station" ) + _PSEPARATOR_ + tr( "Station" ) + _PSEPARATOR_ + _PSEPARATOR_ + _PSEPARATOR_ +  tr( "Metadata" )  );
    }

    if ( sl_Header.contains( tr( "DOI:METAVAR:TEXT:51" ) ) == false )
    {
        sl_Header.append( tr( "DOI:METAVAR:TEXT:51" ) );
        sl_ParameterList.append( QString( "%1" ).arg( i_Env ) + _PSEPARATOR_ + tr( "DOI" ) + _PSEPARATOR_ + tr( "DOI" ) + _PSEPARATOR_ + _PSEPARATOR_ + _PSEPARATOR_ + tr( "Metadata" ) );
    }

    if ( sl_Header.contains( tr( "Type" ) ) == false )
    {
        sl_Header.append( tr( "Type" ) );
        sl_ParameterList.append( QString( "%1" ).arg( i_Env ) + _PSEPARATOR_ + tr( "Type" ) + _PSEPARATOR_ + tr( "Type" ) + _PSEPARATOR_ + _PSEPARATOR_ + _PSEPARATOR_ +  tr( "Metadata" )  );
    }

// **********************************************************************************************

    for ( int i=0; i<sl_Header.count(); ++i )
        vi_posQV.append( -1 );

// **********************************************************************************************

    if ( b_usePANGAEAQualityFlags == true )
    {
        for ( int i=0; i<sl_ParameterList.count(); ++i )
        {
            if ( sl_ParameterList.at( i ).endsWith( "Parameter" ) == true )
            {
                sl_Header.append( "QV:PANGAEA:" + buildODVHeaderItem( sl_ParameterList.at( i ), b_showShortName, b_showMethod, b_showComment, false ) );

                vi_posQV.append( i );
            }
        }
    }

// **********************************************************************************************
// line number

    sl_Header.append( tr( "Line:INTEGER" ) );

    if ( b_usePANGAEAQualityFlags == true )
        sl_Header.append( "QV:PANGAEA:Line:INTEGER" );

// **********************************************************************************************

    if ( ( sl_Header.contains( tr( "Latitude [degress_north]" ) ) == false ) && ( sl_Header.contains( tr( "Longitude [degress_east]" ) ) == false ) )
        return( -70 );

    if ( sl_Header.contains( tr( "Latitude [degress_north]" ) ) == false )
        return( -71 );

    if ( sl_Header.contains( tr( "Longitude [degress_east]" ) ) == false )
        return( -72 );

    return( sl_Header.count() );
}

// **********************************************************************************************
// **********************************************************************************************
// *********************************************************************************************
// 2009-10-20

QString MainWindow::buildODVHeaderItem( const QString& s_Parameter, const bool b_showShortName, const bool b_showMethod, const bool b_showComment, const bool b_containsEventLabel )
{
    QString s_HeaderItem = buildHeaderItem( _FORMAT_ODV, s_Parameter, b_showShortName, b_showMethod, b_showComment );

    if ( s_HeaderItem.toLower() == "campaign" )
        return( tr( "Cruise:METAVAR:TEXT:81" ) );

    if ( s_HeaderItem.toLower() == "campaign label" )
        return( tr( "Cruise:METAVAR:TEXT:81" ) );

    if ( s_HeaderItem.toLower() == "event" )
        return( tr( "Station:METAVAR:TEXT:81" ) );

    if ( s_HeaderItem.toLower() == "event label" )
        return( tr( "Station:METAVAR:TEXT:81" ) );

    if ( s_HeaderItem.toLower() == "labelevent" )
        return( tr( "Station:METAVAR:TEXT:81" ) );

    if ( ( s_HeaderItem.toLower() == "sample code/label" ) && ( b_containsEventLabel == false ) )
        return( tr( "Station:METAVAR:TEXT:81" ) );

    if ( ( s_HeaderItem.toLower() == "core/sample" ) && ( b_containsEventLabel == false ) )
        return( tr( "Station:METAVAR:TEXT:81" ) );

    if ( s_HeaderItem.toLower() == "station" )
        return( tr( "Station:METAVAR:TEXT:81" ) );

    if ( s_HeaderItem.toLower() == "area" )
        return( tr( "Area:METAVAR:TEXT:81" ) );

    if ( s_HeaderItem.toLower() == "area name" )
        return( tr( "Area:METAVAR:TEXT:81" ) );

    if ( s_HeaderItem.toLower() == "device" )
        return( tr( "Device:METAVAR:TEXT:32" ) );

    if ( s_HeaderItem.toLower() == "elevation" )
        return( tr( "Bot. Depth [m]" ) );

    if ( s_HeaderItem.toLower() == "elevation of event [m]" )
        return( tr( "Bot. Depth [m]" ) );

    if ( s_HeaderItem.toLower() == "depth, bathymetric [m]" )
        return( tr( "Bot. Depth [m]" ) );

    if ( s_HeaderItem.toLower() == "date/time" )
        return( tr( "yyyy-mm-ddThh:mm:ss.sss" ) );
    if ( s_HeaderItem.toLower() == "date/time of event" )
        return( tr( "yyyy-mm-ddThh:mm:ss.sss" ) );
    if ( s_HeaderItem.toLower() == "date/time (minutely average)" )
        return( tr( "yyyy-mm-ddThh:mm:ss.sss" ) );
    if ( s_HeaderItem.toLower() == "date/time (hourly average)" )
        return( tr( "yyyy-mm-ddThh:mm:ss.sss" ) );
    if ( s_HeaderItem.toLower() == "date/time (daily average)" )
        return( tr( "yyyy-mm-ddThh:mm:ss.sss" ) );
    if ( s_HeaderItem.toLower() == "date/time (monthly average)" )
        return( tr( "yyyy-mm-ddThh:mm:ss.sss" ) );
    if ( s_HeaderItem.toLower() == "date/time (yearly average)" )
        return( tr( "yyyy-mm-ddThh:mm:ss.sss" ) );

    if ( s_HeaderItem.toLower() == "latitude" )
        return( tr( "Latitude [degress_north]" ) );

    if ( s_HeaderItem.toLower() == "longitude" )
        return( tr( "Longitude [degress_east]" ) );

    if ( s_HeaderItem.toLower() == "lat" )
        return( tr( "Latitude [degress_north]" ) );

    if ( s_HeaderItem.toLower() == "lon" )
        return( tr( "Longitude [degress_east]" ) );

    if ( s_HeaderItem.toLower() == "origin of values" )
        return( tr( "DOI:METAVAR:TEXT:51" ) );

    if ( ( s_HeaderItem.toLower().startsWith( "gear identification number" ) == true ) || ( s_HeaderItem.toLower().startsWith( "gear id" ) == true ) )
        return( tr( "Gear ID:METAVAR:INTEGER" ) );

// **********************************************************************************************
// replace special characters

    s_HeaderItem.replace( "**-12", "~^-~^1~^2" );
    s_HeaderItem.replace( "**-11", "~^-~^1~^1" );
    s_HeaderItem.replace( "**-10", "~^-~^1~^0" );
    s_HeaderItem.replace( "**-9", "~^-~^9" );
    s_HeaderItem.replace( "**-8", "~^-~^8" );
    s_HeaderItem.replace( "**-7", "~^-~^7" );
    s_HeaderItem.replace( "**-6", "~^-~^6" );
    s_HeaderItem.replace( "**-5", "~^-~^5" );
    s_HeaderItem.replace( "**-4", "~^-~^4" );
    s_HeaderItem.replace( "**-3", "~^-~^3" );
    s_HeaderItem.replace( "**-2", "~^-~^2" );
    s_HeaderItem.replace( "**-1", "~^-~^1" );

    s_HeaderItem.replace( "**12", "~^1~^2" );
    s_HeaderItem.replace( "**11", "~^1~^1" );
    s_HeaderItem.replace( "**10", "~^1~^0" );
    s_HeaderItem.replace( "**9", "~^9" );
    s_HeaderItem.replace( "**8", "~^8" );
    s_HeaderItem.replace( "**7", "~^7" );
    s_HeaderItem.replace( "**6", "~^6" );
    s_HeaderItem.replace( "**5", "~^5" );
    s_HeaderItem.replace( "**4", "~^4" );
    s_HeaderItem.replace( "**3", "~^3" );
    s_HeaderItem.replace( "**2", "~^2" );
    s_HeaderItem.replace( "**1", "~^1" );

    s_HeaderItem.replace( "[deg ", "[°" );
    s_HeaderItem.replace( "µ", "~$m~#" );

    if ( s_HeaderItem.contains( "not given" ) == true )
    {
        s_HeaderItem.replace( " (not given - not given)", "" );
        s_HeaderItem.replace( " - not given)", ")" );
        s_HeaderItem.replace( " (not given - ", " (" );
        s_HeaderItem.replace( " (not given)", "" );
    }

// **********************************************************************************************

    return( s_HeaderItem );
}
