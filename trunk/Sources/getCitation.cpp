// ***********************************************************************************************
// *                                                                                             *
// * getCitation.cpp                                                                             *
// *                                                                                             *
// * Dr. Rainer Sieger - 2009-04-07                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QString MainWindow::getCitation( const QString& InputStr )
{
    QString s_Citation = InputStr;

    s_Citation.replace( "Citation:\t", "" );

    if ( s_Citation.contains( ", doi:" ) == true )
        s_Citation = s_Citation.left( s_Citation.indexOf( ", doi:" ) );

    if ( s_Citation.contains( ", unpublished dataset #" ) == true )
        s_Citation = s_Citation.left( s_Citation.indexOf( ", unpublished dataset #" ) );

    if ( s_Citation.contains( ", dataset #" ) == true )
        s_Citation = s_Citation.left( s_Citation.indexOf( ", dataset #" ) );

    return( s_Citation );
}

