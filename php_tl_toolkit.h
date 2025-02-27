/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2015 The PHP Group                                |
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

#ifndef PHP_TL_TOOLKIT_H
#define PHP_TL_TOOLKIT_H

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"

extern zend_module_entry tl_toolkit_module_entry;
#define phpext_tl_toolkit_ptr &tl_toolkit_module_entry

#define PHP_TL_TOOLKIT_VERSION "1.7.1" /* Replace with version number for your extension */

#ifdef PHP_WIN32
#	define PHP_TL_TOOLKIT_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_TL_TOOLKIT_API __attribute__ ((visibility("default")))
#else
#	define PHP_TL_TOOLKIT_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif


ZEND_BEGIN_MODULE_GLOBALS(kdo)
	zend_long  expiry;
	zend_long  salt_length;
	char *private_key;
ZEND_END_MODULE_GLOBALS(kdo)

/* Always refer to the globals in your function as TL_TOOLKIT_G(variable).
   You are encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/
#define TL_TOOLKIT_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(kdo, v)

#if defined(ZTS) && defined(COMPILE_DL_TL_TOOLKIT)
ZEND_TSRMLS_CACHE_EXTERN();
#endif


PHP_FUNCTION(kdo_info);
PHP_FUNCTION(kdo_auth);
PHP_FUNCTION(kdo_get_arch);

#endif	/* PHP_TL_TOOLKIT_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
