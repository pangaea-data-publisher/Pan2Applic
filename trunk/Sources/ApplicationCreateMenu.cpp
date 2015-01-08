// ***********************************************************************************************
// *                                                                                             *
// * createMenu.cpp - creates application menus                                                  *
// *                                                                                             *
// * Dr. Rainer Sieger - 2008-05-18                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

/*! @brief Erstellen der Menue-Aktionen. */

void MainWindow::createActions()
{
// File menu
    newWindowAction = new QAction(tr("&New window"), this);
    newWindowAction->setShortcut(tr("Ctrl+N"));
    connect(newWindowAction, SIGNAL(triggered()), this, SLOT(newWindow()));

    openFileAction = new QAction(tr("&Open..."), this);
    openFileAction->setShortcut(tr("Ctrl+O"));
    connect(openFileAction, SIGNAL(triggered()), this, SLOT(chooseFiles()));

    openFolderAction = new QAction(tr("Select &Folder..."), this);
    openFolderAction->setShortcut(tr("Ctrl+F"));
    connect(openFolderAction, SIGNAL(triggered()), this, SLOT(chooseFolder()));

    saveAction = new QAction(tr("&Save"), this);
    saveAction->setShortcut(tr("Ctrl+S"));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(saveFile()));

    saveAsAction = new QAction(tr("Save &as..."), this);
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveFileAs()));

    hideWindowAction = new QAction(tr("&Close window"), this);
    hideWindowAction->setShortcut(tr("Ctrl+W"));
    connect(hideWindowAction, SIGNAL(triggered()), this, SLOT(hideWindow()));

    getFilesAction = new QAction(tr("&Download PANGAEA datasets..."), this);
    getFilesAction->setShortcut(tr("Ctrl+D"));
    connect(getFilesAction, SIGNAL(triggered()), this, SLOT(doGetDatasets()));

    exitAction = new QAction(tr("&Quit"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    // Converter menu
    createGoogleEarthImportFileAction = new QAction(tr("&Google Earth..."), this);
    createGoogleEarthImportFileAction->setShortcut(tr("Ctrl+G"));
    connect(createGoogleEarthImportFileAction, SIGNAL(triggered()), this, SLOT(doCreateGoogleEarthImportFile()));

    createOceanDataViewFileAction = new QAction(tr("Ocean Data &View..."), this);
    createOceanDataViewFileAction->setShortcut(tr("Ctrl+V"));
    connect(createOceanDataViewFileAction, SIGNAL(triggered()), this, SLOT(doCreateOceanDataViewFile()));

    createShapefileMetadataAction = new QAction(tr("Shapefile (&metadata only)"), this);
    createShapefileMetadataAction->setShortcut(tr("Ctrl+M"));
    connect(createShapefileMetadataAction, SIGNAL(triggered()), this, SLOT(doCreateShapefileMetadata()));

    createShapefileDataAction = new QAction(tr("&Shapefile..."), this);
    createShapefileDataAction->setShortcut(tr("Ctrl+A"));
    connect(createShapefileDataAction, SIGNAL(triggered()), this, SLOT(doCreateShapefileData()));

    createUnformatedTextFileAction = new QAction(tr("&Text (unformated)..."), this);
    createUnformatedTextFileAction->setShortcut(tr("Ctrl+T"));
    connect(createUnformatedTextFileAction, SIGNAL(triggered()), this, SLOT(doCreateUnformatedTextFile()));

    createUnformatedTextFileCuratorModeAction = new QAction(tr("Data &reimport..."), this);
    createUnformatedTextFileCuratorModeAction->setShortcut(tr("Ctrl+R"));
    connect(createUnformatedTextFileCuratorModeAction, SIGNAL(triggered()), this, SLOT(doCreateUnformatedTextFileCuratorMode()));

    createFormatedTextFileAction = new QAction(tr("Te&xt (formated)..."), this);
    createFormatedTextFileAction->setShortcut(tr("Ctrl+X"));
    connect(createFormatedTextFileAction, SIGNAL(triggered()), this, SLOT(doCreateFormatedTextFile()));

    // Options menu
    setGeocodeRangeFlagAction = new QAction(tr("Set geocode range"), this);
    setGeocodeRangeFlagAction->setCheckable( true );
    connect(setGeocodeRangeFlagAction, SIGNAL(triggered()), this, SLOT(doSetGeocodeRangeFlag()));

    GoogleEarthOptionsAction = new QAction(tr("&Google Earth"), this);
    GoogleEarthOptionsAction->setShortcut(tr("Alt+G"));
    connect(GoogleEarthOptionsAction, SIGNAL(triggered()), this, SLOT(doGoogleEarthOptionsDialog()));

    OceanDataViewOptionsAction = new QAction(tr("Ocean &Data View"), this);
    OceanDataViewOptionsAction->setShortcut(tr("Alt+V"));
    connect(OceanDataViewOptionsAction, SIGNAL(triggered()), this, SLOT(doOceanDataViewOptionsDialog()));

    unformatedTextOptionsAction = new QAction(tr("&Text (unformated)"), this);
    unformatedTextOptionsAction->setShortcut(tr("Alt+T"));
    connect(unformatedTextOptionsAction, SIGNAL(triggered()), this, SLOT(doUnformatedTextOptionsDialog()));

    formatedTextOptionsAction = new QAction(tr("Te&xt (formated)"), this);
    formatedTextOptionsAction->setShortcut(tr("Alt+X"));
    connect(formatedTextOptionsAction, SIGNAL(triggered()), this, SLOT(doFormatedTextOptionsDialog()));

    // Help menu
    aboutAction = new QAction(tr("&About ") + getApplicationName( true ), this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAction = new QAction(tr("About &Qt"), this);
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    showHelpAction = new QAction(getApplicationName( true ) + tr(" &Help"), this);
    showHelpAction->setShortcut(tr("F1"));
    connect(showHelpAction, SIGNAL(triggered()), this, SLOT(displayHelp()));

#if defined(Q_OS_WIN)
    newWindowAction->setStatusTip(tr("Create a new file"));
    openFileAction->setStatusTip(tr("Choose an existing file"));
    openFolderAction->setStatusTip(tr("Choose an existing folder"));
    getFilesAction->setStatusTip(tr("Download datasets form PANGAEA"));
    saveAction->setStatusTip(tr("Save the document to disk"));
    saveAsAction->setStatusTip(tr("Save the document under a new name"));
    exitAction->setStatusTip(tr("Exit the application"));
    createGoogleEarthImportFileAction->setStatusTip(tr("create KML file (Google Earth)"));
    createOceanDataViewFileAction->setStatusTip(tr("create ODV file (Ocean Data View)"));
    createShapefileMetadataAction->setStatusTip(tr("create shapefile with metadata only"));
    createShapefileDataAction->setStatusTip(tr("create shapefile"));
    createUnformatedTextFileAction->setStatusTip(tr("create unformated text file"));
    createFormatedTextFileAction->setStatusTip(tr("create formated text file"));
    setGeocodeRangeFlagAction->setStatusTip(tr("show the set geocode range dialog before build"));
    GoogleEarthOptionsAction->setStatusTip(tr("Google Earth options"));
    OceanDataViewOptionsAction->setStatusTip(tr("Ocean Data View options"));
    unformatedTextOptionsAction->setStatusTip(tr("unformated Text options"));
    formatedTextOptionsAction->setStatusTip(tr("formated Text options"));
    aboutAction->setStatusTip(tr("Show the application's About box"));
    aboutQtAction->setStatusTip(tr("Show the Qt library's About box"));
    showHelpAction->setStatusTip(tr("Show the application's help"));
#endif
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

/*! @brief Verbindet Menues mit Aktionen. */

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu( tr( "&File" ) );
    fileMenu->addAction( openFileAction );
    fileMenu->addAction( openFolderAction );
    fileMenu->addSeparator();

#if defined(Q_OS_LINUX)
    ;
#endif

#if defined(Q_OS_MAC)
    fileMenu->addAction( newWindowAction );
    newWindowAction->setEnabled( false );
    fileMenu->addAction( hideWindowAction );
#endif

#if defined(Q_OS_WIN)
    fileMenu->addAction( hideWindowAction );
#endif

    fileMenu->addSeparator();
    fileMenu->addAction( getFilesAction );

#if defined(Q_OS_LINUX)
    fileMenu->addSeparator();
    fileMenu->addAction( exitAction );
#endif

#if defined(Q_OS_WIN)
    fileMenu->addSeparator();
    fileMenu->addAction( exitAction );
#endif

// **********************************************************************************************

    toolsMenu = menuBar()->addMenu( tr( "&Convert" ) );

    if ( gb_CuratorMode == true )
    {
        toolsMenu->addAction( createUnformatedTextFileCuratorModeAction );
        toolsMenu->addSeparator();
    }

    toolsMenu->addAction( createGoogleEarthImportFileAction );
    toolsMenu->addSeparator();
    toolsMenu->addAction( createOceanDataViewFileAction );
    toolsMenu->addSeparator();
    toolsMenu->addAction( createShapefileMetadataAction );
    toolsMenu->addAction( createShapefileDataAction );
    toolsMenu->addSeparator();
    toolsMenu->addAction( createUnformatedTextFileAction );
    toolsMenu->addAction( createFormatedTextFileAction );

// **********************************************************************************************

    helpMenu = menuBar()->addMenu( tr( "&Help" ) );
    helpMenu->addAction( aboutAction );
    helpMenu->addAction( aboutQtAction );
    helpMenu->addSeparator();
    helpMenu->addAction( showHelpAction );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2009-05-04

/*! @brief Setzt das set geocode range flag */

void MainWindow::doSetGeocodeRangeFlag()
{
    if ( gb_setGeocode == true )
      gb_setGeocode = false;
    else
      gb_setGeocode = true;
}
