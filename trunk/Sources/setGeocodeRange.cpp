// ***********************************************************************************************
// *                                                                                             *
// * setGeocodeRange.cpp - Calls setGeocodeRange dialogs                                         *
// *                                                                                             *
// * Dr. Rainer Sieger - 2009-11-23                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include "Application.h"

/*! @brief Ruft die SetGeocodeRange Dialoge auf. */

int MainWindow::setGeocodeRange( const int i_Env )
{
    int err = _ERROR_;

    switch( i_Env )
    {
        case _DATETIME:
            err = doSetGeocodeRangeDateTimeDialog();
            break;
        default:
            err = doSetGeocodeRangeFloatDialog();
            break;
    }

    return( err );
}
