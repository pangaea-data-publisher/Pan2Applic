#ifndef SETCAMPAIGNNAMEDIALOG_H
#define SETCAMPAIGNNAMEDIALOG_H

#include <QDialog>

#include "ui_setcampaignnamedialog.h"

class SetCampaignNameDialog : public QDialog, public Ui::SetCampaignNameDialog
{
Q_OBJECT

public:
    SetCampaignNameDialog( QWidget *parent = 0 );
};

#endif
