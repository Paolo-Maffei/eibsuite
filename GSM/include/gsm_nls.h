
#ifndef GSM_NLS_H
#define GSM_NLS_H

#ifdef HAVE_CONFIG_H
#include <gsm_config.h>
#endif

#ifdef ENABLE_NLS

#ifdef HAVE_LIBINTL_H
#include <libintl.h>
#else
#include "../intl/libintl.h"
#endif
#ifdef HAVE_LOCALE_H
#include <locale.h>
#endif

#define _(String) dgettext(PACKAGE, String)

// this causes automatic NLS initialization if one file of the library
// includes gsm_nls.h

namespace gsmlib
{
  const class InitNLS
  {
  static bool initialized;
    
  public:
    InitNLS()
      {
        if (! initialized)      // do only once
        {
          setlocale(LC_ALL, "");
#ifdef LOCAL_TRANSLATIONS
          bindtextdomain(PACKAGE, "../po");
#else
          bindtextdomain(PACKAGE, LOCALEDIR);
#endif
          textdomain(PACKAGE);
          initialized = true;
        }
      }
  } initNLS;
};

#else

#define _(String) (String)

#endif // ENABLE_NLS

#define N_(String) (String)

#endif // GSM_NLS_H
