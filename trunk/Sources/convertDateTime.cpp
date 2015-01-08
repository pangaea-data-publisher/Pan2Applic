// ***********************************************************************************************
// *                                                                                             *
// * convertDateTime.cpp                                                                         *
// *                                                                                             *
// * Dr. Rainer Sieger - 2009-03-24                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include "Application.h"

// **********************************************************************************************

QDateTime MainWindow::convertDateTime( const QString& s_DateTimeIn )
{
    QString s_DateTimeOut = s_DateTimeIn;

    if ( s_DateTimeOut.length() <= 10 ) // Date without time
        s_DateTimeOut.append( tr( "T00:00:00" ) );

    while ( s_DateTimeOut.count( ":" ) < 2 )
        s_DateTimeOut.append( ":00" );

    QDateTime dt( QDateTime::fromString( s_DateTimeOut, Qt::ISODate ) );
    return( dt );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

int MainWindow::convertDateTime( const int i_DateFormat, QString& s_DateTime, QString& s_Date, QString& s_Time )
{
    int i_day	= 1;
    int i_month	= 1;
    int i_year	= 1970;

// **********************************************************************************************

    if ( i_DateFormat < 0 )
    {
        s_DateTime = s_Date + "T" + s_Time;

        s_DateTime.replace( "00:00:00", "00:00" );

        if ( i_DateFormat == _BUILDDATE )
            s_DateTime = s_DateTime.section( "T", 0, 0 );

        if ( s_DateTime.endsWith( "T" ) )
            s_DateTime.replace( "T", "" );

        if ( i_DateFormat == _BUILDDATETIME )
            s_DateTime.replace( "T", " " );

        return( 0 );
    }

// **********************************************************************************************

    if ( s_DateTime.isEmpty() == true )
    {
        QDateTime DateTime = QDateTime::currentDateTime();
        s_Date	= DateTime.toString( "yyyy-MM-dd" );
        s_Time	= DateTime.toString( "hh:mm" );

        return( 0 );
    }

// **********************************************************************************************

    s_Time	= "12:00";

// **********************************************************************************************

    if ( s_DateTime.count( "-" ) == 1 ) // possible ISO  yyyy-MM
    {
        s_Date = s_DateTime;

        i_year	= s_Date.section( "-", 0, 0 ).toInt();
        i_month	= s_Date.section( "-", 1, 1 ).toInt();
        i_day	= 15;
    }

// **********************************************************************************************

    if ( s_DateTime.count( "-" ) == 2 ) // possible ISO  yyyy-MM-ddTHH:mm
    {
        s_Date = s_DateTime;

        if ( s_DateTime.count( "T" ) > 0 )
        {
            s_Date = s_DateTime.section("T", 0, 0);
            s_Time = s_DateTime.section("T", 1, 1);
        }

        if ( s_DateTime.count( " " ) > 0 )
        {
            s_Date = s_DateTime.section(" ", 0, 0);
            s_Time = s_DateTime.section(" ", 1, 1);
        }

        if ( s_Time.count( ":" ) == 0 )
            s_Time.append( ":00" );

        i_year	= s_Date.section( "-", 0, 0 ).toInt();
        i_month	= s_Date.section( "-", 1, 1 ).toInt();
        i_day	= s_Date.section( "-", 2, 2 ).toInt();
    }

// **********************************************************************************************

    if ( s_DateTime.count( "/" ) == 2 ) // possible ODV format
    {
        if ( s_DateTime.count( " " ) > 0 )
        {
            s_Date = s_DateTime.section( " ", 0, 0 );
            s_Time = s_DateTime.section( " ", 1, 1 );
        }
        else
        {
            s_Date = s_DateTime;
            s_Time = "00:00";
        }

        i_month	= s_Date.section( "/", 0, 0 ).toInt();
        i_day	= s_Date.section( "/", 1, 1 ).toInt();
        i_year	= s_Date.section( "/", 2, 2 ).toInt();
    }

// **********************************************************************************************

    if ( s_DateTime.count( " " ) >= 2 ) // possible long format
    {
        s_DateTime.replace( " Jan ", ".01." );
        s_DateTime.replace( " Feb ", ".02." );
        s_DateTime.replace( " Mar ", ".03." );
        s_DateTime.replace( " Apr ", ".04." );
        s_DateTime.replace( " May ", ".05." );
        s_DateTime.replace( " Jun ", ".06." );
        s_DateTime.replace( " Jul ", ".07." );
        s_DateTime.replace( " Aug ", ".08." );
        s_DateTime.replace( " Sep ", ".09." );
        s_DateTime.replace( " Oct ", ".10." );
        s_DateTime.replace( " Nov ", ".11." );
        s_DateTime.replace( " Dec ", ".12." );
    }

// **********************************************************************************************

    if ( s_DateTime.count( "." ) == 2 ) // possible german format
    {
        s_DateTime.replace( "T", " " );

        if ( s_DateTime.count( " " ) > 0 )
        {
            s_Date = s_DateTime.section( " ", 0, 0 );
            s_Time = s_DateTime.section( " ", 1, 1 );

            if ( s_Time.isEmpty() == true )
                s_Time = "00:00";
            else
                s_Time = s_Time.left(5);
        }
        else
        {
            s_Date = s_DateTime;
            s_Time = "00:00";
        }

        i_day	= s_Date.section( ".", 0, 0 ).toInt();
        i_month	= s_Date.section( ".", 1, 1 ).toInt();
        i_year	= s_Date.section( ".", 2, 2 ).toInt();
    }

// **********************************************************************************************

    if ( i_day < 1 )
        return( -30 );

    if ( ( i_month < 1 ) || ( i_month > 12 ) )
        return( -30 );

    if ( i_year < 1 )
        return( -30 );

    switch ( i_month )
    {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            if ( i_day > 31 )
                return( -30 );
            break;

        case 2:
            if ( i_day > 29 )
                return( -30 );
            break;

        case 4:
        case 6:
        case 9:
        case 11:
            if ( i_day > 30 )
                return( -30 );
        break;

        default:
            return( -30 );
            break;
    }

// **********************************************************************************************

    s_Date.sprintf( "%d-%02d-%02d", i_year, i_month, i_day );

    return( 0 );
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

QString MainWindow::buildDateTimeStr( const QString& s_Data, const int i_pos, const int i_DateTimeFormat )
{
    QString s_DateTimeOut = "";

    switch( i_DateTimeFormat )
    {
        case _BUILDDATE:
            s_DateTimeOut = s_Data.section( "\t", i_pos, i_pos ).section( "T", 0, 0 );
            break;

        case _BUILDDATETIME:
            s_DateTimeOut = s_Data.section( "\t", i_pos, i_pos ).section( "T", 0, 0 ) + " " + s_Data.section( "\t", i_pos, i_pos ).section( "T", 1, 1 );
            break;

        default:
            s_DateTimeOut = s_Data.section( "\t", i_pos, i_pos );
            s_DateTimeOut.replace( "xxxTxxx", "" ); // Date/Time of Event is not given
            break;
    }

    return( s_DateTimeOut );
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

QString MainWindow::buildDateTimeStr( const QString& s_DateTimeIn, const int i_DateTimeFormat )
{
    QString s_DateTimeOut = s_DateTimeIn;

    if ( s_DateTimeOut.isEmpty() == false )
    {
        switch( i_DateTimeFormat )
        {
            case _BUILDDATE:
                s_DateTimeOut = s_DateTimeIn.section( "T", 0, 0 );
                break;

            case _BUILDDATETIME:
                s_DateTimeOut = s_DateTimeIn.section( "T", 0, 0 );
                if ( s_DateTimeIn.section( "T", 1, 1 ).isEmpty() == false )
                    s_DateTimeOut.append( tr( " " ) + s_DateTimeIn.section( "T", 1, 1 ) );
                break;

            default:
                if ( s_DateTimeOut.length() <= 10 ) // Date without time
                    s_DateTimeOut.append( tr( "T00:00:00" ) );
                while ( s_DateTimeOut.count( ":" ) < 2 )
                    s_DateTimeOut.append( tr( ":00" ) );
                break;
        }
    }

    return( s_DateTimeOut );
}

