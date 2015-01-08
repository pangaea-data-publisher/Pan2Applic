/****************************************************************
**
** createDir - creates a directory.
**
** In : QString - name of the data directory, name of the directory to be create
** Out: QString - name of the new directory

** 31.01.2004, Dr. Rainer Sieger - Alfred Wegener Institut Bremerhaven
**
****************************************************************/

#include "Application.h"

QString MainWindow::createDir( const QString& s_Path, const QString& s_newDirName, const bool b_createDirInside, const bool b_emptyDir )
{
    QString s_newDir		= s_Path;
    QString s_PersonalDir	= getDocumentDir();
    QString s_HomeDir		= QDir::homePath();

// **********************************************************************************************

    if ( s_Path.length() + s_newDirName.length() + 5 <= 255 )
    {
        if ( s_newDir == s_HomeDir )
            s_newDir.append( "/" );

        if ( s_newDir == s_PersonalDir )
            s_newDir.append( "/" );

        if ( s_newDir.endsWith( "Desktop" ) == true )
            s_newDir.append( "/" );

        if ( s_newDir.endsWith( "/" ) == false )
        {
            if ( b_createDirInside == false )
            {
                #if defined(Q_OS_LINUX)
                    s_newDir = s_newDir.left( s_newDir.lastIndexOf( "/" ) );
                    s_newDir.append( "/" );
                    s_newDir.append( s_newDirName );
                #endif

                #if defined(Q_OS_WIN)
                    s_newDir.append( " (" );
                    s_newDir.append( s_newDirName );
                    s_newDir.append( ")" );
                #endif

                #if defined(Q_OS_MAC)
                    s_newDir.append( " (" );
                    s_newDir.append( s_newDirName );
                    s_newDir.append( ")" );
                #endif
            }
            else
            {
                s_newDir.append( "/" );
                s_newDir.append( s_newDirName );
            }

            QDir().mkdir( s_newDir );
            s_newDir.append( "/" );
        }
        else
        {
            s_newDir.append( s_newDirName );

            QDir().mkdir( s_newDir );
            s_newDir.append( "/" );
        }

        if ( b_emptyDir == true )
            emptyDir( s_newDir );
    }

    return ( s_newDir );
}
