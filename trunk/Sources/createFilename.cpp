/****************************************************************
**
** createFilename - creates a filename.
**
** In : QString - name of the data directory
** Out: QString - name of the filename

** 31.01.2004, Dr. Rainer Sieger - Alfred Wegener Institut Bremerhaven
**
****************************************************************/

#include <QtGui>

#include "Application.h"

QString MainWindow::createFilename( const QString& s_Path, const QString& s_baseName, const int i_NumOfFiles )
{
    QString s_Filename = "";

    QFileInfo pi( s_Path );

    if ( pi.isDir() == TRUE )
    {
        if ( i_NumOfFiles == 1 )
        {
            s_Filename = s_baseName;
        }
        else
        {
            s_Filename = pi.absolutePath();  // section( '/', -1, -1 );
        }
    }

    s_Filename.replace(" ", "_");

    if ( s_Filename.length() < 1 )
        s_Filename = "Data_imp";

    return ( s_Filename );
}
