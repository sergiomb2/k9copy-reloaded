//
// C++ Interface: k9common
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

#include "config.h"
#ifndef __STDC_LIMIT_MACROS
#endif
#ifndef __STDC_CONSTANT_MACROS
#define __STDC_CONSTANT_MACROS
#endif

#if defined(HAVE_INTTYPES_H)
    #include <inttypes.h>
#elif defined( HAVE_STDINT_H)
     #include <stdint.h>
#endif


#ifdef __FreeBSD__
    #include <osreldate.h>
#endif
#include "dvdread/ifo_types.h" //PM DVDREAD
#include "dvdread/dvd_reader.h" //PM DVDREAD
#include "dvdread/ifo_read.h" //PM DVDREAD
#include "dvdread/nav_read.h" //PM DVDREAD
#include "dvdread/nav_types.h" //PM DVDREAD

#include <QtCore>
#include <QtCore/QGlobalStatic>

#include <QObject>

#ifndef DVD_BLOCK_LEN
#define DVD_BLOCK_LEN 2048
#endif

#include "k9dvdsize.h"

#include <QtDebug>
#include "k9dialogs.h"

#include "k9log.h"
#include "k9tools.h"

#include <KDE/KLocale>
//#include <KDE/KLocalizedString>
#include <KI18n/KLocalizedString>
//#include <KDE/KLocalizedDate>

#include "k9config.h"
#include "k9process.h"



