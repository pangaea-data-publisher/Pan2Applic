// ***********************************************************************************************
// *                                                                                             *
// * SelectParametersDialog.cpp - Selects parameter from parameter list                          *
// *                                                                                             *
// * Dr. Rainer Sieger - 2009-05-13                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtWidgets>

#include "Application.h"
#include "selectParametersDialog.h"

SelectParametersDialog::SelectParametersDialog( QStringList list, int n, bool m, QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    connect(OK_pushButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(Cancel_pushButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(SelectMetadata_pushButton, SIGNAL(clicked()), this, SLOT(selectMetadata()));
    connect(SelectData_pushButton, SIGNAL(clicked()), this, SLOT(selectData()));
    connect(SelectGeocode_pushButton, SIGNAL(clicked()), this, SLOT(selectGeocode()));
    connect(SelectEventGeocodeData_pushButton, SIGNAL(clicked()), this, SLOT(selectEventGeocodeData()));
    connect(DeselectAll_pushButton, SIGNAL(clicked()), this, SLOT(deselectAll()));
    connect(sortList_checkBox, SIGNAL(clicked()), this, SLOT(sortList()));
    connect(Left2Right_pushButton, SIGNAL(clicked()), this, SLOT(slotLeft2Right()));
    connect(Right2Left_pushButton, SIGNAL(clicked()), this, SLOT(slotRight2Left()));
    connect(lb1, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(slotLeft2Right()));
    connect(lb2, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(slotRight2Left()));

    lb1->setSelectionMode( QAbstractItemView::ExtendedSelection );
    lb2->setSelectionMode( QAbstractItemView::ExtendedSelection );

    OK_pushButton->setWhatsThis( "Close dialog" );
    Cancel_pushButton->setWhatsThis( "Cancel dialog" );

    i_maxNumOfItems = n;
    sl_ItemList     = list;
    b_CuratorMode   = m;

    lb1->addItems( sl_ItemList );

    if ( sl_ItemList.filter( "1:" ).count() == 0 )
    {
        SelectMetadata_pushButton->hide();
        SelectData_pushButton->setText( "Select all");
    }

    enableOKButton();
}

// *****************************************************************************
// *****************************************************************************
// *****************************************************************************
/*
 * SLOT slotLeft2Right
 *
 * Move all selected items of the first ListBox into the
 * second ListBox
 */

void SelectParametersDialog::slotLeft2Right()
{
    int n = 0;

    for ( int i=0; i<lb1->count(); ++i )
    {
        if ( lb1->item( i )->isSelected() == true )
            ++n;
    }

    if ( n<i_maxNumOfItems )
    {
        n = lb1->count()-1;

        for ( int i=0; i<=n; ++i )
        {
            if ( lb1->item( i )->isSelected() == true )
                lb2->addItem( lb1->item( i )->text() );
        }

        for ( int i=n; i>=0; --i )
        {
            if ( lb1->item( i )->isSelected() == true )
                lb1->takeItem( i );
        }
    }
    else
    {
        QMessageBox::information( this, "Pan2Applic", QString( "The application you have choosen\nsupports a maximum of %1 parameters." ).arg( i_maxNumOfItems ) );

        for ( int i=0; i<lb1->count(); ++i )
            lb1->setCurrentItem( lb1->item( i ), QItemSelectionModel::Deselect );
    }

    if ( sortList_checkBox->isChecked() )
        lb1->sortItems();

    enableOKButton();
}

// *****************************************************************************
// *****************************************************************************
// *****************************************************************************
/*
 * SLOT slotRight2Left
 *
 * Move all selected items of the second ListBox into the
 * first ListBox
 */

void SelectParametersDialog::slotRight2Left()
{
    int n = lb2->count()-1;

    for ( int i=0; i<=n; ++i )
    {
        if ( lb2->item( i )->isSelected() == true )
            lb1->addItem( lb2->item( i )->text() );
    }

    for ( int i=n; i>=0; --i )
    {
        if ( lb2->item( i )->isSelected() == true )
            lb2->takeItem( i );
    }

    enableOKButton();
}

// *****************************************************************************
// *****************************************************************************
// *****************************************************************************

void SelectParametersDialog::selectMetadata()
{
    for ( int i=0; i<lb1->count(); ++i )
        if ( lb1->item( i )->text().startsWith( "1:") == true )
            lb1->setCurrentItem( lb1->item( i ), QItemSelectionModel::Select );

    slotLeft2Right();
}

// *****************************************************************************
// *****************************************************************************
// *****************************************************************************

void SelectParametersDialog::selectData()
{
    for ( int i=0; i<lb1->count(); ++i )
        if ( lb1->item( i )->text().startsWith( "2:") == true )
            lb1->setCurrentItem( lb1->item( i ), QItemSelectionModel::Select );

    slotLeft2Right();
}

// *****************************************************************************
// *****************************************************************************
// *****************************************************************************

void SelectParametersDialog::selectGeocode()
{
    for ( int i=0; i<lb1->count(); ++i )
        if ( lb1->item( i )->text().startsWith( "3:") == true )
            lb1->setCurrentItem( lb1->item( i ), QItemSelectionModel::Select );

    slotLeft2Right();
}

// *****************************************************************************
// *****************************************************************************
// *****************************************************************************

void SelectParametersDialog::selectEventGeocodeData()
{
    for ( int i=0; i<lb1->count(); ++i )
        if ( lb1->item( i )->text().startsWith( "1: Event") == true )
            lb1->setCurrentItem( lb1->item( i ), QItemSelectionModel::Select );

    for ( int i=0; i<lb1->count(); ++i )
        if ( lb1->item( i )->text().startsWith( "3:") == true )
            lb1->setCurrentItem( lb1->item( i ), QItemSelectionModel::Select );

    for ( int i=0; i<lb1->count(); ++i )
        if ( lb1->item( i )->text().startsWith( "2:") == true )
            lb1->setCurrentItem( lb1->item( i ), QItemSelectionModel::Select );

    slotLeft2Right();
}

// *****************************************************************************
// *****************************************************************************
// *****************************************************************************

void SelectParametersDialog::deselectAll()
{
    sortList();
}

// *****************************************************************************
// *****************************************************************************
// *****************************************************************************

void SelectParametersDialog::sortList()
{
    while ( lb1->count() > 0 )
        lb1->takeItem( 0 );

    while ( lb2->count() > 0 )
        lb2->takeItem( 0 );

    for ( int i=0; i<sl_ItemList.count(); ++i )
        lb1->addItem( sl_ItemList.at( i ) );

    if ( sortList_checkBox->isChecked() )
        lb1->sortItems();

    enableOKButton();
}

// *****************************************************************************
// *****************************************************************************
// *****************************************************************************
/*
 * setItemBackground()
 *
 * sets the background color of items
 */

void SelectParametersDialog::setItemBackground()
{
    // SWG by EkaterineN.Tsomaia - http://colrd.com/palette/18603/ - #698A91, #C6CEB6, #9FB1A2
    // Yellow by Paul Ramcharitar - http://colrd.com/palette/24725/ #FFDD88    #FBCE6E    #EEBB55
    for ( int i=0; i<lb1->count(); ++i )
        if ( lb1->item( i )->text().startsWith( "1:") == true )
            lb1->item( i )->setBackground( QColor( "#FFDD88" ) );

    for ( int i=0; i<lb1->count(); ++i )
        if ( lb1->item( i )->text().startsWith( "2:") == true )
            lb1->item( i )->setBackground( QColor( "#FBCE6E" ) );

    for ( int i=0; i<lb1->count(); ++i )
        if ( lb1->item( i )->text().startsWith( "3:") == true )
            lb1->item( i )->setBackground( QColor( "#EEBB55" ) );

    for ( int i=0; i<lb2->count(); ++i )
        if ( lb2->item( i )->text().startsWith( "1:") == true )
            lb2->item( i )->setBackground( QColor( "#FFDD88" ) );

    for ( int i=0; i<lb2->count(); ++i )
        if ( lb2->item( i )->text().startsWith( "2:") == true )
            lb2->item( i )->setBackground( QColor( "#FBCE6E" ) );

    for ( int i=0; i<lb2->count(); ++i )
        if ( lb2->item( i )->text().startsWith( "3:") == true )
            lb2->item( i )->setBackground( QColor( "#EEBB55" ) );
}

// *****************************************************************************
// *****************************************************************************
// *****************************************************************************

void SelectParametersDialog::enableOKButton()
{
    bool    b_Event = false;

// *****************************************************************************

    setItemBackground();

    SelectMetadata_pushButton->setEnabled( false );
    SelectData_pushButton->setEnabled( false );
    SelectGeocode_pushButton->setEnabled( false );
    SelectEventGeocodeData_pushButton->setEnabled( false );
    DeselectAll_pushButton->setEnabled( false );

    for ( int i=0; i<lb1->count(); i++ )
    {
        if ( lb1->item( i )->text().startsWith( "1: Event") == true )
        {
            b_Event = true;
            break;
        }
    }

    for ( int i=0; i<lb1->count(); i++ )
    {
        if ( lb1->item( i )->text().startsWith( "1:") == true )
        {
            SelectMetadata_pushButton->setEnabled( true );
            break;
        }
    }

    for ( int i=0; i<lb1->count(); i++ )
    {
        if ( lb1->item( i )->text().startsWith( "2:") == true )
        {
            SelectData_pushButton->setEnabled( true );
            break;
        }
    }

    for ( int i=0; i<lb1->count(); i++ )
    {
        if ( lb1->item( i )->text().startsWith( "3:") == true )
        {
            SelectGeocode_pushButton->setEnabled( true );
            break;
        }
    }

    if ( ( SelectMetadata_pushButton->isEnabled() == true ) && ( SelectData_pushButton->isEnabled() == true ) && (SelectGeocode_pushButton->isEnabled() == true ) )
        SelectEventGeocodeData_pushButton->setEnabled( true );

    if ( lb2->count() > 0 )
    {
        OK_pushButton->setEnabled( true );
        DeselectAll_pushButton->setEnabled( true );
        SelectData_pushButton->setDefault( false );

        OK_pushButton->setDefault( true );
        OK_pushButton->setFocus();
    }
    else
    {
        OK_pushButton->setEnabled( false );
        OK_pushButton->setDefault( false );

        SelectData_pushButton->setDefault( true );
        SelectData_pushButton->setFocus();
    }

    if ( lb2->count() > i_maxNumOfItems )
    {
        DeselectAll_pushButton->setEnabled( true );
        OK_pushButton->setEnabled( false );
        OK_pushButton->setDefault( false );

        SelectData_pushButton->setDefault( true );
        SelectData_pushButton->setFocus();

        QMessageBox::information( this, "Pan2Applic", QString( "The application you have choosen\nsupports a maximum of %1 parameters.\nPlease remove %2 parameter(s) from the\nlist or click on cancel." ).arg( i_maxNumOfItems ).arg( lb2->count() - i_maxNumOfItems ) );
    }

    if ( ( SelectData_pushButton->isDefault() == true ) && ( b_CuratorMode == true ) )
    {
        SelectEventGeocodeData_pushButton->setDefault( true );
        SelectEventGeocodeData_pushButton->setFocus();
    }
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

int MainWindow::doSelectParametersDialog( const int i_Env, const QStringList& sl_ParameterList, const bool b_showShortName, const bool b_showMethod, const bool b_showComment, const int i_maxNumOfParameters, bool& b_sortParameterList, QStringList& sl_selectedParameters )
{
    int         err                             = _ERROR_;

    QString     s_Parameter                     = "";
    QString     s_Item                          = "";

    QStringList sl_ItemList;

    bool        b_containsSelectedEnvironment   = false;

// ***********************************************************************************************************************

    if ( sl_ParameterList.count() < 1 )
    {
        QMessageBox::information( this, "Pan2Applic", QString( "Can't find data in file" ) );
        return( _ERROR_ );
    }

    sl_selectedParameters.clear();

    for ( int i=0; i<sl_ParameterList.count(); ++i )
    {
        b_containsSelectedEnvironment = containsSelectedEnvironment( _SELECTPARAMETER, sl_ParameterList.at( i ).section( _PSEPARATOR_, 0, 0 ).toInt(), i_Env );

        if ( ( b_containsSelectedEnvironment == true ) || ( sl_ParameterList.at( i ).endsWith( "Metadata" ) == true ) )
        {
            s_Parameter = buildParameter( sl_ParameterList.at( i ), b_showShortName, b_showMethod, b_showComment );

            if ( sl_ItemList.contains( s_Parameter ) == false )
                sl_ItemList.append( s_Parameter );
        }
    }

// ***********************************************************************************************************************

    SelectParametersDialog dialog( sl_ItemList, i_maxNumOfParameters, gb_CuratorMode );

    if ( gb_CuratorMode == true )
        dialog.setWindowTitle( tr( "Select parameters (curator mode)" ) );
    else
        dialog.SelectEventGeocodeData_pushButton->hide();

    dialog.sortList_checkBox->setChecked( b_sortParameterList );

    if ( dialog.sortList_checkBox->isChecked() == true )
        dialog.lb1->sortItems();

// ******************************************************************************************************

    dialog.OK_pushButton->setWhatsThis( "Close dialog" );
    dialog.Cancel_pushButton->setWhatsThis( "Cancel dialog" );

    dialog.move( posDialog );
    dialog.resize( sizeParameterDialog );
    dialog.show();

// ******************************************************************************************************

    switch ( dialog.exec() )
    {
    case QDialog::Accepted:
        b_sortParameterList = dialog.sortList_checkBox->isChecked();

        for ( int i=0; i<dialog.lb2->count(); ++i )
        {
            s_Item      = dialog.lb2->item( i )->text();
            s_Parameter = buildParameter( s_Item, sl_ParameterList, b_showShortName, b_showMethod, b_showComment );

            if ( s_Parameter.isEmpty() == false )
                sl_selectedParameters.append( s_Parameter );
        }

        err = _NOERROR_;
        break;

    case QDialog::Rejected:
        break;

    default:
        break;
    }

    posDialog           = dialog.pos();
    sizeParameterDialog = dialog.size();

    return( err );
}
