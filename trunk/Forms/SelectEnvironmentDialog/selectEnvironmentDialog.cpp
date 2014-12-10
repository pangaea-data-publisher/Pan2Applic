// ***********************************************************************************************
// *                                                                                             *
// * SelectEnvironmentDialog.cpp - Choose the environment                                        *
// *                                                                                             *
// * Dr. Rainer Sieger - 2009-05-14                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtWidgets>

#include "Application.h"
#include "selectEnvironmentDialog.h"

SelectEnvironmentDialog::SelectEnvironmentDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    connect(OK_pushButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(Cancel_pushButton, SIGNAL(clicked()), this, SLOT(reject()));
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

int MainWindow::doSelectEnvironmentDialog( const bool b_setGeocode, const int i_EnvArray[], const QStringList &sl_MetadataList )
{
    int n     = 0;
    int i_Env = 0;

    QString     s_EnvironmentName   = "";

    QStringList sl_EnvironmentName;

// ***********************************************************************************************************************

    n = getNumOfEnvironments( sl_MetadataList );

    if ( ( sl_MetadataList.count() == 2 ) && ( b_setGeocode == false ) )
        return( getEnvironment( i_EnvArray ) );

    if ( ( sl_MetadataList.count() > 2 ) && ( n == 1 ) )  // only one type of environment
        return( getEnvironment( i_EnvArray ) );

    n = getNumOfEnvironments( i_EnvArray );

    switch( n )
    {
    case 0:
        i_Env = _NOTGIVEN;
        break;

    case 1:
        i_Env = getEnvironment( i_EnvArray );
        break;

    default:
        if ( i_EnvArray[1] > 0 ) sl_EnvironmentName.append( tr( "not given" ) );
        if ( i_EnvArray[2] > 0 ) sl_EnvironmentName.append( tr( "Depth, water [m]" ) );
        if ( i_EnvArray[3] > 0 ) sl_EnvironmentName.append( tr( "Depth, sediment/rock [m]" ) );
        if ( i_EnvArray[4] > 0 ) sl_EnvironmentName.append( tr( "Depth, ice/snow [m]" ) );
        if ( i_EnvArray[5] > 0 ) sl_EnvironmentName.append( tr( "Distance [cm]" ) );
        if ( i_EnvArray[6] > 0 ) sl_EnvironmentName.append( tr( "Point distance from start [m]" ) );
        if ( i_EnvArray[7] > 0 ) sl_EnvironmentName.append( tr( "Altitude [m]" ) );
        if ( i_EnvArray[8] > 0 ) sl_EnvironmentName.append( tr( "Height above ground [m]" ) );
        if ( i_EnvArray[9] > 0 ) sl_EnvironmentName.append( tr( "Date/Time" ) );
        if ( i_EnvArray[10] > 0 ) sl_EnvironmentName.append( tr( "Age [ka BP]" ) );
        if ( i_EnvArray[11] > 0 ) sl_EnvironmentName.append( tr( "Ordinal number" ) );
        if ( i_EnvArray[12] > 0 ) sl_EnvironmentName.append( tr( "Elevation [m a.s.l.]" ) );
        if ( i_EnvArray[13] > 0 ) sl_EnvironmentName.append( tr( "Section, height [m]" ) );
        if ( i_EnvArray[14] > 0 ) sl_EnvironmentName.append( tr( "Event import" ) );

        for ( int i=n; i<=10; ++i )
            sl_EnvironmentName.append( tr( "xxx" ) );

        SelectEnvironmentDialog dialog( this );

        dialog.Geocode_01_radioButton->setText( sl_EnvironmentName.at( 0 ) );

        if ( sl_EnvironmentName.at( 1 ) != "xxx" )
            dialog.Geocode_02_radioButton->setText( sl_EnvironmentName.at( 1 ) );
        else
            dialog.Geocode_02_radioButton->hide();

        if ( sl_EnvironmentName.at( 2 ) != "xxx" )
            dialog.Geocode_03_radioButton->setText( sl_EnvironmentName.at( 2 ) );
        else
            dialog.Geocode_03_radioButton->hide();

        if ( sl_EnvironmentName.at( 3 ) != "xxx" )
            dialog.Geocode_04_radioButton->setText( sl_EnvironmentName.at( 3 ) );
        else
            dialog.Geocode_04_radioButton->hide();

        if ( sl_EnvironmentName.at( 4 ) != "xxx" )
            dialog.Geocode_05_radioButton->setText( sl_EnvironmentName.at( 4 ) );
        else
            dialog.Geocode_05_radioButton->hide();

        if ( sl_EnvironmentName.at( 5 ) != "xxx" )
            dialog.Geocode_06_radioButton->setText( sl_EnvironmentName.at( 5 ) );
        else
            dialog.Geocode_06_radioButton->hide();

        if ( sl_EnvironmentName.at( 6 ) != "xxx" )
            dialog.Geocode_07_radioButton->setText( sl_EnvironmentName.at( 6 ) );
        else
            dialog.Geocode_07_radioButton->hide();

        if ( sl_EnvironmentName.at( 7 ) != "xxx" )
            dialog.Geocode_08_radioButton->setText( sl_EnvironmentName.at( 7 ) );
        else
            dialog.Geocode_08_radioButton->hide();

        if ( sl_EnvironmentName.at( 8 ) != "xxx" )
            dialog.Geocode_09_radioButton->setText( sl_EnvironmentName.at( 8 ) );
        else
            dialog.Geocode_09_radioButton->hide();

        if ( sl_EnvironmentName.at( 9 ) != "xxx" )
            dialog.Geocode_10_radioButton->setText( sl_EnvironmentName.at( 9 ) );
        else
            dialog.Geocode_10_radioButton->hide();

        dialog.Geocode_01_radioButton->setChecked( true );

        dialog.OK_pushButton->setWhatsThis( "Close dialog" );
        dialog.Cancel_pushButton->setWhatsThis( "Cancel dialog" );

        dialog.move( posDialog );
        dialog.resize( dialog.sizeHint() );
        dialog.show();

// ******************************************************************************************************

        switch ( dialog.exec() )
        {
        case QDialog::Accepted:
            if ( dialog.Geocode_01_radioButton->isChecked() == true ) s_EnvironmentName = dialog.Geocode_01_radioButton->text();
            if ( dialog.Geocode_02_radioButton->isChecked() == true ) s_EnvironmentName = dialog.Geocode_02_radioButton->text();
            if ( dialog.Geocode_03_radioButton->isChecked() == true ) s_EnvironmentName = dialog.Geocode_03_radioButton->text();
            if ( dialog.Geocode_04_radioButton->isChecked() == true ) s_EnvironmentName = dialog.Geocode_04_radioButton->text();
            if ( dialog.Geocode_05_radioButton->isChecked() == true ) s_EnvironmentName = dialog.Geocode_05_radioButton->text();
            if ( dialog.Geocode_06_radioButton->isChecked() == true ) s_EnvironmentName = dialog.Geocode_06_radioButton->text();
            if ( dialog.Geocode_07_radioButton->isChecked() == true ) s_EnvironmentName = dialog.Geocode_07_radioButton->text();
            if ( dialog.Geocode_08_radioButton->isChecked() == true ) s_EnvironmentName = dialog.Geocode_08_radioButton->text();
            if ( dialog.Geocode_09_radioButton->isChecked() == true ) s_EnvironmentName = dialog.Geocode_09_radioButton->text();
            if ( dialog.Geocode_10_radioButton->isChecked() == true ) s_EnvironmentName = dialog.Geocode_10_radioButton->text();

            if ( s_EnvironmentName == "not given" ) i_Env = _NOTGIVEN;
            if ( s_EnvironmentName == "Depth, water [m]" ) i_Env = _WATER;
            if ( s_EnvironmentName == "Depth, sediment/rock [m]" ) i_Env = _SEDIMENT;
            if ( s_EnvironmentName == "Depth, ice [m]" ) i_Env = _ICE;
            if ( s_EnvironmentName == "Depth, snow [m]" ) i_Env = _ICE;
            if ( s_EnvironmentName == "Depth, ice/snow [m]" ) i_Env = _ICE;
            if ( s_EnvironmentName == "Distance [cm]" ) i_Env = _DISTANCE;
            if ( s_EnvironmentName == "Point distance from start [m]" ) i_Env = _POINTDISTANCE;
            if ( s_EnvironmentName == "Altitude [m]" ) i_Env = _AIRA;
            if ( s_EnvironmentName == "Height above ground [m]" ) i_Env = _AIRH;
            if ( s_EnvironmentName == "Date/Time" ) i_Env = _DATETIME;
            if ( s_EnvironmentName == "Age [ka BP]" ) i_Env = _AGE;
            if ( s_EnvironmentName == "Ordinal number" ) i_Env = _ORDINALNUMBER;
            if ( s_EnvironmentName == "Elevation [m a.s.l.]" ) i_Env = _ELEVATION;
            if ( s_EnvironmentName == "Section, height [m]" ) i_Env = _SECTION;
            if ( s_EnvironmentName == "Event import" ) i_Env = _EVENTIMPORT;
            break;

        case QDialog::Rejected:
            break;

        default:
            break;
        }

        break;

        posDialog = dialog.pos();
    }

// ******************************************************************************************************

    return( i_Env );
}
