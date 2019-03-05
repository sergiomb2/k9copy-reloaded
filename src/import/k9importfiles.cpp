//
// C++ Implementation: k9copy
//
// Description:
//
// Developer: Pauline123 <pauline123@users.sf.net>, (C) 2014
//
// Author: Jean-Michel PETIT <k9copy@free.fr>, (C) 2005
//
// Copyright: See COPYING file that comes nth this distribution
//
//
#include "k9importfiles.h"
#include "k9settings.h"

#include <qframe.h>
#include "k9tools.h"
#include <QDockWidget>
#include "images.h"
//PM DVDNAV #include "dvdread.h"
#include "dvdread/dvd_reader.h"
#include <QMenu>
#include <QCloseEvent>


#include <KStandardAction>

#if QT_VERSION >= 0x050000
#include <qstatusbar.h>
#include <QMenuBar>
#else
#include <kstatusbar.h>
#include <KMenuBar>
#endif

#include <kstandardshortcut.h> 
#include <KActionCollection>
#include <KShortcutsDialog>
#include <kedittoolbar.h>


#include "k9import.h"
#include "k9menuedit.h"
#include "k9chapteredit.h"
#include "k9newtitle.h"
#include "k9menu.h"
#include "k9dialogs.h"

#if QT_VERSION >= 0x050000
#include <qdialog.h>
#include <QAction>
#else
#include "kdialog.h"
#include <KAction>
#endif

#if QT_VERSION >= 0x050000
// Qt5 Code
	#ifndef FALSE
	#define FALSE (0)
	#endif
	#ifndef TRUE
	#define TRUE (!FALSE)
	#endif

#include <KIconLoader>
#include <QIcon>

#else()
// Qt4 Code
#include <kdeversion.h>
#endif







k9ImportFiles::k9ImportFiles(k9Copy *_parent)
        : KXmlGuiWindow(_parent,0 ) {
   // accept dnd
 
    m_parent=_parent;
    setAcceptDrops(true);
    setDockOptions(QMainWindow::ForceTabbedDocks |QMainWindow::AnimatedDocks);
    setCorner(Qt::BottomRightCorner,Qt::RightDockWidgetArea);
    setCorner(Qt::TopRightCorner,Qt::RightDockWidgetArea);

    setCorner(Qt::BottomLeftCorner,Qt::LeftDockWidgetArea);

    // then, setup our actions
    setupActions();

    // and a status bar
    m_drives=new k9CdDrives(this);
    createWindows();


   // backupAction->setChecked(true);

    changeCaption(i18n("DVD Author"));
}


void k9ImportFiles::createWindows() {

    m_actions["quit"]->setEnabled(true);
    m_actions["preferences"]->setEnabled(true);
    m_actions["create"]->setEnabled(false);

    m_k9Import=new k9Import (this,0,m_drives);
    setCentralWidget(m_k9Import);

    m_newTitle =new k9NewTitle( this);
    m_newTitle->setK9Import(m_k9Import);
    QDockWidget *doc=new QDockWidget(this);//i18n("Shrink Factor"),this,Qt::Tool);
    doc->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable |QDockWidget::DockWidgetClosable );
    doc->setObjectName("addTitle");
    QDockWidget *firstDock=doc;
    m_firstBottom=doc;

    doc->setWidget(m_newTitle);
    doc->setWindowTitle(i18n("Add Title"));
    addDockWidget(Qt::BottomDockWidgetArea, doc);
 

    m_chapterEdit=new k9ChapterEdit(this);
    doc=new QDockWidget(this);//i18n("MPEG4 Encoding Options"),this,Qt::Tool);
    doc->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable |QDockWidget::DockWidgetClosable);
    doc->setWidget(m_chapterEdit);
    doc->setWindowTitle(i18n("Chapter properties"));
    doc->setObjectName("chapterEdit");
    addDockWidget(Qt::BottomDockWidgetArea, doc);
    tabifyDockWidget(doc,firstDock);

    connect(m_k9Import,SIGNAL(aviFileSelected(k9AviFile*)),m_chapterEdit,SLOT(setAviFile(k9AviFile*)));

    doc=new QDockWidget(this);//
    firstDock=doc;
    doc->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable |QDockWidget::DockWidgetClosable);
    m_menuEdit=new k9MenuEdit(this,0,m_k9Import->getNewDVD()->getRootMenu()->getScene());
    doc->setWidget(m_menuEdit);
    doc->setObjectName("menuEdit");
    doc->setWindowTitle(i18n("Edit menu"));
    addDockWidget(Qt::RightDockWidgetArea, doc);
    connect (m_k9Import,SIGNAL(titleSelected(k9Title*)),m_menuEdit,SLOT(titleSelected(k9Title*)));
    connect (m_k9Import,SIGNAL(rootSelected(k9NewDVD*)),m_menuEdit,SLOT(rootSelected(k9NewDVD*)));
    m_k9Import->setMenuEdit(m_menuEdit);

    m_k9Import->init();

    setActions(true);

    setAutoSaveSettings();

}

void k9ImportFiles::removeToolWindows() {
/*    for (KMdiToolViewAccessor *acc=m_ToolViews.first();acc;acc=m_ToolViews.next()) {
        deleteToolWindow(acc);
    }
    m_ToolViews.clear();
*/
}


k9ImportFiles::~k9ImportFiles() {
  delete m_newTitle;
  delete m_menuEdit;
  delete m_chapterEdit;
  m_parent->m_import=NULL;
  m_k9Import->hide();
  m_k9Import->clear();
  delete m_k9Import;
  delete m_drives;

}

bool k9ImportFiles::queryClose   (    ) {

    return true;
}

void k9ImportFiles::setupActions() {
#if QT_VERSION >= 0x050000
    m_actions["create"] = new QAction(QIcon("author"),i18n("Create DVD"),this); 
    backupAction = new QAction(QIcon("dvdcopy"),i18n("DVD Backup"),this );

#else
    m_actions["create"] = new QAction(KIcon("author"),i18n("Create DVD"),this); 
    backupAction = new KAction(KIcon("dvdcopy"),i18n("DVD Backup"),this );
#endif
    m_actions["backup"]=backupAction;
    m_actions["quit"]=KStandardAction::quit(this, SLOT(quit()),(QObject*) actionCollection());
    m_actions["preferences"]=KStandardAction::preferences(this,SLOT(preferences()),(QObject*)actionCollection());
    KStandardAction::keyBindings(this, SLOT(optionsConfigureKeys()), (QObject*)actionCollection());
    KStandardAction::configureToolbars(this, SLOT(optionsConfigureToolbars()), (QObject*)actionCollection());


    setStandardToolBarMenuEnabled(TRUE);
    createStandardStatusBarAction();

    actionCollection()->addAction("Create",m_actions["create"]);

    actionCollection()->addAction("backup",backupAction);


    connect(m_actions["create"], SIGNAL(triggered(bool)), SLOT(ActionCreate()));
    connect(backupAction, SIGNAL(triggered(bool)), SLOT(ActionBackup()));
}

void k9ImportFiles::setActions( bool enabled) {
        m_actions["quit"]->setEnabled(enabled);
        m_actions["preferences"]->setEnabled(enabled);
        m_actions["create"]->setEnabled(enabled);
        m_menuEdit->setEnabled(enabled);
        m_newTitle->setEnabled(enabled);
        m_chapterEdit->setEnabled(enabled);
        m_k9Import->setEnabled(enabled);

}

void k9ImportFiles::optionsConfigureKeys() {

    KShortcutsDialog::configure(actionCollection());

}

void k9ImportFiles::optionsConfigureToolbars() {

    KEditToolBar dlg(factory());
    connect(&dlg,SIGNAL(newToolbarConfig()),this,SLOT(newToolbarConfig()));
    dlg.exec();

}

void k9ImportFiles::newToolbarConfig() {
    // this slot is called when user clicks "Ok" or "Apply" in the toolbar editor.
    // recreate our GUI, and re-apply the settings (e.g. "text under icons", etc.)

    createGUI(0);


//    applyMainWindowSettings(KGlobal::config(), autoSaveGroup());
}

void k9ImportFiles::preferences() {
    k9Settings settings(k9Dialogs::getMainWidget(),i18n("Settings"));
    settings.exec();
}



void k9ImportFiles::ActionCreate() {
    m_k9Import->execute();
}

void k9ImportFiles::ActionBackup() {
    m_parent->show();
    k9Dialogs::setMainWidget(m_parent);
    hide();
/*   
*/ 

}
void k9ImportFiles::changeStatusbar(const QString& text,int id) {
    // display the text on the statusbar

#if QT_VERSION >= 0x050000
#else
    statusBar()->changeItem(text,id);
#endif

}

void k9ImportFiles::changeCaption(const QString& text) {
    // display the text on the caption
#if QT_VERSION >= 0x050000
    setWindowTitle((text));
#else
    setWindowTitle(KDialog::makeStandardCaption(text));
#endif
}


void k9ImportFiles::quit() {
    close( );
    qApp->quit();
}


void k9ImportFiles::closeEvent( QCloseEvent* ce ) {
    k9Config config;
    if (config.getPrefDelTmpFiles())
        k9Tools::clearOutput(config.getPrefOutput()+"/dvd");
    ce->accept();    
    m_parent->show();
}

void k9ImportFiles::setEnabledCreateDVD(bool _state) {
	m_actions["create"]->setEnabled(_state);
}

