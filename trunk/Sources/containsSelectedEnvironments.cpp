/****************************************************************
**
** Class Pan2Applic, containsSelectedEnvironment
** 31.12.2002, Dr. Rainer Sieger
** last change: 28.02.2009
**
****************************************************************/

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

bool MainWindow::containsSelectedEnvironment( const int Mode, const int i_dataEnv, const int i_selectedEnv )
{
    int		i				= 0;
    int		a				= 1;
    int		n				= 0;

    QString s_SelectedEnv	= QString( "@%1@" ).arg( i_selectedEnv+_MAXENV, 0, 2 );
    QString s_DataEnv		= QString( "@%1@" ).arg( i_dataEnv+_MAXENV, 0, 2 );
    QString s_Env			= "";

    bool	b_Env			= false;

    int		i_EnvArray[_MAX_NUM_OF_ENV+1];

// **********************************************************************************************

    switch ( Mode )
    {
        case _SELECTPARAMETER :
            if ( s_SelectedEnv == s_DataEnv )
                b_Env = true;

            if ( s_SelectedEnv != s_DataEnv )
            {
                s_SelectedEnv.replace( "1" , ",+," );
                s_SelectedEnv.replace( "0" , ",-," );
                s_SelectedEnv.replace( ",," , "," );
                s_SelectedEnv.replace( "@," , "" );
                s_SelectedEnv.replace( ",@" , "" );
                s_SelectedEnv.replace( "-" , "0" );
                s_SelectedEnv.replace( "+" , "1" );

                s_DataEnv.replace( "1" , ",+," );
                s_DataEnv.replace( "0" , ",-," );
                s_DataEnv.replace( ",," , "," );
                s_DataEnv.replace( "@," , "" );
                s_DataEnv.replace( ",@" , "" );
                s_DataEnv.replace( "-" , "0" );
                s_DataEnv.replace( "+" , "1" );

                i = s_SelectedEnv.count( "," ) + 1;

                while ( ( i > 0 ) && ( b_Env == false ) )
                {
                    if ( s_SelectedEnv.section( ",", i, i ) == "1" )
                        if ( s_DataEnv.section( ",", i, i ) == s_SelectedEnv.section( ",", i, i ) )
                            b_Env = true;
                    i--;
                }
            }
            break;

        case _BUILDIMPORTFILE :
            memset( i_EnvArray, -1, ( _MAX_NUM_OF_ENV+1 )*sizeof( i_EnvArray[0] ) );

            s_Env = QString( "@%1@" ).arg( i_dataEnv, 0, 2 );

            s_Env.replace( "1" , ",+," );
            s_Env.replace( "0" , ",-," );
            s_Env.replace( ",," , "," );
            s_Env.replace( "@," , "" );
            s_Env.replace( ",@" , "" );
            s_Env.replace( "-" , "0" );
            s_Env.replace( "+" , "1" );

            n = s_Env.count( "," ) + 1;

            for ( i=n; i>=0; i-- )
            {
                if ( s_Env.section( ",", i, i ) == "1" )
                    i_EnvArray[n-i] = a;
                else
                    i_EnvArray[n-i] = 0;

                a *= 2;
            }

            while ( ( ++i<=_MAX_NUM_OF_ENV ) && ( b_Env == false ) )
            {
                if ( ( i_EnvArray[i] == i_selectedEnv ) || ( i_selectedEnv >= _EVENTIMPORT ) )
                    b_Env = true;
            }
            break;
    }

    return( b_Env );
}
