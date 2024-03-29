/*
  +----------------------------------------------------------------------+
  | PHP Version 8                                                        |
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
  | Author: Swen Zanon                                                   |
  +----------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <php.h>
#include <php_ini.h>
#include <ext/standard/info.h>
#include <zend_string.h>
#include <ext/standard/php_string.h>
#include <php_WMSHelperPHPng.h>
//#include "ext/standard/php_string.h"

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

/* {{{ proto string tellMeWhatYouAre(zval *arg)
   Returns nothing */
static void tellMeWhatYouAre(zval *arg)
{
    php_printf("\nThe Element is of type: ");
    switch (Z_TYPE_P(arg)) {
    case IS_NULL:
        php_printf("NULL");
        break;
    case IS_TRUE:
    case IS_FALSE:
        php_printf("Boolean: %s", Z_LVAL_P(arg) ? "TRUE" : "FALSE");
        break;
    case IS_LONG:
        php_printf("Long: %ld", Z_LVAL_P(arg));
        break;
    case IS_DOUBLE:
        php_printf("Double: %f", Z_DVAL_P(arg));
        break;
    case IS_STRING:
        php_printf("String: ");
        PHPWRITE(Z_STRVAL_P(arg), Z_STRLEN_P(arg));
        php_printf("");
        break;
    case IS_ARRAY:
        php_printf("Array");
        break;
    default:
        php_printf("Unknown");
    }
    php_printf("\n");
}

/**
 * Function to calculate for an array with [0] (x) and [1] [y) the pixel-value
 * 
 * @param array point as array with [0] as x and [1] as y
 * @param double minX
 * @param double minY
 * @param double resX
 * @param double resY
 * @return array coord (zval)
 */
static zval /* * */coordArray2pix_static(zval *xy_arr_p, double minX, double minY, double resX, double resY )
{
    zval coord /*, *return_value*/;
    zval *x, *y;
    
    HashTable *xy_hash = Z_ARR_P(xy_arr_p);
    array_init(&coord);
    
    if (NULL != (x = zend_hash_index_find(xy_hash, 0)) && 
        NULL != (y = zend_hash_index_find(xy_hash, 1))) {
        
        convert_to_double_ex(x);
        convert_to_double_ex(y);
        
        add_index_double(&coord, 0, (Z_DVAL_P(x) - minX) * resX);
        add_index_double(&coord, 1, (Z_DVAL_P(y) - minY) * resY);
    }
    
    // how to return struct zval * instead of zval?
//    return_value = emalloc(sizeof(coord));
//    ZVAL_ZVAL(return_value, &coord, 1, 1);
//    return return_value;
    
    return coord;
}

/**
 * Function to calculate the pixel-value for a 
 * string containing value for x and y separated by space
 * 
 * @param string pointstring 
 * @param double minX
 * @param double minY
 * @param double resX
 * @param double resY
 * @return array coord (zval)
 */
static zval coordString2pix_static(zend_string *xy_str_p, double minX, double minY, double resX, double resY )
{
    zval xy_arr_p, coord;
    zend_string *delimiter;
    
    delimiter = zend_string_init(" ", 1, 0);
    array_init(&xy_arr_p);
    php_explode(delimiter, xy_str_p, &xy_arr_p, 2);
    
    if (Z_TYPE(xy_arr_p) == IS_ARRAY) {
        coord = coordArray2pix_static(&xy_arr_p, minX, minY, resX, resY);
    }
    
    // cleanup// cleanup
    zend_string_release(delimiter);
    zval_ptr_dtor(&xy_arr_p);
    
    return coord;
}

ZEND_BEGIN_ARG_INFO(coord2pix2_args, ZEND_SEND_BY_VAL)
    ZEND_ARG_INFO(0, xy_str)
    ZEND_ARG_INFO(0, minX)
    ZEND_ARG_INFO(0, minY)
    ZEND_ARG_INFO(0, resX)
    ZEND_ARG_INFO(0, resY)
ZEND_END_ARG_INFO()

/**
 * Function to calculate the pixel-value for a 
 * string, containing value for x and y separated by space
 * 
 * @param string pointstring 
 * @param double minX
 * @param double minY
 * @param double resX
 * @param double resY
 * @return array point as array with [0] as x and [1] as y
 */
PHP_FUNCTION(coord2pix2) {
    
    zval xy_arr_p, coord;
    zend_string *xy_str, *delimiter;
    double minX, minY, resX, resY;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "Sdddd", &xy_str, &minX, &minY, &resX, &resY) == FAILURE) {
        RETURN_NULL();
    }
    
    delimiter = zend_string_init(" ", 1, 0);
    
    array_init(&xy_arr_p);
    array_init(return_value);
  
    php_explode(delimiter, xy_str, &xy_arr_p, 2);

    if (Z_TYPE(xy_arr_p) == IS_ARRAY) {
        coord = coordArray2pix_static(&xy_arr_p, minX, minY, resX, resY);
        add_index_zval(return_value, 0, &coord);
    }
    
    // cleanup
    zend_string_release(delimiter);
    zval_ptr_dtor(&xy_arr_p);
}

ZEND_BEGIN_ARG_INFO(coord2pix_args, ZEND_SEND_BY_VAL)
    ZEND_ARG_INFO(0, xy_str)
    ZEND_ARG_INFO(0, minX)
    ZEND_ARG_INFO(0, minY)
    ZEND_ARG_INFO(0, resX)
    ZEND_ARG_INFO(0, resY)
ZEND_END_ARG_INFO()

/**
 * Function to calculate the pixel-value for a 
 * string, containing value for x and y separated by space.
 * This functions is slightly faster.
 * 
 * @param string pointstring 
 * @param double minX
 * @param double minY
 * @param double resX
 * @param double resY
 * @return array point as array with [0] as x and [1] as y
 */
PHP_FUNCTION(coord2pix) {
    
    zval coord;
    zend_string *xy_str;
    double minX, minY, resX, resY;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "Sdddd", &xy_str, &minX, &minY, &resX, &resY) == FAILURE) {
        RETURN_NULL();
    }
    
    array_init(return_value);
    coord = coordString2pix_static(xy_str, minX, minY, resX, resY);
    add_index_zval(return_value, 0, &coord);
}

ZEND_BEGIN_ARG_INFO(coords2pix_args, ZEND_SEND_BY_VAL)
    ZEND_ARG_INFO(0, str)
    ZEND_ARG_INFO(0, minX)
    ZEND_ARG_INFO(0, minY)
    ZEND_ARG_INFO(0, resX)
    ZEND_ARG_INFO(0, resY)
ZEND_END_ARG_INFO()

/**
 * Function to walk through an array of points and 
 * calculate for every entry the pixel-value
 * 
 * @param linestring with points containing values for x and y separated by space
 * @param double minX
 * @param double minY
 * @param double resX
 * @param double resY
 * @return array coords with array containing [0] for x and [1] for y
 */
ZEND_FUNCTION(coords2pix) {
    
    zval pts_arr;
    zend_string *delimiter, *str, *trimmed_str;
    double minX, minY, resX, resY;
    HashTable *pts_hash;
    zval coord;
    zval *zv;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "Sdddd", &str, &minX, &minY, &resX, &resY) == FAILURE) {
        RETURN_NULL();
    }
    
    delimiter = zend_string_init(",", 1, 0);
    array_init(return_value);
    array_init(&pts_arr);

    trimmed_str = php_trim(str, NULL, 0, 3);
    php_explode(delimiter, trimmed_str, &pts_arr, LONG_MAX);
    //php_explode(delimiter, Z_STR_P(str), &pts_arr, LONG_MAX);
    
    if (Z_TYPE(pts_arr) == IS_ARRAY) {
        pts_hash = Z_ARR_P(&pts_arr);
        ZEND_HASH_FOREACH_VAL(pts_hash, zv) {
            coord = coordString2pix_static(Z_STR_P(zv), minX, minY, resX, resY);
            add_next_index_zval(return_value, &coord );
//            add_next_index_zval(return_value, (void *)coordString2pix_static(Z_STR_P(zv), minX, minY, resX, resY) );
            //add_next_index_zval(return_value, (struct zval *)coord2pix_static2(Z_STR_P(zv), minX, minY, resX, resY) );
        } ZEND_HASH_FOREACH_END();
    }
    
    // cleanup
    zend_string_release(delimiter);
    zend_string_release(trimmed_str);
    zval_ptr_dtor(&pts_arr);
}


ZEND_BEGIN_ARG_INFO(points2pix_args, ZEND_SEND_BY_VAL)
    ZEND_ARG_INFO(0, pts_arr)
    ZEND_ARG_INFO(0, minX)
    ZEND_ARG_INFO(0, minY)
    ZEND_ARG_INFO(0, resX)
    ZEND_ARG_INFO(0, resY)
ZEND_END_ARG_INFO()

/**
 * Function to walk through an array of points and 
 * calculate for every entry the pixel-value
 * 
 * @param array of points with x and y separated with space
 * @param double minX
 * @param double minY
 * @param double resX
 * @param double resY
 * @return array coords with array containing [0] for x and [1] for y
 */
ZEND_FUNCTION(points2pix) {
    
    zval *pts_arr;
    double minX, minY, resX, resY;
    HashTable *pts_hash;
    zval coord;
    zval *zv;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "adddd", &pts_arr, &minX, &minY, &resX, &resY) == FAILURE) {
            return;
    }
    
    array_init(return_value);
    
    if (Z_TYPE_P(pts_arr) == IS_ARRAY) {
            pts_hash = Z_ARR_P(pts_arr);
            ZEND_HASH_FOREACH_VAL(pts_hash, zv) {
                coord = coordString2pix_static(Z_STR_P(zv), minX, minY, resX, resY);
                add_next_index_zval(return_value, &coord );
            } ZEND_HASH_FOREACH_END();
    }
}

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_WMSHelperPHPng_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirmWMSHelperCompiled)
{
    ZVAL_STRING(return_value, "Module WMSHelperPHPng is compiled into PHP.");
}

/**
 * string WMSHelperPHPngVersion()
 */
PHP_FUNCTION(getWMSHelperVersion)
{
    
    ZVAL_STRING(return_value, PHP_WMSHELPERPHPNG_VERSION );
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
        php_info_print_table_row(2, "WMS-Helper-PHPng version", "1.0.0");
        php_info_print_table_row(2, "Author", "Swen Zanon");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

ZEND_BEGIN_ARG_INFO(WMSHelperPHPng_method_no_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

/* {{{ WMSHelperPHPng_functions[]
 *
 * Every user visible function must have an entry in WMSHelperPHPng_functions[].
 */
const zend_function_entry WMSHelperPHPng_functions[] = {
        ZEND_FE(coord2pix, coord2pix_args)
        ZEND_FE(coord2pix2, coord2pix2_args)
        ZEND_FE(coords2pix, coords2pix_args)
        ZEND_FE(points2pix, points2pix_args)
        ZEND_FE(confirmWMSHelperCompiled, WMSHelperPHPng_method_no_args)
        ZEND_FE(getWMSHelperVersion, WMSHelperPHPng_method_no_args)
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
