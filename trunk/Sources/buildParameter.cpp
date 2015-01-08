// ***********************************************************************************************
// *                                                                                             *
// * buildParameter.cpp                                                                          *
// *                                                                                             *
// * Dr. Rainer Sieger - 2009-03-24                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// called by analyzeDataset

bool MainWindow::isMetadataParameter( const QString& s_Parameter, const QString& s_ParameterName )
{
    if ( s_Parameter.endsWith( "eMetadata") == true )
        return( s_Parameter.endsWith( QString( "%1%2%3%4%5%6%7eMetadata" ).arg( _PSEPARATOR_ ).arg( s_ParameterName ).arg( _PSEPARATOR_ ).arg( s_ParameterName ).arg( _PSEPARATOR_ ).arg( _PSEPARATOR_ ).arg( _PSEPARATOR_ ) ) );

    return( s_Parameter.endsWith( QString( "%1%2%3%4%5%6%7Metadata" ).arg( _PSEPARATOR_ ).arg( s_ParameterName ).arg( _PSEPARATOR_ ).arg( s_ParameterName ).arg( _PSEPARATOR_ ).arg( _PSEPARATOR_ ).arg( _PSEPARATOR_ ) ) );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// called by analyzeDataset

bool MainWindow::isMetadataParameter( const QString& s_Parameter, const QString& s_ParameterName, const QString& s_shortName )
{
    if ( s_Parameter.endsWith( "eMetadata") == true )
        return( s_Parameter.endsWith( QString( "%1%2%3%4%5%6%7eMetadata" ).arg( _PSEPARATOR_ ).arg( s_ParameterName ).arg( _PSEPARATOR_ ).arg( s_shortName ).arg( _PSEPARATOR_ ).arg( _PSEPARATOR_ ).arg( _PSEPARATOR_ ) ) );

    return( s_Parameter.endsWith( QString( "%1%2%3%4%5%6%7Metadata" ).arg( _PSEPARATOR_ ).arg( s_ParameterName ).arg( _PSEPARATOR_ ).arg( s_shortName ).arg( _PSEPARATOR_ ).arg( _PSEPARATOR_ ).arg( _PSEPARATOR_ ) ) );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QStringList MainWindow::buildParameterList( const QString& s_Environment, const QString& s_Header )
{
    QString     sd_Parameter;
    QStringList sl_ParameterList;
    QStringList sld_ParameterList;

    sld_ParameterList = s_Header.split( "\t" );

    for ( int i=0; i<sld_ParameterList.count(); i++ )
    {
        sd_Parameter = sld_ParameterList.at( i );
        sd_Parameter.replace( "\"", "" );

//      sl_ParameterList.append( s_Environment + sd_Parameter.section( "@", 0, 0 ) + _PSEPARATOR_ + sd_Parameter.section( "@", 0, 0 ) );
        sl_ParameterList.append( s_Environment + sd_Parameter + _PSEPARATOR_ + sd_Parameter );
    }

    return( sl_ParameterList );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QString MainWindow::buildParameter( const QString& s_ParameterStr )
{
    int		i               = 0;

    QString sd_Parameter    = "";

    QString s_Parameter     = "";
    QString s_ParameterName = "";
    QString s_ShortName     = "";
    QString s_Unit          = "";

// **********************************************************************************************

    if ( ( s_ParameterStr.contains( "@" ) == true ) && ( s_ParameterStr.endsWith( " * eMetadata" ) == true ) )
        sd_Parameter = s_ParameterStr.section( "@", 1, 1 );
    else
        sd_Parameter = s_ParameterStr;

    sd_Parameter.replace( " * ", "\t" );

    s_Parameter = sd_Parameter.section( "\t", 0, 0 );

    if ( s_Parameter.contains( "[" ) == true )
    {
        s_Parameter.replace( " [", "\t" );  // start of unit
        s_Parameter.replace( "] (", "\t" ); // end of unit, start of short name

        s_ParameterName = s_Parameter.section( "\t", 0, 0 );
        s_Unit          = " [" + s_Parameter.section( "\t", 1, 1 ) + "]";
        s_ShortName     = s_Parameter.section( "\t", 2, 2 ).left( s_Parameter.section( "\t", 2, 2 ).length() - 1 );

        if ( s_ShortName.isEmpty() == true )
            s_ShortName = s_ParameterName;

        s_Parameter = s_ParameterName + s_Unit + _PSEPARATOR_ + s_ShortName + s_Unit + _PSEPARATOR_;
    }
    else
    {
        s_Parameter.replace( " (", "\t" );  // without unit, start of short name, Attention if parameter name contains " ("

        s_ParameterName = s_Parameter.section( "\t", 0, 0 );
        s_ShortName     = s_Parameter.section( "\t", 1, 1 ).left( s_Parameter.section( "\t", 1, 1 ).length() - 1 );

        if ( s_ShortName.isEmpty() == true )
            s_ShortName = s_ParameterName;

        s_Parameter = s_ParameterName + _PSEPARATOR_ + s_ShortName + _PSEPARATOR_;
    }

    if ( sd_Parameter.contains( "PI:") == true ) // PI not used here
        ++i;

    if ( sd_Parameter.contains( "METHOD:") == true )
    {
        ++i;
        QString sd_Method = sd_Parameter.section( "\t" , i, i );
        sd_Method.replace( "METHOD: ", "" );
        sd_Method.replace( " (URI:", "\t" );
        s_Parameter.append( sd_Method.section( "\t", 0, 0 ) );
    }
    else
    {
        if ( ( sd_Parameter.endsWith( "Geocode" ) == false ) && ( sd_Parameter.endsWith( "eMetadata" ) == false ) )
            s_Parameter.append( tr( "not given" ) );
    }

    s_Parameter.append( _PSEPARATOR_ );

    if ( sd_Parameter.contains( "COMMENT:") == true )
    {
        ++i;
        sd_Parameter.replace( "COMMENT: ", "" );
        s_Parameter.append( sd_Parameter.section( "\t", i, i ) );
    }
    else
    {
        if ( ( sd_Parameter.endsWith( "Geocode" ) == false ) && ( sd_Parameter.endsWith( "eMetadata" ) == false ) )
            s_Parameter.append( tr( "not given" ) );
    }

    s_Parameter.append( _PSEPARATOR_ );

    if ( sd_Parameter.section( "\t", i+1, i+1 ).isEmpty() == false )
        s_Parameter.append( sd_Parameter.section( "\t", i+1, i+1 ) );
    else
        s_Parameter.append( tr( "Parameter" ) );

    s_Parameter.replace( "\"", "" );

    return( s_Parameter );
}

// ****************************************************************************************
// ****************************************************************************************
// ****************************************************************************************

QString MainWindow::buildParameter( const QString& InputStr, const bool b_showShortName, const bool b_showMethod, const bool b_showComment )
{
    int     i_Type      = 0;

    QString s_Parameter = "";

// **************************************************************************

    if ( InputStr.section( _PSEPARATOR_, 5, 5 ) == "eMetadata" )
        s_Parameter = "1: ";

    if ( InputStr.section( _PSEPARATOR_, 5, 5 ) == "Metadata" )
        s_Parameter = "2: ";

    if ( InputStr.section( _PSEPARATOR_, 5, 5 ) == "Geocode" )
        s_Parameter = "2: ";

    if ( InputStr.section( _PSEPARATOR_, 5, 5 ) == "Parameter" )
    {
        i_Type      = 1;
        s_Parameter = "2: ";
    }

    if ( ( b_showShortName == false ) || ( InputStr.section( _PSEPARATOR_, 5, 5 ) == "Geocode" ) )
        s_Parameter.append( InputStr.section( _PSEPARATOR_, 1, 1 ) ); // Parameter name
    else
        s_Parameter.append( InputStr.section( _PSEPARATOR_, 2, 2 ) ); // Parameter short name

    if ( ( b_showMethod == true ) && ( i_Type == 1 ) )
    {
        s_Parameter.append( " - " );

        if ( InputStr.section( _PSEPARATOR_, 3, 3 ).isEmpty() == false )
        {
            if ( InputStr.section( _PSEPARATOR_, 3, 3 ).length() > _MAX_PARAMETER_LENGTH + 3 )
                s_Parameter.append( InputStr.section( _PSEPARATOR_, 3, 3 ).left( _MAX_PARAMETER_LENGTH ) + tr( "..." ) ); // Method
            else
                s_Parameter.append( InputStr.section( _PSEPARATOR_, 3, 3 ) ); // Method
        }
        else
        {
            s_Parameter.append( tr( "Method not given" ) );
        }
    }

    if ( ( b_showComment == true ) && ( i_Type == 1 ) )
    {
        s_Parameter.append( " - " );

        if ( InputStr.section( _PSEPARATOR_, 4, 4 ).isEmpty() == false )
        {
            if ( InputStr.section( _PSEPARATOR_, 4, 4 ).length() > _MAX_PARAMETER_LENGTH + 3 )
                s_Parameter.append( InputStr.section( _PSEPARATOR_, 4, 4 ).left( _MAX_PARAMETER_LENGTH ) + tr( "..." ) ); // Comment
            else
                s_Parameter.append( InputStr.section( _PSEPARATOR_, 4, 4 ) ); // Comment
        }
        else
        {
            s_Parameter.append( tr( "Comment not given" ) );
        }
    }

    s_Parameter.replace( "\"", "" );

    return( s_Parameter );
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

QString MainWindow::buildParameter( const QString& Item, const QStringList& sl_ParameterList, const bool b_showShortName, const bool b_showMethod, const bool b_showComment )
{
    int     i               = -1;

    QString sd              = Item;

    QString sd1             = "";
    QString sd2             = "";

    QString s_Parameter     = "";

// ***********************************************************************************************************************

    if ( ( Item.startsWith( "1: " ) == true ) || ( Item.startsWith( "2: " ) == true ) || ( Item.startsWith( "3: " ) == true ) )
        sd = Item.section( ": ", 1, 1 ).section( " - ", 0, 0 );

    if ( b_showMethod == true )
        sd1.append( Item.section( " - ", 1, 1 ).section( "...", 0, 0 ) );

    if ( ( b_showMethod == true ) && ( b_showComment == true ) )
        sd2.append( Item.section( " - ", 2, 2 ).section( "...", 0, 0 ) );

    if ( ( b_showMethod == false ) && ( b_showComment == true ) )
        sd2.append( Item.section( " - ", 1, 1 ).section( "...", 0, 0 ) );

    while ( ++i < sl_ParameterList.count() )
    {
        if ( ( b_showShortName == false ) || ( sl_ParameterList.at( i ).section( _PSEPARATOR_, 5, 5 ) == "Geocode" ) )
        {
            if ( sl_ParameterList.at( i ).section( _PSEPARATOR_, 1, 1 ) == sd )
                s_Parameter = sl_ParameterList.at( i );
        }
        else
        {
            if ( sl_ParameterList.at( i ).section( _PSEPARATOR_, 2, 2 ) == sd )
                s_Parameter = sl_ParameterList.at( i );
        }

        if ( ( s_Parameter.isEmpty() == false ) && ( b_showMethod == true ) && ( sd1 != "Method not given" ) && ( sd1.isEmpty() == false ) ) // Method
            if ( s_Parameter.section( _PSEPARATOR_, 3, 3 ).startsWith( sd1 ) == false )
                s_Parameter = "";

        if ( ( s_Parameter.isEmpty() == false ) && ( b_showComment == true ) && ( sd2 != "Comment not given" ) && ( sd2.isEmpty() == false ) ) // Comment
            if ( s_Parameter.section( _PSEPARATOR_, 4, 4 ).startsWith( sd2 ) == false )
                s_Parameter = "";

        if ( ( b_showMethod == false ) && ( b_showComment == false ) && ( s_Parameter.isEmpty() == true ) )
        {
            if ( sl_ParameterList.at( i ).section( _PSEPARATOR_, 1, 1 ) == Item )
                s_Parameter = Item;
        }

        if ( s_Parameter.isEmpty() == false )
            i = sl_ParameterList.count();
    }

    s_Parameter.replace( "\"", "" );

    return( s_Parameter );
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

int MainWindow::getFirstPos( const QStringList& sl_ParameterList, const QString& s_Parameter )
{
    for ( int i=0; i<sl_ParameterList.count(); ++i )
    {
        if ( sl_ParameterList.at( i ).toLower() == s_Parameter.toLower() )
            return( i );

        if ( sl_ParameterList.at( i ).section( _PSEPARATOR_, 1, 1 ).toLower() == s_Parameter.toLower() )
            return( i );

        if ( sl_ParameterList.at( i ).section( _PSEPARATOR_, 2, 2 ).toLower() == s_Parameter.toLower() )
            return( i );
    }

    return( -1 ); // not found
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

void MainWindow::getSpecialPos( const QStringList& sl_ParameterList, QVector<int> &vi_s_ParameterSpecialPos )
{
    for ( int i=0; i<sl_ParameterList.count(); ++i )
    {
        vi_s_ParameterSpecialPos.append( _ISDOUBLE );

        if ( sl_ParameterList.at( i ).contains( _PSEPARATOR_ ) == true )
        {
            if ( sl_ParameterList.at( i ).section( _PSEPARATOR_, 1, 1 ).startsWith( "date/time", Qt::CaseInsensitive ) == true )
                vi_s_ParameterSpecialPos[i] = _ISDATETIME;

            if ( sl_ParameterList.at( i ).section( _PSEPARATOR_, 1, 1 ).startsWith( "elevation", Qt::CaseInsensitive ) == true )
                vi_s_ParameterSpecialPos[i] = _ISELEVATION;

            if ( sl_ParameterList.at( i ).section( _PSEPARATOR_, 1, 1 ).startsWith( "event", Qt::CaseInsensitive ) == true )
                vi_s_ParameterSpecialPos[i] = _ISEVENT;

            if ( sl_ParameterList.at( i ).section( _PSEPARATOR_, 1, 1 ).startsWith( "station", Qt::CaseInsensitive ) == true )
                vi_s_ParameterSpecialPos[i] = _ISEVENT;

            if ( sl_ParameterList.at( i ).section( _PSEPARATOR_, 1, 1 ).startsWith( "latitude", Qt::CaseInsensitive ) == true )
                vi_s_ParameterSpecialPos[i] = _ISLATITUDE;

            if ( sl_ParameterList.at( i ).section( _PSEPARATOR_, 1, 1 ).startsWith( "longitude", Qt::CaseInsensitive ) == true )
                vi_s_ParameterSpecialPos[i] = _ISLONGITUDE;
        }
        else
        {
            if ( sl_ParameterList.at( i ).startsWith( "date/time", Qt::CaseInsensitive ) == true )
                vi_s_ParameterSpecialPos[i] = _ISDATETIME;

            if ( sl_ParameterList.at( i ).startsWith( "elevation", Qt::CaseInsensitive ) == true )
                vi_s_ParameterSpecialPos[i] = _ISELEVATION;

            if ( sl_ParameterList.at( i ).startsWith( "event", Qt::CaseInsensitive ) == true )
                vi_s_ParameterSpecialPos[i] = _ISEVENT;

            if ( sl_ParameterList.at( i ).startsWith( "station", Qt::CaseInsensitive ) == true )
                vi_s_ParameterSpecialPos[i] = _ISEVENT;

            if ( sl_ParameterList.at( i ).startsWith( "latitude", Qt::CaseInsensitive ) == true )
                vi_s_ParameterSpecialPos[i] = _ISLATITUDE;

            if ( sl_ParameterList.at( i ).startsWith( "longitude", Qt::CaseInsensitive ) == true )
                vi_s_ParameterSpecialPos[i] = _ISLONGITUDE;
        }
    }
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

int MainWindow::getGeocodePosition( const QStringList& sl_ParameterList, const int i_Env )
{
// ***********************************************************************************************************************
// PANGAEA Datasets

    QString s_EnvironmentName = getEnvironmentName( i_Env );

    for ( int i=0; i<sl_ParameterList.count(); ++i )
    {
        if ( ( sl_ParameterList.at( i ).section( _PSEPARATOR_, 1, 1 ).toLower() == s_EnvironmentName.toLower() ) && ( sl_ParameterList.at( i ).endsWith( tr( "Geocode" ) ) == true ) )
            return( i );
    }

// ***********************************************************************************************************************
// Datawarehouse

    QString s_EnvironmentShortName = getEnvironmentShortName( i_Env );

    switch( i_Env )
    {
        case _DATETIME:
            for ( int i=0; i<sl_ParameterList.count(); ++i )
            {
                if ( sl_ParameterList.at( i ).startsWith( "date/time (minutely average)", Qt::CaseInsensitive ) == true )
                    return( i );
                if ( sl_ParameterList.at( i ).startsWith( "date/time (hourly average)", Qt::CaseInsensitive ) == true )
                    return( i );
                if ( sl_ParameterList.at( i ).startsWith( "date/time (daily average)", Qt::CaseInsensitive ) == true )
                    return( i );
                if ( sl_ParameterList.at( i ).startsWith( "date/time (monthly average)", Qt::CaseInsensitive ) == true )
                    return( i );
                if ( sl_ParameterList.at( i ).startsWith( "date/time (yearly average)", Qt::CaseInsensitive ) == true )
                    return( i );
                if ( sl_ParameterList.at( i ).startsWith( "date/time", Qt::CaseInsensitive ) == true )
                    return( i );
            }
            break;

        default:
            for ( int i=0; i<sl_ParameterList.count(); ++i )
            {
                if ( sl_ParameterList.at( i ).contains( s_EnvironmentShortName, Qt::CaseInsensitive ) == true )
                    return( i );
            }
            break;
    }

// ***********************************************************************************************************************
// no Geocode found

    return( -1 );
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

int MainWindow::getGearIDPosition( const QStringList& sl_ParameterList )
{
// ***********************************************************************************************************************
// PANGAEA Datasets

    for ( int i=0; i<sl_ParameterList.count(); ++i )
    {
        if ( ( sl_ParameterList.at( i ).section( _PSEPARATOR_, 1, 1 ).toLower().startsWith( "gear identification number" ) == true ) && ( sl_ParameterList.at( i ).section( _PSEPARATOR_, 1, 1 ).toLower().startsWith( "gear id" ) == true ) )
            return( i );
    }

// ***********************************************************************************************************************
// no Gear ID found

    return( -1 );
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

QString MainWindow::getEnvironmentName( const int i_Env )
{
    switch ( i_Env )
    {
        case _NOTGIVEN:
            return( "not given" );
            break;
        case _WATER:
            return( "Depth, water [m]" );
            break;
        case _SEDIMENT:
            return( "Depth, sediment/rock [m]" );
            break;
        case _ICE:
            return( "Depth, ice/snow [m]" );
            break;
        case _DISTANCE:
            return( "Distance [cm]" );
            break;
        case _POINTDISTANCE:
            return( "Point distance from start [m]" );
            break;
        case _AIRA:
            return( "Altitude [m]" );
            break;
        case _AIRH:
            return( "Height above ground [m]" );
            break;
        case _DATETIME:
            return( "Date/Time" );
            break;
        case _AGE:
            return( "Age [ka BP]" );
            break;
        case _ORDINALNUMBER:
            return( "Ordinal number" );
            break;
        case _ELEVATION:
            return( "Elevation [m a.s.l.]" );
            break;
        case _SECTION:
            return( "Section, height [m]" );
            break;
        default:
            return( "not given" );
            break;
    }

    return( "xxx" );
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

QString MainWindow::getEnvironmentShortName( const int i_Env )
{
    switch ( i_Env )
    {
        case _NOTGIVEN:
            return( "not given" );
            break;

        case _WATER:
            return( "Depth water [m]" );
            break;

        case _SEDIMENT:
            return( "Depth [m]" );
            break;

        case _ICE:
            return( "Depth ice/snow [m]" );
            break;

        case _DISTANCE:
            return( "Distance [cm]" );
            break;

        case _POINTDISTANCE:
            return( "Dist [m]" );
            break;

        case _AIRA:
            return( "Altitude [m]" );
            break;

        case _AIRH:
            return( "Height [m]" );
            break;

        case _DATETIME:
            return( "Date/Time" );
            break;

        case _AGE:
            return( "Age [ka BP]" );
            break;

        case _ORDINALNUMBER:
            return( "No" );
            break;

        case _ELEVATION:
            return( "Elevation [m a.s.l.]" );
            break;

        case _SECTION:
            return( "Section [m]" );
            break;

        default:
            return( "not given" );
            break;
    }

    return( "xxx" );
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

QStringList MainWindow::buildODVOutputStr( const int i_LineNo, const bool b_addLineNo, const QString& s_Input, const QString& s_Metadata,
                                       const QVector<int> vi_pos, const QVector<int> vi_posQV, const QVector<int> vi_s_ParameterSpecialPos,
                                       const bool b_usePANGAEAQualityFlags,
                                       const bool b_containsDOI, const QString& s_DeviceType,
                                       const QString& s_FieldDelimiter )
{
    QString     s_Cruise = "";

    QStringList sl_Output;

// ***********************************************************************************************************************

    for ( int j=0; j<vi_pos.count(); ++j )
    {
        if ( vi_pos[j] >= 0 )
        {   // Parameter
            switch ( vi_s_ParameterSpecialPos[j] )
            {
            case _ISDATETIME:
                sl_Output.append( buildDateTimeStr( s_Input.section( "\t", vi_pos[j], vi_pos[j] ), _FORMAT_ISO ) );
                break;

            case _ISELEVATION:
                sl_Output.append( QString( "%1" ).arg( -1. * s_Input.section( "\t", vi_pos[j], vi_pos[j] ).toDouble() ) );
                break;

            case _ISEVENT:
                if ( b_addLineNo == true )
                    sl_Output.append(  QString( "%1_%2" ).arg( s_Input.section( "\t", vi_pos[j], vi_pos[j] ) ).arg( i_LineNo ) );
                else
                    sl_Output.append( s_Input.section( "\t", vi_pos[j], vi_pos[j] ) );
                break;

            case _ISLATITUDE:
            case _ISLONGITUDE:
                if ( s_Input.section( "\t", vi_pos[j], vi_pos[j] ).isEmpty() == true )
                    sl_Output.append( "delete line" );
                else
                    sl_Output.append( s_Input.section( "\t", vi_pos[j], vi_pos[j] ) );
                break;

            default:
                sl_Output.append( s_Input.section( "\t", vi_pos[j], vi_pos[j] ) );
                break;
            }
        }
        else
        {   // Metadata
            switch ( vi_pos[j] )
            {
            case -4: // Campaign
                s_Cruise = s_Metadata.section( "\t", -vi_pos[j], -vi_pos[j] );
                s_Cruise.replace( " (", " - " );
                s_Cruise.replace( ")", "" );
                sl_Output.append( s_Cruise );
                break;

            case -5: // Event
                if ( s_Metadata.section( "\t", -vi_pos[j], -vi_pos[j] ) == "xxx" )
                {
                    if ( b_addLineNo == true )
                        sl_Output.append(  QString( "Line_%1" ).arg( i_LineNo ) );
                    else
                        sl_Output.append( "" );
                }
                else
                {
                    if ( b_addLineNo == true )
                        sl_Output.append(  QString( "%1_%2" ).arg( s_Metadata.section( "\t", -vi_pos[j], -vi_pos[j] ) ).arg( i_LineNo ) );
                    else
                        sl_Output.append( s_Metadata.section( "\t", -vi_pos[j], -vi_pos[j] ) );
                }
                break;

            case -6: // Gear
                sl_Output.append( s_Metadata.section( "\t", -vi_pos[j], -vi_pos[j] ) );
                break;

            case -7: // Date
                sl_Output.append( tr( "-" ) );
                break;

            case -8: // Time
                sl_Output.append( tr( "-" ) );
                break;

            case -9: // Date/Time of event
                sl_Output.append( buildDateTimeStr( s_Metadata, -vi_pos[j], _FORMAT_ISO ) );
                break;

            case -10: // Date/Time start
                sl_Output.append( buildDateTimeStr( s_Metadata, -vi_pos[j], _FORMAT_ISO ) );
                break;

            case -11: // Latitude
                sl_Output.append( s_Metadata.section( "\t", -vi_pos[j], -vi_pos[j] ) );
                break;

            case -12: // Longitude
                sl_Output.append( s_Metadata.section( "\t", -vi_pos[j], -vi_pos[j] ) );
                break;

            case -13: // Elevation of event
                sl_Output.append( QString( "%1" ).arg( -1. * s_Metadata.section( "\t", -vi_pos[j], -vi_pos[j] ).toFloat() ) );
                break;

            case -14: // Area
                sl_Output.append( s_Metadata.section( "\t", -vi_pos[j], -vi_pos[j] ) );
                break;

            case -15: // DOI
                sl_Output.append( s_Metadata.section( "\t", -vi_pos[j], -vi_pos[j] ) );
                break;

            case -31: // ODV Device Type
                sl_Output.append( s_DeviceType );
                break;

            case -999:
                sl_Output.append( tr( "-" ) );
                break;

            default:
                sl_Output.append( tr( "-" ) );
//              sl_Output.append( s_Metadata.section( "\t", -vi_pos[j], -vi_pos[j] ) );
                break;
            }
        }
    }

// ***********************************************************************************************************************

    QString s_Output = removeQualityFlag( sl_Output, s_FieldDelimiter );

    if ( b_usePANGAEAQualityFlags == true )
        s_Output.append( setQualityFlag( sl_Output, vi_posQV, s_FieldDelimiter ) );

    if ( b_containsDOI == true )
    {
        s_Output.replace( tr( "http://doi.pangaea.de/" ), tr( "doi:" ) );
        s_Output.replace( tr( ", DOI registration in progress" ), tr( "" ) );
        s_Output.replace( tr( "Dataset ID: " ), tr( "doi:10.1594/PANGAEA." ) );
        s_Output.replace( tr( ", unpublished dataset" ), tr( "" ) );
        s_Output.replace( tr( "xxx" ), tr( "" ) );
    }

    s_Output.append( s_FieldDelimiter + QString( "%1" ).arg( i_LineNo ) );

    if ( b_usePANGAEAQualityFlags == true )
        s_Output.append( s_FieldDelimiter + "#" );

    return( s_Output.split( s_FieldDelimiter ) );
}
