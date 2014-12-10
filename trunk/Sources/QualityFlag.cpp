// ***********************************************************************************************
// *                                                                                             *
// * QualityFlag.cpp - sets the quality flag                                                     *
// *                                                                                             *
// * Dr. Rainer Sieger - 2009-10-22                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include "Application.h"

/*! @brief Setzt das Quality flag, wenn Eintrag mit Qualtiy Zeichen beginnt. */

QString MainWindow::setQualityFlag( const QStringList& sl_Output, const QVector<int> vi_posQV, const QString s_FieldDelimiter )
{
    QStringList sl_QV;

    QString     s_QV    = "";

    bool        b_OK    = true;

//-----------------------------------------------------------------------------------------------------------------------

    for ( int j=0; j<vi_posQV.count(); ++j )
    {
        if ( vi_posQV[j] >= 0 )
        {
            sl_Output.at( vi_posQV[j] ).toFloat( &b_OK );

            if ( b_OK == false )
            {
                s_QV = sl_Output.at( vi_posQV[j] ).left( 1 );

                if ( ( s_QV == "*" ) || ( s_QV == "/" ) || ( s_QV == "#" ) || ( s_QV == "?" ) || ( s_QV == "<" ) || ( s_QV == ">" ) )
                    sl_QV.append( s_QV );
                else
                    sl_QV.append( "" ); // cell includes text
            }
            else
                sl_QV.append( "" ); // cell is float
        }
    }

// **********************************************************************************************

    s_QV = sl_QV.join( s_FieldDelimiter );

    return( s_FieldDelimiter + s_QV );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2009-10-25

/*! @brief Loescht das Quality flag im Output-String. */

QString MainWindow::removeQualityFlag( const QStringList& sl_Output, const QString s_FieldDelimiter )
{
    QString s_Output = "@" + s_FieldDelimiter + sl_Output.join( s_FieldDelimiter );

    s_Output.replace( s_FieldDelimiter + tr( "*" ), s_FieldDelimiter );
    s_Output.replace( s_FieldDelimiter + tr( "/" ), s_FieldDelimiter );
    s_Output.replace( s_FieldDelimiter + tr( "#" ), s_FieldDelimiter );
    s_Output.replace( s_FieldDelimiter + tr( "?" ), s_FieldDelimiter );
    s_Output.replace( s_FieldDelimiter + tr( "<" ), s_FieldDelimiter );
    s_Output.replace( s_FieldDelimiter + tr( ">" ), s_FieldDelimiter );
    s_Output.replace( s_FieldDelimiter + tr( "ø" ), s_FieldDelimiter );

    return( s_Output.section( s_FieldDelimiter, 1 ) );
}
