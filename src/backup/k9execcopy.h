//
// C++ Interface: k9execcopy
//
// Description:
//
// Developer: Pauline123 <pauline123@users.sf.net>, (C) 2014
//
// Author:  <>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef K9EXECCOPY
#define K9EXECCOPY
#include "k9dvd.h"
#include <QObject>



#if QT_VERSION >= 0x050000
#include <QProgressDialog>
#else
#include <KProgressDialog>
#endif




class k9ResultingFile;
class k9ExecCopy:public QObject  {
Q_OBJECT
public:
    enum eOutput {oDVD,oISO,oFolder};

    k9ExecCopy();
    void CreateMP4();
    void extractMPEG2();
    void extractAudio();
    void setDvd(k9DVD* theValue) {
        m_dvd = theValue;
    }

    void setOutput(const eOutput& theValue) {
        m_output = theValue;
    }

    void setCopyMenus(bool theValue) {
        m_copyMenus = theValue;
    }

    void setPath(const QString& theValue) {
        m_path = theValue;
    }

    void setSpeed(const QString& theValue);
	
    bool isCancelled() {
        return m_cancelled;
    }
    
    void setDvdSize( int theValue) {
      m_dvdSize=theValue;
    }
private:
    k9DVD *m_dvd;
    bool m_cancelled;
    QString m_speed;
    QString m_path;
    eOutput m_output;
    bool m_copyMenus;
#if QT_VERSION >= 0x050000
    QProgressDialog *m_progressDialog;
#else
    KProgressDialog *m_progressDialog;
#endif
    void createMkv(k9DVDTitle *_title,const QString &_filename,QMultiMap< int, k9ResultingFile* > _files,int _nbParts, const QString &_aspect) ;
    QStringList m_outputMsg;
    int m_mkvPos;
    int m_dvdSize;
protected slots:
    void MkvStdout(char *, int );
    void finished(int, QProcess::ExitStatus);
public slots:
    void copyDVD();
};



#endif
