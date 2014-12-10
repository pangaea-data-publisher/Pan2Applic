// ***********************************************************************************************
// *                                                                                             *
// * getParameterList.cpp - get parameters                                                       *
// *                                                                                             *
// * Dr. Rainer Sieger - 2009-05-31                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtGui>

#include "Application.h"
#include "Globals.h"

/*! @brief ermittelt die Parameter. */

int MainWindow::getParameterList( QFile& fin, const int i_Environment, QStringList& sl_ParameterList )
{
    QString InputStr            = "";
    QString s_Environment       = "";

    int     i_NumOfParameters   = 0;

    bool    b_isParameter       = TRUE;

// ***********************************************************************************************

    QTextStream tin( &fin );

    InputStr = tin.readLine();

    if ( InputStr.startsWith( "/* DATA DESCRIPTION:" ) == TRUE ) // file starts with PANGAEA header
    {
        while ( ( InputStr.startsWith( "*/" ) == FALSE ) && ( tin.atEnd() == FALSE ) )
        {
            if ( InputStr.startsWith( "Parameter(s):" ) == TRUE )
            {
                s_Environment = QString( "%1" ).arg( i_Environment ) + PSEPARATOR;

                while ( b_isParameter == TRUE )
                {
                    if ( i_NumOfParameters < (int) MAX_NUM_OF_PARAMETERS )
                    {
                        sl_ParameterList.append( s_Environment + buildParameter( InputStr.section( "\t", 1, 1 ) ) );

                        ++i_NumOfParameters;
                    }

                    InputStr = tin.readLine();

                    if ( InputStr.startsWith( "\t" ) == FALSE )
                        b_isParameter = FALSE;
                }
            }

            InputStr = tin.readLine();
        }
    }
    else
    {
        return( 0 );
    }

    return( i_NumOfParameters );
}
