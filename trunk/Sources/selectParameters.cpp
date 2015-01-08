/****************************************************************
**
** Class Pan2Applic, selectParameters
** 31.12.2002, Dr. Rainer Sieger
** last change: 30.12.2005
**
****************************************************************/

#include <QtGui>

#include "Application.h"
#include "Globals.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

int MainWindow::selectParameters( QStringList sl_ParameterList, QStringList& sl_selectedParameters, QStringList& sl_selectedUnits, const bool b_showMethod, const bool b_showComment, const int i_selectedEnv, const int i_NumOfFiles )
{
	unsigned int	i					= 0;
	unsigned int	j					= 0;

	int				i_NumOfParameters	= sl_ParameterList.count();

    QStringList     sl_availableParameters;

// **********************************************************************************************

	if ( i_NumOfParameters <= 0 )
		return( -2 );

// **********************************************************************************************

	setStatusBar( tr( "Select parameters" ) );

// **********************************************************************************************

	sl_selectedParameters.clear();
    sl_availableParameters.clear();

// **********************************************************************************************

    for( i=0; i<(unsigned int) sl_ParameterList.count(); i++ )
	{
        int i_dataEnv = sl_ParameterList.at( i ).section( "@", 0, 0 ).section( PSEPARATOR, 0, 0 ).toInt();

		if ( containsSelectedEnvironment( SELECTPARAMETER, i_dataEnv, i_selectedEnv ) == TRUE )
            sl_availableParameters += buildParameter( sl_ParameterList.at( i ), SELECTPARAMETER, b_showMethod, b_showComment );
	}

    i_NumOfParameters = removeDoubleParameters( sl_availableParameters, i_NumOfFiles, MAX_NUM_OF_UNSORTED_PARAMETERS );

    if ( ( sl_availableParameters.count() == 1 ) || ( b_getAllParameter == TRUE ) )
	{
        sl_selectedParameters.append( sl_availableParameters.at( 0 ) );
        sl_selectedUnits.append( sl_availableParameters.at( 0 ).section( " [", 2, 2 ).section( "]", 0, 0 ) + PSEPARATOR + sl_availableParameters.at( 0 ).section( " [", 2, 2 ).section( "]", 0, 0 ) + PSEPARATOR + "1.0" );
	}
	else
	{
        while ( ( sl_selectedParameters.count() <= 0 ) || ( sl_selectedParameters.count() > MAX_NUM_OF_PARAMETERS ) )
		{
			SelectParameterDialog Dialog;
			if ( Dialog.exec() == QDialog::Accepted )
			{
				qApp->processEvents();

                for( i=0; i<sl_selectedParameters.count(); i++ )
				{
                    for( j=0; j<sl_availableParameters.count(); j++ )
					{
                        if ( sl_availableParameters[j] == sl_selectedParameters[i] )
						{
							sl_selectedParameters.append( gsl_selectedParameters[i] );
							sl_selectedUnits.append( gsl_selectedParameters[i].section( " [", 2, 2 ).section( "]", 0, 0 ) + PSEPARATOR + gsl_selectedParameters[i].section( " [", 2, 2 ).section( "]", 0, 0 ) + PSEPARATOR + "1.0" );
							break;
						}
					}
				}
			}
			else
			{
				setStatusBar( tr( "Select parameters was cancelled" ), 2000 );
				return( -2 );
			}
		}
	}

	setStatusBar( tr( "" ) );

	return( sl_selectedParameters.count() );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

bool MainWindow::containsSelectedEnvironment( const int Mode, const int i_dataEnv, const int i_selectedEnv )
{
    int		i				= 0;
    int		a				= 1;
    int		n				= 0;

    QString s_SelectedEnv	= QString( "@%1@" ).arg( i_selectedEnv+MAXENV, 0, 2 );
    QString s_DataEnv		= QString( "@%1@" ).arg( i_dataEnv+MAXENV, 0, 2 );
    QString s_Env			= "";

    bool	b_Env			= FALSE;

    int		i_EnvArray[MAX_NUM_OF_ENV+1];

// **********************************************************************************************

    switch ( Mode )
    {
        case SELECTPARAMETER :
            if ( s_SelectedEnv == s_DataEnv )
                b_Env = TRUE;

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

                i = s_SelectedEnv.contains( "," );

                while ( ( i > 0 ) && ( b_Env == FALSE ) )
                {
                    if ( s_SelectedEnv.section( ",", i, i ) == "1" )
                        if ( s_DataEnv.section( ",", i, i ) == s_SelectedEnv.section( ",", i, i ) )
                            b_Env = TRUE;
                    i--;
                }
            }
            break;

        case BUILDIMPORTFILE :
            memset( i_EnvArray, -1, ( MAX_NUM_OF_ENV+1 )*sizeof( i_EnvArray[0] ) );

            s_Env = QString( "@%1@" ).arg( i_dataEnv, 0, 2 );

            s_Env.replace( "1" , ",+," );
            s_Env.replace( "0" , ",-," );
            s_Env.replace( ",," , "," );
            s_Env.replace( "@," , "" );
            s_Env.replace( ",@" , "" );
            s_Env.replace( "-" , "0" );
            s_Env.replace( "+" , "1" );

            n = s_Env.contains( "," );

            for ( i=n; i>=0; i-- )
            {
                if ( s_Env.section( ",", i, i ) == "1" )
                    i_EnvArray[n-i] = a;
                else
                    i_EnvArray[n-i] = 0;

                a *= 2;
            }

            while ( ( ++i<=MAX_NUM_OF_ENV ) && ( b_Env == FALSE ) )
            {
                if ( ( i_EnvArray[i] == i_selectedEnv ) || ( i_selectedEnv >= EVENTIMPORT ) )
                    b_Env = TRUE;
            }
            break;
    }

    return( b_Env );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

int MainWindow::removeDoubleParameters( QStringList& sl_ParameterList, const int i_NumOfFiles, const unsigned int i_max_Num_Of_Unsorted_Parameters )
{
    unsigned int	i = 0;

    QStringList		sl_tempParameterList;

// **********************************************************************************************

    if ( sl_ParameterList.count() < i_max_Num_Of_Unsorted_Parameters )
        return( 0 );

// **********************************************************************************************

    if ( sl_ParameterList.count() < 1 )
        return( -1 );

// **********************************************************************************************

    if ( i_NumOfFiles > 1 )
        sl_ParameterList.sort();

// **********************************************************************************************

    if ( sl_ParameterList[0].section( "@", 1, 1 ) != "Geocode" )
        sl_tempParameterList = sl_ParameterList[0].section( "@", 0, 0 );

    for( i=1; i<sl_ParameterList.count(); i++ )
        if ( ( sl_ParameterList[i].section( "@", 0, 0 ) != sl_ParameterList[i-1].section( "@", 0, 0 ) ) && ( sl_ParameterList[i].section( "@", 1, 1 ) != "Geocode" ) )
            sl_tempParameterList.append( sl_ParameterList[i].section( "@", 0, 0 ) );

    sl_ParameterList.clear();

    if ( sl_tempParameterList.count() > 0 )
        sl_ParameterList = sl_tempParameterList;

    return( sl_ParameterList.count() );
}

