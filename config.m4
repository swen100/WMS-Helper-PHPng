dnl $Id$

PHP_ARG_ENABLE(WMSHelperPHPng, whether to enable WMSHelperPHPng support,
[  --enable-WMSHelperPHPng           Enable WMSHelperPHPng support])

if test "$PHP_WMSHELPERPHPNG" != "no"; then
  PHP_NEW_EXTENSION(WMSHelperPHPng, WMSHelperPHPng.c, $ext_shared)
fi
