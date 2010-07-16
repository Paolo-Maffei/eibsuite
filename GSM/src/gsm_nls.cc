#ifdef HAVE_CONFIG_H
#include <gsm_config.h>
#endif
#include <gsm_nls.h>
#include <string>

using namespace std;

#ifdef ENABLE_NLS

using namespace gsmlib;

#ifdef HAVE_LOCALE_H
#include <locale.h>
#endif

bool InitNLS::initialized = false;

#endif // ENABLE_NLS
