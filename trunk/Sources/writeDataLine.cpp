// ***********************************************************************************************
// *                                                                                             *
// * createTextFile.cpp - Create a text file                                                     *
// *                                                                                             *
// * Dr. Rainer Sieger - 2009-04-24                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include "Application.h"

/*! @brief Test ob Datenzeile ausgegeben werden soll. */

bool MainWindow::writeDataLine( const bool b_setGeocode, const bool b_setGearID, const int i_GeocodePos, const int i_GearIDPos, const QStringList& sl_Output, const int i_Env, const QDateTime dt_GEOCODE_min, const QDateTime dt_GEOCODE_max, const float f_GEOCODE_min, const float f_GEOCODE_max, const int i_GearID )
{
    float  f_GEOCODE        = 0.;

// ***********************************************************************************************
// Gear ID

    if ( ( b_setGearID == true ) && ( i_GearIDPos >= 0 ) )
    {
        if ( sl_Output.at( i_GearIDPos ).toInt() != i_GearID )
            return( false );
    }

// ***********************************************************************************************
// Geocode range

    if ( ( b_setGeocode == true ) && ( i_GeocodePos >= 0 ) )
    {
        QDateTime dt_GEOCODE( QDateTime::currentDateTime() );

        switch( i_Env )
        {
            case _DATETIME:
                dt_GEOCODE = convertDateTime( sl_Output.at( i_GeocodePos ) );

                if ( ( dt_GEOCODE_min <= dt_GEOCODE ) && ( dt_GEOCODE <= dt_GEOCODE_max ) )
                    return( true );
                else
                    return( false );
                break;

            default:
                f_GEOCODE = sl_Output.at( i_GeocodePos ).toFloat();

                if ( ( f_GEOCODE_min <= f_GEOCODE ) && ( f_GEOCODE <= f_GEOCODE_max ) )
                    return( true );
                else
                    return( false );
                break;
        }
    }

    return( true );
}
