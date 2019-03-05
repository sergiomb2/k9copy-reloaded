//
// C++ Implementation: k9Settings
//
// Description:
//
// Developer: Pauline123 <pauline123@users.sf.net>, (C) 2014
//
// Author: Jean-Michel PETIT <k9copy@free.fr>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "k9settings.h"

#include <qframe.h>


#include <qlayout.h>
#include <QDialogButtonBox>

#if QT_VERSION >= 0x050000
#include <qdialog.h>
#include <kiconloader.h>
#include <qaction.h>
#include <QIcon>
#else
#include <kglobal.h>
#include <KIcon>
#include <kiconloader.h>
#include <kdialog.h>
#include <kaction.h>
#endif //Qt5 Code

#include <QDebug>

k9Settings::k9Settings(QWidget *parent,const QString &):  KPageDialog (parent) {

    setFaceType(KPageDialog::List);
#if QT_VERSION >= 0x050000 //Qt5 Code
  	setStandardButtons( QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Apply );
#else // Qt4 Code
    setButtons( Ok|Apply|Cancel);

#endif
	 setWindowTitle((i18n("Settings")));
    //setButtons( Default|Ok|Apply|Cancel);

    //setInitialSize(QSize(750,350), false);
    KPageWidgetItem *page;


    m_prefDVD =new k9prefDVD(this);
    page = addPage(m_prefDVD,i18n("DVD"));
    page->setHeader(i18n("DVD Backup"));
#if QT_VERSION >= 0x050000
    page->setIcon(QIcon("dvdcopy"));
#else
    page->setIcon(KIcon("dvdcopy"));
#endif

    m_prefMencoder =new k9prefMencoder(this);
    page = addPage(m_prefMencoder,i18n("Encoders"));
    page->setHeader(i18n("MPEG-4 Codecs"));
#if QT_VERSION >= 0x050000
    page->setIcon(QIcon("mencoder"));
#else
    page->setIcon(KIcon("mencoder"));
#endif
    m_prefMPEG4 =new k9prefMPEG4(this);
    page = addPage(m_prefMPEG4,i18n("MPEG-4"));
    page->setHeader(i18n("MPEG-4 Encoding"));
#if QT_VERSION >= 0x050000
    page->setIcon(QIcon("video-mp4"));
#else
    page->setIcon(KIcon("video-mp4"));
#endif

    m_prefPreview =new k9prefPreview(this);
    page = addPage(m_prefPreview,i18n("Preview"));
    page->setHeader(i18n("Title preview"));
#if QT_VERSION >= 0x050000
    page->setIcon(QIcon("video-projector"));
#else
    page->setIcon(KIcon("video-projector"));
#endif

    m_prefAuthor = new k9prefAuthor(this);
    page = addPage(m_prefAuthor,i18n("Authoring"));
    page->setHeader(i18n("Authoring"));
#if QT_VERSION >= 0x050000
    page->setIcon(QIcon("author"));
#else
    page->setIcon(KIcon("author"));
#endif

    m_prefDirs=new k9prefDirs(this);
    page = addPage(m_prefDirs,i18n("Paths"));
    page->setHeader(i18n("Default paths"));
#if QT_VERSION >= 0x050000
    page->setIcon(QIcon("folder"));
#else
    page->setIcon(KIcon("folder"));
#endif
    

    m_configDlg=new kConfigDlg(this);
    page = addPage(m_configDlg,i18n("Devices"));
    page->setHeader(i18n("Devices not detected by k9copy"));
#if QT_VERSION >= 0x050000
    page->setIcon(QIcon("drive-optical"));
#else
    page->setIcon(KIcon("drive-optical"));

#endif

#if QT_VERSION >= 0x050000
connect(buttonBox()->button(QDialogButtonBox::Apply),SIGNAL(clicked()), SLOT(slotApplyClicked()));
connect(buttonBox()->button(QDialogButtonBox::Ok),SIGNAL(clicked()), SLOT(slotOkClicked()));
#else
    connect(this, SIGNAL(applyClicked()), SLOT(slotApplyClicked()));
    connect(this, SIGNAL(okClicked()), SLOT(slotOkClicked()));
#endif
}

void k9Settings::slotOkClicked() {
    qDebug() << "THIS IS THE SLOT/SIGNAL WORKING";	
    m_prefDVD->save();
    m_prefMPEG4->save();
    m_prefMencoder->save();
    m_prefPreview->save();
    m_prefAuthor->save();

    m_configDlg->save();

    m_prefDirs->save();

}

void k9Settings::slotApplyClicked() {
   slotOkClicked();
   m_prefMPEG4->load();
}

k9Settings::~k9Settings() {
 /*   delete m_configDlg;
    delete m_prefDVD;
    delete m_prefMPEG4;
    delete m_prefMencoder;
    delete m_prefPreview;
*/
}
