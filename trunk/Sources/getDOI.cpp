// ***********************************************************************************************
// *                                                                                             *
// * getDOI.cpp                                                                                  *
// *                                                                                             *
// * Dr. Rainer Sieger - 2009-04-07                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QString MainWindow::getDOI( const QString& InputStr )
{
    QString s_DOI = InputStr;

    s_DOI.replace( "Citation:\t", "" );

    if ( s_DOI.contains( "doi:" ) == true )
        s_DOI = s_DOI.right( s_DOI.length() - s_DOI.indexOf( "doi:" ) );

    if ( s_DOI.contains( "Dataset #" ) == true )
    {
        s_DOI.replace( " (DOI registration in progress)", "" );
        s_DOI = "doi:10.1594/PANGAEA." + s_DOI.right( s_DOI.length() - s_DOI.indexOf( "#" ) - 1 ) + "@ DOI registration in progress";
    }

    if ( s_DOI.contains( "Unpublished dataset #" ) == true )
        s_DOI = "Dataset ID: " + s_DOI.right( s_DOI.length() - s_DOI.indexOf( "Unpublished dataset #" ) - 21 ) + "@ unpublished dataset";

    s_DOI.replace( ",", "" );               // -> if "In Supplement to:"
    s_DOI.replace( "@ unpub", ", unpub" );  // -> if "unpublished dataset"
    s_DOI.replace( "@ DOI", ", DOI" );      // -> if "DOI registration in progress"

    return ( s_DOI );
}
