#ifndef _GLOBALS

    #define _GLOBALS

// Application constants
    const int   _NOERROR_                                = 0;
    const int   _ERROR_                                  = 1;
    const int   _APPBREAK_                               = 2;
    const int   _NODATAFOUND_                            = 3;
    const int   _DATAFOUND_                              = 4;
    const int   _CHOOSEABORTED_                          = 5;
    const int   _FILENOEXISTS_                           = 6;

    const int   _ZIP_                                    = 1;
    const int   _GZIP_                                   = 2;

    const QString _PROGRAMDOI_                           = "doi:10.1594/PANGAEA.288115";
    const QString _PROGRAMREADME_                        = "https://wiki.pangaea.de/wiki/Pan2Applic";

    const QString _VERSION_                              = "4.13";  // Version number, 2016-08-18
    const QChar   _PSEPARATOR_                           = '|';

    const int _MAX_NUM_OF_PARAMETERS                     = 1000;
    const int _MAX_NUM_OF_URLS                           = 20;
    const int _MAX_PARAMETER_LENGTH                      = 50;
    const int _MAX_NUM_OF_ENV                            = 15;

    const int _DEVICETYPEB                               = 1;
    const int _DEVICETYPEC                               = 2;
    const int _DEVICETYPEAUTO                            = 3;

    const int _ISDOUBLE                                  = 0;
    const int _ISSTRING                                  = 1;
    const int _ISDATE                                    = 2;
    const int _ISTIME                                    = 3;
    const int _ISDATETIME                                = 4;
    const int _ISDOI                                     = 5;
    const int _ISELEVATION                               = 6;
    const int _ISEVENT                                   = 7;
    const int _ISLATITUDE                                = 8;
    const int _ISLONGITUDE                               = 9;

    const int _ISGEOCODE                                 = 1;
    const int _ISMETADATA                                = 2;
    const int _ISPARAMETER                               = 3;

    const int _ANALYZEHEADER                             = 1;
    const int _SELECTPARAMETER                           = 2;
    const int _BUILDHEADER                               = 3;
    const int _BUILDIMPORTFILE                           = 4;

    const int _BUILDDATE                                 = 1;
    const int _BUILDDATETIME                             = 2;
    const int _BUILDISODATETIME                          = 3;

    const int _FORMAT_ISO                                = 0;
    const int _FORMAT_ODV                                = 1;
    const int _FORMAT_PANMAP                             = 2;
    const int _FORMAT_PANPLOT                            = 3;
    const int _FORMAT_SHAPE_DATA                         = 4;
    const int _FORMAT_SHAPE_METADATA                     = 5;
    const int _FORMAT_UNFORMATEDTEXTFILE                 = 6;
    const int _FORMAT_FORMATEDTEXTFILE                   = 7;
    const int _FORMAT_KMLFILE                            = 8;

    const int _FILENAMEPOS                               = 0;
    const int _METAINFOPOS                               = 1;
    const int _NUMOFHEADERLINESPOS                       = 2;
    const int _ENVIRONMENTPOS                            = 3;
    const int _CAMPAIGNLABELPOS                          = 4;
    const int _EVENTLABELPOS                             = 5;
    const int _GEARNAMEPOS                               = 6;
    const int _DATEPOS                                   = 7;
    const int _TIMEPOS                                   = 8;
    const int _DATETIMEPOS                               = 9;
    const int _DATETIMESTARTPOS                          = 10;
    const int _LATITUDEPOS                               = 11;
    const int _LONGITUDEPOS                              = 12;
    const int _ELEVATIONPOS                              = 13;
    const int _AREANAMEPOS                               = 14;
    const int _DOIPOS                                    = 15;
    const int _CITATIONPOS                               = 16;
    const int _DATADATEPOS                               = 17;
    const int _DATATIMEPOS                               = 18;
    const int _DATADATETIMEPOS                           = 19;
    const int _DATAELEVATIONPOST                         = 20;
    const int _DATAEVENTLABELPOS                         = 21;
    const int _DATACAMPAIGNLABELPOS                      = 22;
    const int _DATAAREANAMEPOS                           = 23;
    const int _DATAGEARNAMEPOS                           = 24;
    const int _DATALATITUDEPOS                           = 25;
    const int _DATALONGITUDEPOS                          = 26;

    const int _NOTGIVEN                                  = 2;       // 1
    const int _WATER                                     = 4;       // 2
    const int _SEDIMENT                                  = 8;       // 3
    const int _ICE                                       = 16;      // 4
    const int _DISTANCE                                  = 32;      // 5
    const int _POINTDISTANCE                             = 64;      // 6
    const int _AIRA                                      = 128;     // 7
    const int _AIRH                                      = 256;     // 8
    const int _DATETIME                                  = 512;     // 9
    const int _AGE                                       = 1024;    // 10
    const int _ORDINALNUMBER                             = 2048;    // 11
    const int _ELEVATION                                 = 4096;    // 12
    const int _SECTION                                   = 8192;    // 13
    const int _EVENTIMPORT                               = 16384;   // 14
    const int _MAXENV                                    = 32768;   // 15

    const int _DATE                                      = 1;
    const int _TIME                                      = 2;
    const int _TYPE                                      = 3;
    const int _CAMPAIGN                                  = 4;
    const int _ELEVATIONEVENT                            = 5;
    const int _EVENT                                     = 6;
    const int _LATITUDE                                  = 7;
    const int _LONGITUDE                                 = 8;
    const int _AREA                                      = 9;
    const int _GEAR                                      = 10;

    const int _RED                                       = 0;
    const int _GREEN                                     = 1;
    const int _BLUE                                      = 2;
    const int _ORANGE                                    = 3;
    const int _YELLOW                                    = 4;
    const int _WHITE                                     = 5;

    const int _CIRCLE                                    = 0;
    const int _SQUARE                                    = 1;
    const int _STAR                                      = 2;
    const int _TRIANGLE                                  = 3;

    const int _TABSTOP_                                  = 0;
    const int _COMMA_                                    = 1;
    const int _SEMICOLON_                                = 2;
    const int _SPACE_                                    = 3;

// EOL
    const int _UNIX_                                     = 0;    // LF
    const int _MACOS_                                    = 1;    // CR
    const int _WIN_                                      = 2;    // CR+LF

// Encoding
    const int _SYSTEM_                                   = -1;   // System
    const int _UTF8_                                     = 0;    // UTF-8
    const int _APPLEROMAN_                               = 1;    // Apple Roman
    const int _LATIN1_                                   = 2;    // Latin-1 = ISO 8859-1

// Extension
    const int _TXT_                                      = 0;
    const int _CSV_                                      = 1;

#endif
