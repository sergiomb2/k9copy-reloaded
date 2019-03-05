//
// C++ Interface: k9copy
//
// Description:
//
// Developer: Pauline123 <pauline123@users.sf.net>, (C) 2014
//
// Author: Jean-Michel PETIT <k9copy@free.fr>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//


#ifndef _K9IMPORTFILES_H_
#define _K9IMPORTFILES_H_

#include <config.h>
#include <kxmlguiwindow.h>
#include "k9common.h"
#include "k9cddrive.h"
#include "k9copy.h"

#include <KToggleAction>
#if QT_VERSION >= 0x050000
#include <qapplication.h>
#include <QAction>
#else
#include <kapplication.h>
#include <kaction.h>
#endif

#include <QObject>

class k9Import;
class k9MenuEdit;
class k9NewTitle;
class k9ChapterEdit;
class QDockWidget;
class k9ImportFiles : public KXmlGuiWindow {
    Q_OBJECT
public:
    /**
     * Default Constructor
     */
    k9ImportFiles(k9Copy *_parent);

    void clone(QString _input,QString _output);
    void setInput(QString _input);
    void setOutput(QString _output);
    /**
     * Default Destructor
     */
    virtual ~k9ImportFiles();

protected:
    bool queryClose( );
    void closeEvent( QCloseEvent* ce );

private slots:
    void ActionCreate();
    void ActionBackup();
    void changeStatusbar(const QString& text,int id);
    void changeCaption(const QString& text);
    void optionsConfigureKeys();
    void optionsConfigureToolbars();
    void newToolbarConfig();
public slots:
    void preferences();
    void quit();
    void setActions(bool enabled);
    void setEnabledCreateDVD(bool _state);

    QDockWidget* firstBottom() const {return m_firstBottom;}
	
private:
    void setupAccel();
    void setupActions();
    void createWindows();
    void removeToolWindows();
#if QT_VERSION >= 0x050000
    QAction *CopyAction;
    QAction *PlayTitleAction;
    QAction *mkMP4Action;
    QAction *ejectAction;
    QAction *backupAction;
#else
    KAction *CopyAction;
    KAction *PlayTitleAction;
    KAction *mkMP4Action;
    KAction *ejectAction;
    KAction *backupAction;
#endif
    QDockWidget *m_firstBottom;
    k9Copy *m_parent;
private:
    k9MenuEdit *m_menuEdit;
    k9NewTitle *m_newTitle;
    k9ChapterEdit *m_chapterEdit;
    k9Import *m_k9Import;
    QMap  <QString,QAction*> m_actions; 
    //QPtrList <KMdiToolViewAccessor> m_ToolViews;
    k9CdDrives *m_drives;
};



#endif
