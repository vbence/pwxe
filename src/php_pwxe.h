/*
 * This file is part of Pwxe, a PHP extension to stop execution
 * of writable files.
 *
 * Author: Varga Bence <vbence@czentral.org>
 */

#ifndef PHP_PWXE_H
#define PHP_PWXE_H 1


#define PHP_PWXE_VERSION "0.1"
#define PHP_PWXE_EXTNAME "PWXE"

extern void pwxe_zend_init(TSRMLS_D);
extern void pwxe_zend_shutdown(TSRMLS_D);

PHP_MINIT_FUNCTION(pwxe);
PHP_MSHUTDOWN_FUNCTION(pwxe);
PHP_RINIT_FUNCTION(pwxe);
PHP_RSHUTDOWN_FUNCTION(pwxe);
PHP_MINFO_FUNCTION(pwxe);

extern zend_module_entry pwxe_module_entry;
#define phpext_pwxe_ptr &pwxe_module_entry


#endif
