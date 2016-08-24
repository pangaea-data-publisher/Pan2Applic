/* 2016-08-17                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2016-08-17

int MainWindow::extractExif( const QString &s_ExifTool, const QStringList &sl_FilenameList, const QString &s_FilenameOut, const int i_DateTimeFormat, const int i_UtcOffset )
{
    int         n                = 0;

    int         i_NumOfTurns     = 0;

    QString     s_Message        = "";

    QString     s_DateTimeFormat = "";

    QString     s_arg            = "";

    QStringList sl_Input;

    QProcess    process;

// **********************************************************************************************

    switch( i_DateTimeFormat )
    {
    case _BUILDDATE:
        s_DateTimeFormat = "yyyy-MM-dd";
        break;

    case _BUILDDATETIME:
        s_DateTimeFormat = "yyyy-MM-dd hh:mm";
        break;

    case _BUILDISODATETIME:
        s_DateTimeFormat = "yyyy-MM-ddThh:mm:ss";
        break;

    default:
        s_DateTimeFormat = "yyyy-MM-ddThh:mm";
        break;
    }

// **********************************************************************************************

    QFileInfo fiOut( s_FilenameOut );
    QFileInfo fiDone( fiOut.absolutePath() + "/" + "ExifTool_done.txt" );
    QFileInfo fiTemp( fiOut.absolutePath() + "/" + "ExifTool_out_temp.txt" );
    QFileInfo fiKML( fiOut.absolutePath() + "/" + fiOut.baseName() + ".kml" );

    #if defined(Q_OS_LINUX)
        QFileInfo fiScript( fiOut.absolutePath() + "/" + "extratExif.sh" );
        QFile fScript( fiScript.absoluteFilePath() );
    #endif

    #if defined(Q_OS_MAC)
        QFileInfo fiScript( fiOut.absolutePath() + "/" + "extratExif.sh" );
        QFile fScript( fiScript.absoluteFilePath() );
    #endif

    #if defined(Q_OS_WIN)
        QFileInfo fiScript( fiOut.absolutePath() + "/" + "extratExif.cmd" );
        QFile fScript( fiScript.absoluteFilePath() );
    #endif

// **********************************************************************************************

    if ( fiOut.exists() == true )
        QFile::remove( fiOut.absoluteFilePath() );

    if ( fiKML.exists() == true )
        QFile::remove( fiKML.absoluteFilePath() );

    if ( fiDone.exists() == true )
        QFile::remove( fiDone.absoluteFilePath() );

    if ( fiTemp.exists() == true )
        QFile::remove( fiTemp.absoluteFilePath() );

    if ( fiScript.exists() == true )
        QFile::remove( fiScript.absoluteFilePath() ) ;

// **********************************************************************************************

    if ( fScript.open( QIODevice::WriteOnly | QIODevice::Text) == false )
        return( -20 );

    QTextStream tScript( &fScript );

// **********************************************************************************************
// create script
// ExifTool -n -T -filename -gpsdatetime -gpslatitude -gpslongitude image.jpg >> out.txt

    #if defined(Q_OS_LINUX)
        tScript << "#!/bin/bash" << endl;
    #endif

    #if defined(Q_OS_MAC)
        tScript << "#!/bin/bash" << endl;
    #endif

    #if defined(Q_OS_WIN)
        ;
    #endif

    for ( int i=0; i<sl_FilenameList.count(); i++ )
    {
        QFileInfo fi( sl_FilenameList.at( i ) );

        if ( ( fi.suffix().toLower() == "jpg" ) || ( fi.suffix().toLower() == "tif" ) )
            tScript << "\"" << QDir::toNativeSeparators( s_ExifTool ) << "\" -n -T -filename -gpsdatetime -gpslatitude -gpslongitude \"" << QDir::toNativeSeparators( sl_FilenameList.at( i ) ) << "\" >> \"" << fiTemp.absoluteFilePath() << "\"" << endl;
    }

    tScript << "echo location extracted > " << "\""  << QDir::toNativeSeparators( fiDone.absoluteFilePath() ) << "\"" << endl;

    fScript.close();

// **********************************************************************************************
// run script

    #if defined(Q_OS_LINUX)
        wait( 500 );

        s_arg = "chmod u+x \"" + fiScript.absoluteFilePath() + "\"";
        process.startDetached( s_arg );

        wait( 500 );
    #endif

    #if defined(Q_OS_MAC)
        wait( 500 );

        s_arg = "chmod u+x \"" + fiScript.absoluteFilePath() + "\"";
        process.startDetached( s_arg );

        wait( 500 );
    #endif

    s_arg = "\"" + fiScript.absoluteFilePath() + "\"";

    if ( process.startDetached( s_arg ) == false )
    {
        s_Message = "Cannot start the script\n\n    " + QDir::toNativeSeparators( fiScript.fileName() ) + "\n\n Please start the script manually from your shell.";
        QMessageBox::warning( this, getApplicationName( true ), s_Message );
    }

// **********************************************************************************************

    while ( fiDone.exists() == false )
    {
        wait( 1000 );

        if ( ++i_NumOfTurns > 10 )
            break;
    }

// **********************************************************************************************
// create output file

    if ( ( n = readFile( fiTemp.absoluteFilePath(), sl_Input, _SYSTEM_ ) ) < 1 )
        return( _NODATAFOUND_ );

    QFile fout( fiOut.absoluteFilePath() );

    if ( fout.open( QIODevice::WriteOnly | QIODevice::Text) == false )
        return( -20 );

    QTextStream tout( &fout );

    tout << "Event label" << "\t" << "Date/Time (UTC)" << "\t" << "Date/Time (local)" << "\t" << "Latitude" << "\t" << "Longitude" << "\t" << "File name" << endl;

    for ( int i=0; i<n; i++ )
    {
        QString s_EventLabel = sl_Input.at( i ).section( "\t", 0, 0 ).section( ".", 0, 0 );
        QString s_Date       = sl_Input.at( i ).section( "\t", 1, 1 ).section( " ", 0, 0 ).replace( ":", "-" );
        QString s_Time       = sl_Input.at( i ).section( "\t", 1, 1 ).section( " ", 1, 1 ).section( "Z", 0, 0 ).section( ".", 0, 0 );
        QString s_DateTime   = s_Date + "T" + s_Time;
        double  d_Latitude   = sl_Input.at( i ).section( "\t", 2, 2 ).toDouble();
        double  d_Longitude  = sl_Input.at( i ).section( "\t", 3, 3 ).toDouble();

        QDateTime dtUtc   = QDateTime::fromString( s_DateTime, "yyyy-MM-ddThh:mm:ss" );
        QDateTime dtLocal = QDateTime::fromString( s_DateTime, "yyyy-MM-ddThh:mm:ss" ).addSecs( i_UtcOffset );

        tout << s_EventLabel << "\t";
        tout << dtUtc.toString( s_DateTimeFormat ) << "\t";
        tout << dtLocal.toString( s_DateTimeFormat ) << "\t";
        tout << QString( "%1" ).arg( d_Latitude, 0, 'f', 6 ) << "\t";
        tout << QString( "%1" ).arg( d_Longitude, 0, 'f', 6 ) << "\t";
        tout << sl_FilenameList.at( i ) << endl;
    }

    fout.close();

// **********************************************************************************************

    if ( fiDone.exists() == true )
        QFile::remove( fiDone.absoluteFilePath() );

    if ( fiTemp.exists() == true )
        QFile::remove( fiTemp.absoluteFilePath() );

    if ( fiScript.exists() == true )
        QFile::remove( fiScript.absoluteFilePath() ) ;

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2016-08-17

void MainWindow::doExtractExif()
{
    int		  err            = 0;
    int       stopProgress   = 0;

    QString   s_ExifTool     = "";

// **********************************************************************************************

    s_ExifTool = findExifTool();

    if ( s_ExifTool != "ExifTool not found" )
    {
        existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList );

        if ( doExifToolOptionsDialog() == QDialog::Accepted )
            extractExif( s_ExifTool, gsl_FilenameList, gs_FilenameExifOut, gi_DateTimeFormat, gi_UtcOffset );
        else
            err = _CHOOSEABORTED_;
    }
    else
    {
        err = _CHOOSEABORTED_;
    }

// **********************************************************************************************

    if ( ( err == _NOERROR_ ) && ( gb_CreateKmlFile == true ) )
    {
        gsl_FilenameList.clear();
        gsl_FilenameList.append( gs_FilenameExifOut );
        doCreateGoogleEarthImportFile();
    }

    if ( ( err == _NOERROR_ ) && ( gb_CreateKmlFile == false ) )
        QMessageBox::information( this, getApplicationName( true ), "Exif record extracted" );

// **********************************************************************************************

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Extracting Exif record was canceled" ), true );

    onError( err );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// check the availability of ExifTool

QString MainWindow::findExifTool()
{
#if defined(Q_OS_LINUX)
    QFileInfo fi_ExifTool( "/usr/local/bin/exiftool" );

    if ( fi_ExifTool.exists() == false )
    {
        QMessageBox::information( this, getApplicationName( true ), tr( "You have to install the\nprogram ExifTool (http://www.sno.phy.queensu.ca/~phil/ExifTool/)\nbefore using this function." ) );

        return( "ExifTool not found" );
    }
#endif

#if defined(Q_OS_MAC)
    QFileInfo fi_ExifTool( "/usr/local/bin/exiftool" );

    if ( fi_ExifTool.exists() == false )
    {
        QMessageBox::information( this, getApplicationName( true ), tr( "You have to install the\nprogram ExifTool (http://www.sno.phy.queensu.ca/~phil/ExifTool/)\nbefore using this function." ) );

        return( "ExifTool not found" );
    }
#endif

#if defined(Q_OS_WIN)
    QString s_ExifTool = getenv( "ProgramFiles(x86)" ) + "/exiftool/exiftool.exe";

    QFileInfo fi_ExifTool( s_ExifTool );

    if ( fi_ExifTool.exists() == false )
    {
        QMessageBox::information( this, getApplicationName( true ), tr( "You have to install the\nprogram ExifTool (http://www.sno.phy.queensu.ca/~phil/ExifTool/)\nbefore using this function." ) );

        return( "ExifTool not found" );
    }
#endif

    return( fi_ExifTool.absoluteFilePath() );
}
