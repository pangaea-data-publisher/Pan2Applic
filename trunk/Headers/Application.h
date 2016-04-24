/* Application.h              */
/* 2013-05-15                 */
/* Dr. Rainer Sieger          */

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>

#include <QTextEdit>
#include <QTextCodec>
#include <QTextStream>
#include <QStringListModel>
#include <QListView>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QCloseEvent>
#include <QProcess>
#include <QTimer>
#include <QMimeData>
#include <QDesktopServices>
#include <QMessageBox>
#include <QProgressBar>
#include <QProgressDialog>
#include <QStatusBar>
#include <QSettings>
#include <qmath.h>

#include "Globals.h"
#include "Webfile.h"
#include "shapefil.h"

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

class QAction;
class QLabel;
class QProgressDialog;
class QProgressBar;
class QTimer;
class QDateTime;
class QUrl;
class QTextEdit;
class QStringListModel;
class QListView;
class QTableWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Q_CLASSINFO( "author", "Rainer Sieger" )
    Q_CLASSINFO( "url", "https://pangaea.de" )

struct structURL
{
    int position;				//!< Position der URL
    QString TitleURL;			//!< Kurzname der URL
};

struct structDataFormat
{
    int type;                   //!< Type des Parameters (_ISDOUBLE, _ISSTRING, _ISDATE, _ISTIME, _ISDATETIME)
    int width;                  //!< Ausgabeformat des Parameters, Breite
    int precision;              //!< Ausgabeformat des Parameters, Anzahl der Nachkommastellen
};

public:
    explicit MainWindow( QWidget *parent = 0 );
    ~MainWindow();

    int			gi_ActionNumber;                 //!< Die ActionNumber wird bei jedem angewandenten Tool um 1 erhoeht.
    int         gi_NumOfProgramStarts;           //!< Anzahl der Programmstarts
    int         gi_EOL;                          //!< End-of-Line Zeichen fuer die Outputdateien
    int         gi_Extension;                    //! Dateierweiterung fuer Ausgabedateien
    int         gi_CodecInput;                   //!< Encoding der Inputdateien
    int         gi_CodecOutput;                  //!< Encoding der Outputdateien
    int         gi_CodecDownload;                //!< Encoding der Downloaddateien

    bool        gb_showProgressBar;              //!< Soll der ProgressBar im Fenster unten rechts angezeigt werden?

    QStringList gsl_FilenameList;                //!< Liste aller InputDateinamen incl. Pfad.

    QString     gs_Version;                      //!< Aktuelle Version des Programms.
    QString     gs_Path;                         //!< Der zuletzt verwendete Pfad.
    QString     gs_FilenameFormat;               //!< Bestimmt in die Form des automatisch erzeugten neuen Dateinamens. \%a = aktuelle Actionnumber, \%N = Dateiname.
    QString     gs_MissingValue;                 //!< Missing value fuer den Format Converter

    QString     gs_FilenameGoogleEarthProgram;   //!< Pfad und Name des Google Earth Programmes.
    QString     gs_FilenameGoogleEarth;          //!< Pfad und Name der KML-Datei
    QString     gs_FilenameOceanDataViewProgram; //!< Pfad und Name des Ocean Data View Programmes.
    QString     gs_FilenameOceanDataView;        //!< Pfad und Name der Ocean Data View Importdatei.
    QString     gs_FilenameShapefile;            //!< Pfad und Name der Shapefile-Datei.
    QString     gs_FilenameText;                 //!< Pfad und Name der Outputtextdatei.

    bool        gb_CuratorMode;                  //!< Curator Modus schaltet einen Menüeintrag frei.

    bool        gb_startGoogleEarth;             //!< true wenn Google Earth automatisch gestart werden soll.
    bool        gb_startOceanDataView;           //!< true wenn ODV automatisch gestart werden soll.

    bool        gb_displayEventLabel;
    bool        gb_displayDescription;

    int         gi_DateTimeFormat;                  //!< Date/Time Format.

    bool        gb_addLineNo;                       //!< fuege Line number an Event label an.
    bool        gb_usePANGAEAQualityFlags;          //!< Wenn true werden die PANGAEA Quality flags ausgegeben.
    int         gi_DeviceTypeOceanDataView;         //!< Device type Auswahl

    int         gi_GearID;                          //!< Gear ID

    float       gf_GEOCODE_min;                     //!< Geocode float minimum
    float       gf_GEOCODE_max;                     //!< Geocode float maximum
    QString     gs_GEOCODE_min;                     //!< Geocode date/time minimum
    QString     gs_GEOCODE_max;                     //!< Geocode date/time maximum

    int         gi_IconSize;                        //!< Groesse der Marker bei KML
    int         gi_IconColor;                       //!< Farbe der Marker bei KML
    int         gi_TracklineWidth;                  //!< Breite der Trackline bei KML
    int         gi_TracklineColor;                  //!< Farbe der Trackline bei KML

    int         gi_FieldDelimiter;                  //!< Felddelimiter fuer den Format Converter
    int         gi_FieldWidth;                      //!< Feldbreite fuer den Format Converter
    int         gi_FieldAlignment;                  //!< Textausrichtung fuer den Format Converter

    bool        gb_createDir;                       //!< Wo soll das Collection Verzeichnis erstellt werden?
    bool        gb_setGeocode;                      //!< Wenn true wird der Geocode Range abgefragt.
    bool        gb_setGearID;                       //!< Wenn true wird die GearID abgefragt.

    bool        gb_sortParameterList;               //!< Im Parameter-Auswahldialog werden die Parameter sortiert.
    bool        gb_showShortName;                   //!< Im Parameter-Auswahldialog wird der Short name der Parameter angezeigt.
    bool        gb_showMethod;                      //!< Im Parameter-Auswahldialog wird die Method mit angezeigt.
    bool        gb_showComment;                     //!< Im Parameter-Auswahldialog wird der Parameter-Comment mit angezeigt.

    QString     gs_IDListFile;                      //!< PanGet, Name der Datei, die die ID-Liste enthaelt
    QString     gs_DownloadDirectory;               //!< PanGet, Download-Verzeichnis

// ***************************************************************************************************************************

    int         setFilename( const int Mode, const bool CuratorMode, const int NumOfFiles, const QString& FilenameIn, QString& FilenameOut );

    int         analyzeDataset( const QString& FilenameIn, const int Codec, const bool showShortName, const bool showMethod, const bool showComment, QStringList& MetadataList, QStringList& ParameterList, int EnvArray[] );

    void        downloadDatasets( const QString &IDListFile, const QString &DownloadDirectory, const bool DownloadData, const bool DownloadCitation, const bool DownloadMetadata, const int CodecDownload, const int EOL, const int Extension );

    int         createGoogleEarthImportFile( QStringList& MetadataList, const QString& FilenameOut, const int Codec, const bool displayEventLabel, const bool displayDescription, const int IconSize, const int IconColor, const int TracklineWidth, const int TracklineColor, const int NumOfFiles );
    int         createOceanDataViewFile( QStringList& MetadataList, QStringList& ParameterList, const QString& FilenameOut, const int Codec, const int Env, const int GearTypeOceanDataView, const bool showShortName, const bool showMethod, const bool showComment, const bool usePANGAEAQualityFlags, const bool setGeocode, const bool addLineNo, const float GEOCODE_min, const float GEOCODE_max, const QString& s_GEOCODE_min, const QString& s_GEOCODE_max, const bool setGearID, const int GearID, const int NumOfFiles );
    int         createShapefileMetadata( QStringList& MetadataList, const QString& FilenameOut, const int Codec, const int NumOfFiles );
    int         createShapefileData( QStringList& MetadataList, QStringList& ParameterList, const structDataFormat DF[], const QString& FilenameOut, const int Codec, const int Env, const bool setGeocode, const float GEOCODE_min, const float GEOCODE_max, const QString& s_GEOCODE_min, const QString& s_GEOCODE_max, const bool setGearID, const int GearID, const int NumOfFiles );
    int         createTextFile( QStringList& MetadataList, QStringList& ParameterList, const QString& FilenameOut, const int CodecInput, const int CodecOutput, const int EOL, const int OutputFormat, const int Env, const int DateTimeFormat, const bool showShortName, const bool showMethod, const bool showComment, const QString& MissingValue, const int FieldAlignment, const int FieldWidth, const int FieldDelimiter, const bool gb_setGeocode, const float f_GEOCODE_min, const float f_GEOCODE_max, const QString& s_GEOCODE_min, const QString& s_GEOCODE_max, const bool setGearID, const int GearID, const int NumOfFiles );

    bool        writeDataLine( const bool setGeocode, const bool setGearID, const int GeocodePos, const int GearIDPos, const QStringList& GEOCODE, const int Env, const QDateTime dt_GEOCODE_min, const QDateTime dt_GEOCODE_max, const float f_GEOCODE_min, const float f_GEOCODE_max, const int GearID );

    int         convertDateTime( const int DateTimeFormat, QString& DateTime, QString& Date, QString& Time );
    QDateTime   convertDateTime( const QString& s_DateTime );

    QString     setQualityFlag( const QStringList& QutputStr, const QVector<int> posQV, const QString FieldDelimiter );
    QString     removeQualityFlag( const QStringList& Output, const QString FieldDelimiter );

    double      calculateDistance( const double Latitude1, const double Longitude1, const double Latitude2, const double Longitude2 );

    int         getNumOfEnvironments( const int EnvArray[] );
    int         getNumOfEnvironments( const QStringList& MetadataList );
    int         getEnvironment( const int EnvArray[] );
    int         getEnvironment( const QString& Parameter, int EnvArray[] );
    QString     getEnvironmentName( const int Env );
    QString     getEnvironmentShortName( const int Env );
    bool        isGeocode( const QString Parameter );

    int         getGeocodePosition( const QStringList& ParameterList, const int Env );
    int         getGearIDPosition( const QStringList& ParameterList );

    int         getFirstPos( const QStringList& ParameterList, const QString& Parameter );
    int         getParameterType( const QString& Parameter );
    void        getSpecialPos( const QStringList& ParameterList, QVector<int> &SpecialPos );

    int         setGeocodeRange( const int Env );
    int         setGearIDDialog();

    int         writeProjectionFile( const QString& Filename );
    int         writeDBFHeader( DBFHandle hDBF );
    int         writeDBFHeader( DBFHandle hDBF, const QStringList& ParameterList, const structDataFormat DF[] );
    int         writeShapfileDataEntry( DBFHandle hDBF, SHPHandle hSHP, const QStringList& Output, const structDataFormat DF[], const int Latitude, const int Longitude, const QString& DOI, const int iRecord );
    int         writeShapfileMetadataEntry( DBFHandle hDBF, SHPHandle hSHP, const QString& Metadata, const int iRecord );
    int         writeShapfileMetadataEntry( DBFHandle hDBF, SHPHandle hSHP, const int Codec, const QString& Metadata, const int iRecord, const int NumOfFiles );
    void        setDefaultFormats( const QStringList& ParameterList, structDataFormat DF[] );

    int         openKMLFile( QFile& fkml );
    int         closeKMLFile( QFile& fkml );
    int         writeKMLEntry( QFile& fkml, const QStringList& MetadataList, const bool displayEventLabel, const bool displayDescription, const float IconSize, const int IconColor, structURL URL[], const int i );
    int         writeKMLTrack( QFile& fkml, const QStringList& MetadataList, const int TracklineWidth, const int TracklineColor, const int Start, const int End );
    int         openKMLFolder( QFile& fkml, const QString& Campaign );
    int         closeKMLFolder( QFile& fkml, const int n=2 );

    int         containsParameter( const bool containsParameterBlock, const QStringList& ParameterList, const QString& Parameter, const bool showShortName, const bool showMethod, const bool showComment );

    int         buildHeader( QStringList& Header, QStringList& ParameterList, const int Env, const int OutputFormat, const bool showShortName, const bool showMethod, const bool showComment );
    QString     buildHeaderItem( const int OutputFormat, const QString& Parameter, const bool showShortName, const bool showMethod, const bool showComment );
    int         buildODVHeader( QStringList& Header, QStringList& ParameterList, QVector<int> &posQV, const int Env, const bool showShortName, const bool showMethod, const bool showComment, const bool usePANGAEAQualityFlags );
    QString     buildODVHeaderItem( const QString& Parameter, const bool showShortName, const bool showMethod, const bool showComment, const bool containsEventLabel );

    QString     buildParameter( const QString& InputStr, const bool showShortName, const bool showMethod, const bool showComment );
    QString     buildParameter( const QString& InputStr, const QStringList& ParameterList, const bool showShortName, const bool showMethod, const bool showComment );
    QString     buildParameter( const QString& InputStr );
    QStringList buildParameterList( const QString& s_Environment, const QString& s_Header );

    bool        isMetadataParameter( const QString& Parameter, const QString& ParameterName );
    bool        isMetadataParameter( const QString& Parameter, const QString& ParameterName, const QString& shortName );

    QStringList buildODVOutputStr( const int LineNo, const bool addLineNo, const QString& InputStr, const QString& Metadata, const QVector<int> pos, const QVector<int> posQV, const QVector<int> SpecialPos, const bool usePANGAEAQualityFlags, const bool containsDOI, const QString& DeviceType, const QString& FieldDelimiter );
    QString     buildDateTimeStr( const QString& Metadata, const int pos, const int DateTimeFormat );
    QString     buildDateTimeStr( const QString& DateTime, const int DateTimeFormat );

    QString     setDeviceType( const int Type );
    QString     getDOI( const QString& InputStr );
    QString     getCitation( const QString& InputStr );

    int         getEventMetadata( const QString& InputStr, QString& DateTime, QString& Elevation, QString& EventLabel, QString& CampaignLabel, QString& AreaName, QString& Gear, QString& Latitude, QString& Longitude, QStringList& ParameterList );
    int         getDataMetadata( const int Codec, const int OutputFormat, const QString& MetadataString, QStringList& DataMetadataList, structURL URL[], const int NumOfFiles );
    int         getDataMetadataPosition( const bool containsPANGAEAHeader, const QString& InputStr, int& DatePos, int& TimePos, int& i_DateTimePos, int& ElevationPos, int& EventLabelPos, int& CampaignLabelPos, int& AreaNamePos, int& GearNamePos, int& LatitudePos, int& LongitudePos, QStringList& ParameterList );
    bool        getDataMetadataPosition( const QString& MetadataString, int& DatePos, int& TimePos, int& DateTimePos, int& ElevationPos, int& EventLabelPos, int& CampaignLabelPos, int& AreaNamePos, int& GearNamePos, int& LatitudePos, int& LongitudePos );
    bool        addToMetadataList( const QString& Metadata, QString& LatLon, QStringList& MetadataList );
    bool        parseData( const int line, const QString& InputStr, const QString& MetadataString, const int AreaNamePos, const int CampaignLabelPos, const int EventLabelPos, const int GearPos, const int DatePos, const int TimePos, const int DateTimePos, const int LatitudePos, const int LongitudePos, const int ElevationPos, structURL URL[], QString& Metadata );
    void        clearURL( structURL URL[] );

    void        doCreateTextFile( const int Format, const bool CuratorMode = false );

    int         doSelectParametersDialog( const int Env, const QStringList& ParameterList, const bool showShortName, const bool showMethod, const bool showComment, const int maxNumOfParameters, bool& sortParameterList, QStringList& selectedParameters );
    int         doSelectEnvironmentDialog( const bool setGeocode, const int EnvArray[], const QStringList& MetadataList );

/*
    int         doGPStoGpxOptionsDialog( int &StartID, int &EndID, QString &TrackName );
    int         doGPStoKmlOptionsDialog( int &StartID, int &EndID, int &IconColor, int &IconSize, int &TracklineColor, int &TracklineWidth, QString &TrackName, QString &FilenameGoogleEarthProgram, bool &startGoogleEarth );
*/

    bool        containsSelectedEnvironment( const int Mode, const int dataEnv, const int selectedEnv );

    QString     createDir( const QString& Path, const QString& newDirName, const bool createDirInside, const bool emptyDir );

    int         startProgram( const QString& Program, const QString& Filename );
    int         startGoogleEarth( const QString& Program, const QString& Filename );

protected:
    void dragEnterEvent( QDragEnterEvent *event );
    void dropEvent( QDropEvent *event );
    void closeEvent( QCloseEvent *event );
    void onError( const int err );

signals:
    void finishedBuildFilelist( bool );

private slots:
    int  chooseFile();
    int  chooseFiles();
    int  chooseFolder( const QString& Folder = "" );
    int  saveFile();
    int  saveFileAs();

    void exitApplication();
    void hideWindow();
    void newWindow();
    void displayHelp();
    void about();
    void clearStatusMessage();
    void doGetDatasets();

    void doCreateGoogleEarthImportFile();
    void doCreateOceanDataViewFile();
    void doCreateShapefileMetadata();
    void doCreateShapefileData();
    void doCreateUnformatedTextFile();
    void doCreateUnformatedTextFileCuratorMode();
    void doCreateFormatedTextFile();
    void doSetGeocodeRangeFlag();

/*
    void doCreateGpxFile();
    void doCreateKmlFile();
*/

    int doGoogleEarthOptionsDialog();
    int doOceanDataViewOptionsDialog();
    int doShapeFileOptionsDialog();
    int doFormatedTextOptionsDialog();
    int doUnformatedTextOptionsDialog();
    int doSetGeocodeRangeFloatDialog();
    int doSetGeocodeRangeDateTimeDialog();
    int doGetDatasetsDialog();

private:
    QStringList expandCommandline();
    QStringList renameFiles( const QStringList Filename, const QString &searchStr, const QString &replaceStr );
    bool buildFilename( const int ActionNumber, const QString &FilenameFormat, const int Extension, const QString &Filename, QString &FilenameIn, QString &FilenameOut );
    bool containsBinaryFile( const QStringList FilenameList );
    bool existsFirstFile( const int ActionNumber, const QString &FilenameFormat, const int Extension, QStringList &FilenameList );
    bool isEmptyLine( const QString &String );
    bool check7z();
    int NumOfSections( const QString &String );
    int addToFilenameList( QStringList &FilenameList, const QString &Filename, const bool showAllFiles = true, const bool decompressFile = false );
    int calcFileSizeClass( const QString &FilenameIn, const int NumOfFiles, const int lowerLimit = 100, const int upperLimit = 1000 );
    int copyStringList( const QStringList InStringList, QStringList &OutStringList, const QString &s_Pattern = "*" );
    int emptyDir( const QString &Dir );
    int decompressFile( const QString &Filename, const bool delZipFile = false );
    int incFileProgress( const int NumOfFiles, const int FileNumber );
    int incProgress( const int NumOfFiles, const int Step );
    int readFile( const QString &FilenameIn, QStringList &Input, const int Codec = -1, const int NumOfFiles = 0, const qint64 Bytes = 0 );
    int removeFile( const QString &Filename );
    int downloadFile( const QString &Url, const QString &absoluteFilePath );
    unsigned int incProgress( const int NumOfFiles, const unsigned int filesize, const unsigned int length, const QString &InputStr );
    void appendItem( QStringList &List, const QString &Item, const QString &SS = "", const QString &RS = "" );
    void compressFile( const QString &FilenameIn );
    void clearFilenameList( int &ActionNumber, QStringList &FilenameList );
    void clearList( QStringList& List );
    void clearMessage();
    void createActions();
    void createMenus();
    void createStatusBar( const bool showProgressBar );
    void enableMenuItems( const QStringList FilenameList );
    void endTool( const int err, const int stopProgress, int &ActionNumber, const QString &FilenameFormat, const int Extension, QStringList &FilenameList, const QString &doneMessage = "Done", const QString &canceledMessage = "Converter was canceled", const bool clearList = false, const bool incActionNumber = true );
    void initFileProgress( const int NumOfFiles, const QString &FilenameIn, const QString &MessageText );
    void initProgress( const int NumOfFiles, const QString &Filename, const QString &MessageText, const int totalNumberOfSteps );
    void initProgress( const int totalNumberOfSteps );
    void listDir( QStringList &Filename, const QString &Directory, const bool showAllFiles = true, const bool decompressFile = false );
    void loadPreferences();
    void resetFileProgress( const int NumOfFiles );
    void resetProgress( const int NumOfFiles = 0 );
    void savePreferences();
    void setNormalCursor();
    void setStatusBar( const QString &Message = "", const int seconds = 0 );
    void setStatusBarFileInProgress( const QString &Filename, const QString &Message = "", const int i_seconds = 0 );
    void setWTitle( const int ActionNumber, const QString &FilenameFormat, const int Extension, const QStringList FilenameList );
    void setWaitCursor();
    void showFilenameList( const int ActionNumber, const QString &FilenameFormat, const int Extension, const QStringList FilenameList );
    void showList( const QStringList List );
    void showMessage( const QString &Message, QStringList &MessageList );
    void wait( const int msec );

    QString getApplicationName( const bool replaceUnderline = false );
    QString getDataLocation();
    QString getDocumentDir();
    QString getPreferenceFilename();
    QString getVersion();
    QString setEOLChar( const int EOL );
    QString setExtension( const int Extension );

    QMenu	*fileMenu;
    QMenu	*toolsMenu;
    QMenu	*optionsMenu;
    QMenu	*helpMenu;

    QAction *newWindowAction;
    QAction *openFileAction;
    QAction *openFolderAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *hideWindowAction;
    QAction *getDatasetsAction;
    QAction *setEncodingDialogAction;
    QAction *exitAction;
    QAction *aboutAction;
    QAction *aboutQtAction;
    QAction *showHelpAction;

    QAction *createGoogleEarthImportFileAction;
    QAction *createOceanDataViewFileAction;
    QAction *createShapefileMetadataAction;
    QAction *createShapefileDataAction;
    QAction *createUnformatedTextFileAction;
    QAction *createUnformatedTextFileCuratorModeAction;
    QAction *createFormatedTextFileAction;

    QAction *ASCIIOptionsAction;
    QAction *GoogleEarthOptionsAction;
    QAction *OceanDataViewOptionsAction;
    QAction *formatedTextOptionsAction;
    QAction *unformatedTextOptionsAction;
    QAction *ShapeFileOptionsAction;
    QAction *setGeocodeRangeFlagAction;

    QProgressDialog *ProgressDialog;
    QProgressDialog *FileProgressDialog;
    QProgressBar    *ProgressBar;
    QLabel			*StatusMessage;

    QStringListModel	*Model;
    QListView			*ListView;

    QTextEdit			*textViewer;

    QPoint	posWindow;              //!< Position des Programmfensters.
    QSize	sizeWindow;             //!< Groesse des Programmfensters.

    QPoint	posDialog;              //!< Position der Dialoge.
    QSize	sizeParameterDialog;	 //!< Groesse der Parameterdialogs.
};

#endif
