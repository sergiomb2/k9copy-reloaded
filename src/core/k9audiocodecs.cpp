//
// C++ Implementation: k9audiocodecs
//
// Description:
//
// Developer: Pauline123 <pauline123@users.sf.net>, (C) 2014
//
// Author: Jean-Michel PETIT <k9copy@free.fr>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "k9audiocodecs.h"

#include <QStringList>

#include <KConfigGroup>

class _k9AudioCodec {
public:
    _k9AudioCodec():name(""),options(""),encoder(""),extension("") {}
    _k9AudioCodec(QString _name,QString _options,QString _encoder,QString _extension="") {
        name=_name;
        options=_options;
	encoder=_encoder;
	extension=_extension;
    }
    QString name;
    QString options;
    QString encoder;
    QString extension;
};


k9AudioCodecs::k9AudioCodecs(QObject *parent)
        : QObject(parent) {
    m_config=new k9Config();

    KConfig *config= m_config->getConfig();
    KConfigGroup group=config->group("codecs");
    int nbAudio=group.readEntry("AudioCount",0);


    //adds default codecs
    if (nbAudio==0) {
        reset();
        m_config=new k9Config();
    }

    for (int cpt=0;cpt <nbAudio;cpt++) {
	group=config->group(QString("audiocodec%1").arg(cpt));
	m_codecs[cpt] = _k9AudioCodec(group.readEntry("name"),group.readEntry("opt",""),group.readEntry("encoder","mencoder"),group.readEntry("extension"));

    }

    delete m_config;

}

void k9AudioCodecs::reset() {
    m_codecs[0]=_k9AudioCodec("copy","-oac copy","mencoder");
    m_codecs[1]=_k9AudioCodec("mp2","-oac lavc -lavcopts acodec=mp2:abitrate=$AUDBR","mencoder");
    m_codecs[2]=_k9AudioCodec("ac3","-oac lavc -lavcopts acodec=ac3:abitrate=$AUDBR","mencoder");
    m_codecs[3]=_k9AudioCodec("IMA Adaptive PCM","-oac lavc -lavcopts acodec=adpcm_ima_wav:abitrate=$AUDBR","mencoder");
    m_codecs[4]=_k9AudioCodec("sonic","-oac lavc -lavcopts acodec=sonic:abitrate=$AUDBR","mencoder");
    m_codecs[5]=_k9AudioCodec("aac","-oac faac -faacopts br=$AUDBR","mencoder");
    m_codecs[6]=_k9AudioCodec("mp3 (lame)","-oac mp3lame -lameopts abr:br=$AUDBR","mencoder");
    m_codecs[7]=_k9AudioCodec("copy","-codec:a copy","ffmpeg","");
    m_codecs[8]=_k9AudioCodec("mp3","-codec:a libmp3lame -ab $AUDBRk -ac 2 -ar 44100","ffmpeg",".mp3");
    m_codecs[9]=_k9AudioCodec("vorbis","-codec:a vorbis -ab $AUDBRk -ac 2 -ar 44100","ffmpeg",".ogg");
    m_codecs[10]=_k9AudioCodec("aac","-codec:a libfaac -ab $AUDBRk -ac 2 -ar 44100","ffmpeg",".aac");

    save();
}

void k9AudioCodecs::save() {
    m_config=new k9Config();

    KConfig *config= m_config->getConfig();
    KConfigGroup group;

    
    int cpt=0;
    for (QMap<int,_k9AudioCodec>::iterator i=m_codecs.begin();i!=m_codecs.end();++i) {
	group=config->group(QString("audiocodec%1").arg(cpt));
	group.writeEntry("name",i.value().name);
        group.writeEntry("opt", i.value().options);
	group.writeEntry("encoder", i.value().encoder);
        group.writeEntry("extension",i.value().extension);

        cpt++;
    }
    group=config->group("codecs");
    group.writeEntry("AudioCount",cpt);

    m_config->save();

    delete m_config;
}




int k9AudioCodecs::count() {
    return m_codecs.count();
}

void k9AudioCodecs::setOptions(int _num,QString _value) {
    m_codecs[_num].options=_value;
}


void k9AudioCodecs::setEncoder(int _num,QString _value) {
   m_codecs[_num].encoder=_value;
}

void k9AudioCodecs::setCodecName(int _num,QString _value) {
    m_codecs[_num].name=_value;
}

void k9AudioCodecs::setExtension(int _num,QString _value) {
    m_codecs[_num].extension=_value;
}

QString k9AudioCodecs::getOptions(int _num) {
    return m_codecs[_num].options;
}

QString k9AudioCodecs::getCodecName(int _num) {
    return m_codecs[_num].name;
}

int k9AudioCodecs::getAbsCodecNum(int _num,const QString &_encoder) {
   k9AudioCodecs a(0);
   int nb=a.count();
   int num=0;
   for (int i=0;i<nb;i++) {
      if (a.getEncoder(i) == _encoder) {
	  if (_num==num)
	     return i;
	  else num++;

      }
   }
   return 0;
}



QString k9AudioCodecs::getEncoder(int _num) {
    return m_codecs[_num].encoder=="" ? QString("mencoder"): m_codecs[_num].encoder;

}

QString k9AudioCodecs::getExtension(int _num) {
    return m_codecs[_num].extension;

}

void k9AudioCodecs::remove(int _num) {
    int nb=count();
    if (nb>0) {
        for (int i=_num;i<nb-1;i++) {
            m_codecs[i]=m_codecs[i+1];
        }
        m_codecs.remove(nb-1);
    }
}

k9AudioCodecs::~k9AudioCodecs() {
}

