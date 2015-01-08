// ***********************************************************************************************
// *                                                                                             *
// * analyzeDataset.cpp                                                                          *
// *                                                                                             *
// * Dr. Rainer Sieger - 2009-03-24                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2011-03-11  - Tsunami Hits Japan Coast

int MainWindow::analyzeDataset( const QString& s_FilenameIn, const int i_CodecInput, const bool b_showShortName, const bool b_showMethod, const bool b_showComment, QStringList& sl_MetadataList, QStringList& sl_ParameterList, int i_EnvArray[] )
{
    int             i                   = 0;
    int             err                 = 0;

    int             i_MetaInfo          = 0;
    int             i_Environment       = 0;
    int             i_ParameterType     = 0;

    int             i_NumOfParameters   = 0;
    int             i_NumOfDataPoints   = 0;

    int             i_pos               = 0;

    int             n                   = 0;

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

    QString         s_Environment       = "xxx";
    QString         s_EventLabel        = "xxx";
    QString         s_Latitude          = "xxx";
    QString         s_Longitude         = "xxx";
    QString         s_CampaignLabel     = "xxx";
    QString         s_AreaName          = "xxx";
    QString         s_GearName          = "xxx";
    QString         s_Date              = "xxx";
    QString         s_Time              = "xxx";
    QString         s_DateTimeStart     = "xxx";
    QString         s_DateTimeEnd       = "xxx";
    QString         s_DOI               = "xxx";
    QString         s_Citation          = "xxx";
    QString         s_Elevation         = "xxx";

    QString         sd_Parameter        = "";

    QStringList     sld_ParameterList;
    QStringList     sl_Input;

    bool            b_containsPANGAEAHeader     = false;
    bool            b_containsEventBlock		= false;
    bool            b_containsParameterBlock	= false;

// **********************************************************************************************
// read data file

    if ( ( n = readFile( s_FilenameIn, sl_Input, i_CodecInput, 50000 ) ) < 1 ) // reads the first 50000 characters
        return( n );

// **********************************************************************************************
// is data file valid?

    if ( sl_Input.contains( "/* TABULAR SUMMARY OF DATASETS LISTED IN THIS COLLECTION: */" ) == true ) // file is a parent summary
        return( 0 );

    if ( n <= 1 ) // data file is empty or contains only one line (no data available)
        return( 0 );

    if ( sl_Input.at( 0 ) == "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2//EN\">" ) // login required
        return( 0 );

    if ( sl_Input.at( 0 ).startsWith( "<html><head><title>Request failed:" ) ) // request failed:
        return( 0 );

// **********************************************************************************************

    if ( sl_Input.at( 0 ).startsWith( "/* DATA DESCRIPTION:" ) == true ) // file starts with PANGAEA header
    {
        b_containsPANGAEAHeader = true;

        sld_ParameterList.clear();

        while ( ( i < n ) && ( sl_Input.at( i ).startsWith( "*/" ) == false ) )
        {
            if ( sl_Input.at( i ).startsWith( "Citation:" ) == true )
            {
                s_DOI      = getDOI( sl_Input.at( i ) );
                s_Citation = getCitation( sl_Input.at( i ) );
            }

// **********************************************************************************************
// get Date/Time Start from coverage
// DATE/TIME START: 2005-10-31T00:35:05 * DATE/TIME END: 2005-11-09T06:52:43

            if ( sl_Input.at( i ).contains( "DATE/TIME START: " ) == true )
            {
                s_DateTimeStart = sl_Input.at( i ).section( "\t", 1, 1 ).section( " * ", 0, 0 );
                s_DateTimeStart.replace( "DATE/TIME START: ", "" );

                s_DateTimeEnd = sl_Input.at( i ).section( "\t", 1, 1 ).section( " * ", 1, 1 );
                s_DateTimeEnd.replace( "DATE/TIME END: ", "" );

                gs_GEOCODE_min = s_DateTimeStart;
                gs_GEOCODE_max = s_DateTimeEnd;
            }

// **********************************************************************************************
// get first event

            if ( sl_Input.at( i ).startsWith( "Event(s):" ) == true )
            {
                b_containsEventBlock = true;

                err = getEventMetadata( sl_Input.at( i ), s_Date, s_Time, s_Elevation, s_EventLabel, s_CampaignLabel, s_AreaName, s_GearName, s_Latitude, s_Longitude, sld_ParameterList );
            }

// **********************************************************************************************
// get parameter(s)

            if ( sl_Input.at( i ).startsWith( "Parameter(s):" ) == true )
            {
                b_containsParameterBlock = true;

                while ( ( i < n ) && ( ( sl_Input.at( i ).startsWith( "Parameter(s):" ) == true ) || ( sl_Input.at( i ).startsWith( "\t" ) == true ) ) )
                    sld_ParameterList.append( sl_Input.at( i++ ).section( "\t", 1, 1 ) );
            }

// **********************************************************************************************
// get size

            if ( sl_Input.at( i ).startsWith( "Size:" ) == true )
                i_NumOfDataPoints = sl_Input.at( i ).section( "\t", 1, 1 ).section( " data points", 0, 0).toInt();

// **********************************************************************************************

            ++i;
        }

        ++i;
    }

// **********************************************************************************************
//  PANGAEA surface dataset

    if ( b_containsPANGAEAHeader == true )
    {
// **********************************************************************************************
// get environment if data file contains a parameter block

        i_NumOfParameters = sld_ParameterList.count();

        for ( int j=0; j<i_NumOfParameters; ++j )
        {
            sd_Parameter = buildParameter( sld_ParameterList.at( j ) );

            if ( sd_Parameter.endsWith( "Geocode" ) == true )
                i_Environment += getEnvironment( sd_Parameter.section( _PSEPARATOR_, 0, 0 ), i_EnvArray );
        }

// **********************************************************************************************
// get metadata from data section

        getDataMetadataPosition( b_containsPANGAEAHeader, sl_Input.at( i ), i_DatePos, i_TimePos, i_DateTimePos, i_ElevationPos, i_EventLabelPos, i_CampaignLabelPos, i_AreaNamePos, i_GearNamePos, i_LatitudePos, i_LongitudePos, sld_ParameterList );
    }

// **********************************************************************************************

    if ( b_containsPANGAEAHeader == false )
    {
// **********************************************************************************************
// get metadata from data section if data file is not a PANGAEA dataset

        n = NumOfSections( sl_Input.at( i ) );  // sl_Input.at( i ) = Data header line

        for ( int j=0; j<n; ++j )
            i_Environment += getEnvironment( sl_Input.at( i ).section( "\t", j, j ), i_EnvArray );

        i_NumOfParameters = getDataMetadataPosition( b_containsPANGAEAHeader, sl_Input.at( i ), i_DatePos, i_TimePos, i_DateTimePos, i_ElevationPos, i_EventLabelPos, i_CampaignLabelPos, i_AreaNamePos, i_GearNamePos, i_LatitudePos, i_LongitudePos, sld_ParameterList );
    }

// **********************************************************************************************
// build parameter list

    s_Environment = QString( "%1" ).arg( i_Environment ) + _PSEPARATOR_;

    for ( int j=0; j<i_NumOfParameters; ++j )
    {
        if ( b_containsParameterBlock == true )
        {
            sd_Parameter = s_Environment + buildParameter( sld_ParameterList.at( j ) );
            i_pos = containsParameter( true, sl_ParameterList, sd_Parameter, b_showShortName, b_showMethod, b_showComment );
        }
        else
        {
            i_ParameterType = getParameterType( sld_ParameterList.at( j ) );

            switch ( i_ParameterType )
            {
            case _ISGEOCODE:
                sd_Parameter = s_Environment + sld_ParameterList.at( j ) + _PSEPARATOR_ + sld_ParameterList.at( j ) + _PSEPARATOR_ + _PSEPARATOR_ + _PSEPARATOR_ + "Geocode";
                break;

            case _ISMETADATA:
                sd_Parameter = s_Environment + sld_ParameterList.at( j ) + _PSEPARATOR_ + sld_ParameterList.at( j ) + _PSEPARATOR_ + _PSEPARATOR_ + _PSEPARATOR_ + "Metadata";
                break;

            default:
                sd_Parameter = s_Environment + sld_ParameterList.at( j ) + _PSEPARATOR_ + sld_ParameterList.at( j ) + _PSEPARATOR_ + _PSEPARATOR_ + _PSEPARATOR_ + "Parameter";
                break;
            }

            i_pos = containsParameter( false, sl_ParameterList, sd_Parameter, b_showShortName, b_showMethod, b_showComment );
        }

        if ( ( i_pos < 0 ) || ( i_pos > sl_ParameterList.count() ) )
            sl_ParameterList.append( sd_Parameter );
    }

// **********************************************************************************************
// set Header

    if ( b_containsPANGAEAHeader == false )
        i = 1;

// **********************************************************************************************
// set Metainfo

    if ( b_containsEventBlock == true )
        i_MetaInfo = 1;

    if ( i_LatitudePos > -1 )
        ++i_MetaInfo;

    if ( i_LongitudePos > -1 )
        ++i_MetaInfo;

// **********************************************************************************************
// build metadata string

    QString s_Metadata = s_FilenameIn;                                      // 0
    s_Metadata.append( QString( "\t%1" ).arg( i_MetaInfo ) );               // 1
    s_Metadata.append( QString( "\t%1" ).arg( i ) );                        // 2
    s_Metadata.append( QString( "\t%1" ).arg( i_Environment ) );            // 3
    s_Metadata.append( QString( "\t%1" ).arg( s_CampaignLabel ) );          // 4
    s_Metadata.append( QString( "\t%1" ).arg( s_EventLabel ) );				// 5
    s_Metadata.append( QString( "\t%1" ).arg( s_GearName ) );				// 6
    s_Metadata.append( QString( "\t%1" ).arg( s_Date ) );					// 7
    s_Metadata.append( QString( "\t%1" ).arg( s_Time ) );					// 8
    s_Metadata.append( QString( "\t%1T%2" ).arg( s_Date ).arg( s_Time ) );	// 9
    s_Metadata.append( QString( "\t%1" ).arg( s_DateTimeStart ) );			// 10
    s_Metadata.append( QString( "\t%1" ).arg( s_Latitude ) );           	// 11
    s_Metadata.append( QString( "\t%1" ).arg( s_Longitude ) );				// 12
    s_Metadata.append( QString( "\t%1" ).arg( s_Elevation ) );				// 13
    s_Metadata.append( QString( "\t%1" ).arg( s_AreaName ) );				// 14
    s_Metadata.append( QString( "\t%1" ).arg( s_DOI ) );					// 15
    s_Metadata.append( QString( "\t%1" ).arg( s_Citation ) );           	// 16
    s_Metadata.append( QString( "\t%1" ).arg( i_DatePos ) );                // 17
    s_Metadata.append( QString( "\t%1" ).arg( i_TimePos ) );                // 18
    s_Metadata.append( QString( "\t%1" ).arg( i_DateTimePos ) );            // 19
    s_Metadata.append( QString( "\t%1" ).arg( i_ElevationPos ) );           // 20
    s_Metadata.append( QString( "\t%1" ).arg( i_EventLabelPos ) );          // 21
    s_Metadata.append( QString( "\t%1" ).arg( i_CampaignLabelPos ) );       // 22
    s_Metadata.append( QString( "\t%1" ).arg( i_AreaNamePos ) );            // 23
    s_Metadata.append( QString( "\t%1" ).arg( i_GearNamePos ) );            // 24
    s_Metadata.append( QString( "\t%1" ).arg( i_LatitudePos ) );            // 25
    s_Metadata.append( QString( "\t%1" ).arg( i_LongitudePos ) );           // 26
    s_Metadata.append( QString( "\t%1" ).arg( i_NumOfDataPoints) );         // 27

    sl_MetadataList.append( s_Metadata );

    if ( sl_MetadataList.count() == 1 )
        err = _ERROR_;

    return( err );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2009-10-11

int MainWindow::containsParameter( const bool b_containsParameterBlock, const QStringList& sl_ParameterList, const QString& s_Parameter, const bool b_showShortName, const bool b_showMethod, const bool b_showComment )
{
    if ( b_containsParameterBlock == true )
    {
        // is a PANGAEA dataset

        int i_Flag = (int) b_showShortName * 100 + (int) b_showMethod * 10 + (int) b_showComment;

        for ( int i=0; i<sl_ParameterList.count(); ++i )
        {
            if ( sl_ParameterList.at( i ).section( _PSEPARATOR_, 1, 5 ) == s_Parameter.section( _PSEPARATOR_, 1, 5 ) )
                return( i );

            switch( i_Flag )
            {
                case 111: // b_showShortName == true && b_showMethod == true && b_showComment == true
                    if ( sl_ParameterList.at( i ).section( _PSEPARATOR_, 2, 5 ) == s_Parameter.section( _PSEPARATOR_, 2, 5 ) )
                        return( i );
                    break;

                case 100: // b_showShortName == true && b_showMethod == false && b_showComment == false
                    if ( ( sl_ParameterList.at( i ).section( _PSEPARATOR_, 2, 2 ) == s_Parameter.section( _PSEPARATOR_, 2, 2 ) )
                        && ( sl_ParameterList.at( i ).section( _PSEPARATOR_, 5, 5 ) == s_Parameter.section( _PSEPARATOR_, 5, 5 ) ) )
                        return( i );
                    break;

                case 110: // b_showShortName == true && b_showMethod == true && b_showComment == false
                    if ( ( sl_ParameterList.at( i ).section( _PSEPARATOR_, 2, 2 ) == s_Parameter.section( _PSEPARATOR_, 2, 2 ) )
                        && ( sl_ParameterList.at( i ).section( _PSEPARATOR_, 3, 3 ) == s_Parameter.section( _PSEPARATOR_, 3, 3 ) )
                        && ( sl_ParameterList.at( i ).section( _PSEPARATOR_, 5, 5 ) == s_Parameter.section( _PSEPARATOR_, 5, 5 ) ) )
                        return( i );
                    break;

                case 101: // b_showShortName == true && b_showMethod == false && b_showComment == true
                    if ( ( sl_ParameterList.at( i ).section( _PSEPARATOR_, 2, 2 ) == s_Parameter.section( _PSEPARATOR_, 2, 2 ) )
                        && ( sl_ParameterList.at( i ).section( _PSEPARATOR_, 4, 5 ) == s_Parameter.section( _PSEPARATOR_, 4, 5 ) ) )
                        return( i );
                    break;

                case 10: // b_showShortName == false && b_showMethod == true && b_showComment == false
                    if ( ( sl_ParameterList.at( i ).section( _PSEPARATOR_, 1, 1 ) == s_Parameter.section( _PSEPARATOR_, 1, 1 ) )
                        && ( sl_ParameterList.at( i ).section( _PSEPARATOR_, 3, 3 ) == s_Parameter.section( _PSEPARATOR_, 3, 3 ) )
                        && ( sl_ParameterList.at( i ).section( _PSEPARATOR_, 5, 5 ) == s_Parameter.section( _PSEPARATOR_, 5, 5 ) ) )
                        return( i );
                    break;

                case 1: // b_showShortName == false && b_showMethod == false && b_showComment == true
                    if ( ( sl_ParameterList.at( i ).section( _PSEPARATOR_, 1, 1 ) == s_Parameter.section( _PSEPARATOR_, 1, 1 ) )
                        && ( sl_ParameterList.at( i ).section( _PSEPARATOR_, 4, 5 ) == s_Parameter.section( _PSEPARATOR_, 4, 5 ) ) )
                        return( i );
                    break;

                case 11: // b_showShortName == false && b_showMethod == true && b_showComment == true
                    if ( ( sl_ParameterList.at( i ).section( _PSEPARATOR_, 1, 1 ) == s_Parameter.section( _PSEPARATOR_, 1, 1 ) )
                        && ( sl_ParameterList.at( i ).section( _PSEPARATOR_, 3, 5 ) == s_Parameter.section( _PSEPARATOR_, 3, 5 ) ) )
                        return( i );
                    break;

                default:  // b_showShortName == false && b_showMethod == false && b_showComment == false
                    if ( ( sl_ParameterList.at( i ).section( _PSEPARATOR_, 1, 1 ) == s_Parameter.section( _PSEPARATOR_, 1, 1 ) )
                        && ( sl_ParameterList.at( i ).section( _PSEPARATOR_, 5, 5 ) == s_Parameter.section( _PSEPARATOR_, 5, 5 ) ) )
                        return( i );
                    break;
            }
        }

        if ( isMetadataParameter( s_Parameter, tr( "Campaign" ) ) == true )
            return( -4 );

        if ( isMetadataParameter( s_Parameter, tr( "Event" ) ) == true )
            return( -5 );

        if ( isMetadataParameter( s_Parameter, tr( "Event label" ) ) == true )
            return( -5 );

        if ( isMetadataParameter( s_Parameter, tr( "Event label" ), tr( "Event" ) ) == true )
            return( -5 );

        if ( isMetadataParameter( s_Parameter, tr( "Device" ) ) == true )
            return( -6 );

        if ( isMetadataParameter( s_Parameter, tr( "Date/Time" ) ) == true )
            return( -9 );

        if ( isMetadataParameter( s_Parameter, tr( "Date/Time of event " ), tr( "Date/Time" ) ) == true )
            return( -9 );

        if ( isMetadataParameter( s_Parameter, tr( "Latitude" ) ) == true )
            return( -11 );

        if ( isMetadataParameter( s_Parameter, tr( "Latitude" ), tr( "Latitude of event" ) ) == true )
            return( -11 );

        if ( isMetadataParameter( s_Parameter, tr( "Longitude" ) ) == true )
            return( -12 );

        if ( isMetadataParameter( s_Parameter, tr( "Longitude" ), tr( "Longitude of event" ) ) == true )
            return( -12 );

        if ( isMetadataParameter( s_Parameter, tr( "Elevation" ) ) == true )
            return( -13 );

        if ( isMetadataParameter( s_Parameter, tr( "Elevation" ), tr( "Elevation of event" ) ) == true )
            return( -13 );

        if ( isMetadataParameter( s_Parameter, tr( "Area" ) ) == true )
            return( -14 );

        if ( isMetadataParameter( s_Parameter, tr( "DOI" ) ) == true )
            return( -15 );

        if ( isMetadataParameter( s_Parameter, tr( "Type" ) ) == true )
            return( -31 );
    }
    else
    {
        // is not a PANGAEA dataset

        for ( int i=0; i<sl_ParameterList.count(); ++i )
        {
            if ( s_Parameter.startsWith( sl_ParameterList.at( i ), Qt::CaseInsensitive ) == true )
                return( i );
        }

        if ( s_Parameter.contains( "station", Qt::CaseInsensitive ) == true )
            return( -5 );

        if ( s_Parameter.contains( "event", Qt::CaseInsensitive ) == true )
            return( -5 );

        if ( s_Parameter.contains( "date/time", Qt::CaseInsensitive ) == true )
            return( -9 );

        if ( s_Parameter.contains( "cruise", Qt::CaseInsensitive ) == true )
            return( -30 );

        if ( s_Parameter.contains( "campaign", Qt::CaseInsensitive ) == true )
            return( -30 );

        if ( s_Parameter.contains( "type", Qt::CaseInsensitive ) == true )
            return( -31 );
    }

    return( -999 );
}
