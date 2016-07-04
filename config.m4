dnl $Id$
dnl config.m4 for extension WMSHelperPHPng

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(WMSHelperPHPng, for WMSHelperPHPng support,
dnl Make sure that the comment is aligned:
dnl [  --with-WMSHelperPHPng             Include WMSHelperPHPng support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(WMSHelperPHPng, whether to enable WMSHelperPHPng support,
dnl Make sure that the comment is aligned:
dnl [  --enable-WMSHelperPHPng           Enable WMSHelperPHPng support])

if test "$PHP_WMSHELPERPHPNG" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-WMSHelperPHPng -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/WMSHelperPHPng.h"  # you most likely want to change this
  dnl if test -r $PHP_WMSHELPERPHPNG/$SEARCH_FOR; then # path given as parameter
  dnl   WMSHELPERPHPNG_DIR=$PHP_WMSHELPERPHPNG
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for WMSHelperPHPng files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       WMSHELPERPHPNG_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$WMSHELPERPHPNG_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the WMSHelperPHPng distribution])
  dnl fi

  dnl # --with-WMSHelperPHPng -> add include path
  dnl PHP_ADD_INCLUDE($WMSHELPERPHPNG_DIR/include)

  dnl # --with-WMSHelperPHPng -> check for lib and symbol presence
  dnl LIBNAME=WMSHelperPHPng # you may want to change this
  dnl LIBSYMBOL=WMSHelperPHPng # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $WMSHELPERPHPNG_DIR/$PHP_LIBDIR, WMSHELPERPHPNG_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_WMSHELPERPHPNGLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong WMSHelperPHPng lib version or lib not found])
  dnl ],[
  dnl   -L$WMSHELPERPHPNG_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(WMSHELPERPHPNG_SHARED_LIBADD)

  PHP_NEW_EXTENSION(WMSHelperPHPng, WMSHelperPHPng.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
