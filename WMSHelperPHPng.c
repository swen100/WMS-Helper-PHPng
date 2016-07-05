/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2016 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_WMSHelperPHPng.h"

/* If you declare any globals in php_WMSHelperPHPng.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(WMSHelperPHPng)
*/

/* True global resources - no need for thread safety here */
static int le_WMSHelperPHPng;

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("WMSHelperPHPng.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_WMSHelperPHPng_globals, WMSHelperPHPng_globals)
    STD_PHP_INI_ENTRY("WMSHelperPHPng.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_WMSHelperPHPng_globals, WMSHelperPHPng_globals)
PHP_INI_END()
*/
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_WMSHelperPHPng_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_WMSHelperPHPng_compiled)
{
	char *arg = NULL;
	size_t arg_len, len;
	zend_string *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	strg = strpprintf(0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "WMSHelperPHPng", arg);

	RETURN_STR(strg);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and
   unfold functions in source code. See the corresponding marks just before
   function definition, where the functions purpose is also documented. Please
   follow this convention for the convenience of others editing your code.
*/


/* {{{ php_WMSHelperPHPng_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_WMSHelperPHPng_init_globals(zend_WMSHelperPHPng_globals *WMSHelperPHPng_globals)
{
	WMSHelperPHPng_globals->global_value = 0;
	WMSHelperPHPng_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(WMSHelperPHPng)
{
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(WMSHelperPHPng)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(WMSHelperPHPng)
{
#if defined(COMPILE_DL_WMSHELPERPHPNG) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(WMSHelperPHPng)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(WMSHelperPHPng)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "WMSHelperPHPng support", "enabled");
        php_info_print_table_row(2, "WMS-Helper-PHPng version", "1.0 alpha");
        php_info_print_table_row(2, "Author", "Swen Zanon");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ WMSHelperPHPng_functions[]
 *
 * Every user visible function must have an entry in WMSHelperPHPng_functions[].
 */
const zend_function_entry WMSHelperPHPng_functions[] = {
	ZEND_FE(confirm_WMSHelperPHPng_compiled,	NULL)		/* For testing, remove later. */
        ZEND_FE(coord2pix, NULL)
        //ZEND_FE(coords2pix, NULL)
        //ZEND_FE(points2pix, NULL)
	ZEND_FE_END	/* Must be the last line in WMSHelperPHPng_functions[] */
};
/* }}} */

/* {{{ WMSHelperPHPng_module_entry
 */
zend_module_entry WMSHelperPHPng_module_entry = {
	STANDARD_MODULE_HEADER,
	"WMSHelperPHPng",
	WMSHelperPHPng_functions,
	ZEND_MODULE_STARTUP_N(WMSHelperPHPng),
	ZEND_MODULE_SHUTDOWN_N(WMSHelperPHPng),
	ZEND_MODULE_ACTIVATE_N(WMSHelperPHPng),		/* Replace with NULL if there's nothing to do at request start */
	ZEND_MODULE_DEACTIVATE_N(WMSHelperPHPng),	/* Replace with NULL if there's nothing to do at request end */
	ZEND_MODULE_INFO_N(WMSHelperPHPng),
	PHP_WMSHELPERPHPNG_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_WMSHELPERPHPNG
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(WMSHelperPHPng)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

        /**
 * 
 * @param ht
 * @param return_value
 * @param return_value_ptr
 * @param this_ptr
 * @param return_value_used
 */
ZEND_FUNCTION(coord2pix) {
    
    zval xy_str_p, delimiter, xy_arr_p, return_value;
    double minX, minY, resX, resY;
    
    //MAKE_STD_ZVAL(xy_str_p);
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zdddd", &xy_str_p, &minX, &minY, &resX, &resY) == FAILURE) {
        RETURN_NULL();
    }
    
    //ALLOC_INIT_ZVAL(delimiter);
    ZVAL_STRING(&delimiter, " ");
    array_init(return_value);
    
    //ALLOC_INIT_ZVAL(xy_arr_p);
    array_init(xy_arr_p);

    php_explode(delimiter, xy_str_p, xy_arr_p, 1);
    
    //if (Z_TYPE_P(xy_arr_p) == IS_ARRAY) {
    //    add_next_index_zval(return_value, coord2pix_static(xy_arr_p, minX, minY, resX, resY) );
    //}
    
    RETURN_ARR(xy_arr_p);
}
