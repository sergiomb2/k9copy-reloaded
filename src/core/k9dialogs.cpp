/***************************************************************************
*    Developer: Pauline123 <pauline123@users.sf.net>, (C) 2014

 *   Copyright (C) 2007 by Jean-Michel   *
 *   jm_petit@laposte.net   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "k9dialogs.h"
#include "k9common.h"
#include <QDebug>
#include <QString>
#include <KMessageBox>

#if QT_VERSION >= 0x050000
#include <QFontDialog>
#include <QFileDialog>
#include <QDialog>
#include <QMessageBox>
#else
#include <KFontDialog>
#include <KFileDialog>
#include <KDialog>
#include <KNotification>
#endif



#include <QApplication>
#include "k9tools.h"
    
#if QT_VERSION >= 0x050000
// Qt5 code
#include <QIcon>
#include <QDebug>
#else
// Qt4 code
#endif



QWidget * k9MainWidget;
void k9Dialogs::setMainWidget(QWidget *_widget) {
    k9MainWidget=_widget;
}

QWidget* k9Dialogs::getMainWidget() {
    return k9MainWidget;
}



QString k9Dialogs::getOpenFileName (const QString &_initialDir,const QString &_filter,QWidget *_parent,const QString &_title) {
#if QT_VERSION >= 0x050000
        return QFileDialog::getOpenFileName ( _parent,_title,_initialDir,_filter);
#else // Qt4 Code
        return KFileDialog::getOpenFileName (_initialDir,_filter, _parent,_title);
#endif 

}

QString k9Dialogs::getSaveFileName (const QString &_initialDir,const QString &_filter,QWidget *_parent,const QString &_title) {
#if QT_VERSION >= 0x050000
        return QFileDialog::getSaveFileName ( _parent,_title,_initialDir,_filter);
#else // Qt4 Code
        return KFileDialog::getSaveFileName (_initialDir,_filter, _parent,_title);

#endif
}

QString k9Dialogs::getExistingDirectory (const QString &_initialDir) {
#if QT_VERSION >= 0x050000
        return QFileDialog::getExistingDirectory(0,i18n("select a directory"),_initialDir);
#else // Qt4 KDE4 Code
        return KFileDialog::getExistingDirectory (_initialDir);

#endif
}

void k9Dialogs::error(const QString &_message,const QString &_title,const QStringList &_detail) {
    if (k9Tools::getBatchCopy()) {
        k9log::writeln(QString("Error : %1\n%2").arg(_message).arg(_detail.join("\n")));
        return;
    }

    QWidget *activeWindow=QApplication::activeWindow();
    if (activeWindow==0)
	activeWindow=k9MainWidget;

    QStringList body;
    body << "<b>"+_message +"</b>" << "" ;
    body << _detail;


#if QT_VERSION >= 0x050000
    QMessageBox message(QMessageBox::Critical,_title,_message,QMessageBox::Ok,activeWindow) ;
    message.exec();
#else
    KNotification *notify=KNotification::event(KNotification::Error,_title,body.join("<br>"),KIcon("k9copy").pixmap(48));

    notify->setText(body.join("<br>"));
    notify->setTitle(_title);
    notify->sendEvent();
    KMessageBox::errorList(activeWindow,_message,_detail,_title);

#endif
}

void k9Dialogs::information(const QString &_message, const QString &_title) {
    if (k9Tools::getBatchCopy()) {
        k9log::writeln(QString("Information : %1").arg(_message));
        return;
    }

    QWidget *activeWindow=QApplication::activeWindow();
    if (activeWindow==0)
	activeWindow=k9MainWidget;
#if QT_VERSION >= 0x050000
    QMessageBox message(QMessageBox::Information,_title, _message, 0, activeWindow);
    message.exec();
#else
    KNotification *notify=KNotification::event(KNotification::Notification ,_title,_message,KIcon("k9copy").pixmap(48));
    notify->sendEvent();
    notify->activate();

    KMessageBox::information(activeWindow,_message,_title);
#endif

}
int k9Dialogs::warningContinueCancel(const QString &  _message,const QString & _title,const QStringList &_detail) {
    QWidget *activeWindow=QApplication::activeWindow();
    if (activeWindow==0)
	activeWindow=k9MainWidget;

#if QT_VERSION >= 0x050000
    	
    QMessageBox message(QMessageBox::Warning,_title,_message,QMessageBox::Ignore | QMessageBox::Cancel,activeWindow);
    message.setButtonText(QMessageBox::Ignore, i18n("Continue"));
    int ret = message.exec();
    if (ret == QMessageBox::Ignore) // Continue is 5 in KDE land... this Ignore must be 5, and Cancel is 1
    {qDebug() << "Continue Clicked";
    qDebug() << ret;
    return 5;}
    else
    {qDebug() << "Cancel Clicked";
    qDebug() << ret;
    return 2;}
#else
    return KMessageBox::warningContinueCancelList(activeWindow,_message,_detail,_title);
#endif


}

bool k9Dialogs::getFont(QFont &font) {
    #if QT_VERSION >= 0x050000
    bool ok;
    QFont res=QFontDialog::getFont ( &ok,font,k9MainWidget);
    if (ok) {
        font=res;
        return true;
    } else
	return false;
    #else
    int result= KFontDialog::getFont( font );
    if (result==KFontDialog::Accepted)
        return true;
    else
        return false;
    #endif

}
