/*
 * This file is part of Pwxe, a PHP extension to stop execution
 * of writable files.
 *
 * Author: Varga Bence <vbence@czentral.org>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "php_pwxe.h"
#include "SAPI.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

typedef zend_op_array* (zend_compile_t)(zend_file_handle*, int TSRMLS_DC);
static zend_compile_t *old_compile_file;

static zend_function_entry pwxe_functions[] = {
    {NULL, NULL, NULL}
};

zend_module_entry pwxe_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    PHP_PWXE_EXTNAME,
    pwxe_functions,
    PHP_MINIT(pwxe),
    PHP_MSHUTDOWN(pwxe),
    PHP_RINIT(pwxe),
    PHP_RSHUTDOWN(pwxe),    
    PHP_MINFO(pwxe),
#if ZEND_MODULE_API_NO >= 20010901
    PHP_PWXE_VERSION,
#endif
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_PWXE
ZEND_GET_MODULE(pwxe)
#endif

PHP_INI_BEGIN()
    PHP_INI_ENTRY("pwxe.filter", "1", PHP_INI_SYSTEM, NULL)
PHP_INI_END()

PHP_MINFO_FUNCTION(pwxe)
{
    php_info_print_table_start();
    php_info_print_table_row(2, "PHP Write Xor Execute Extension", "enabled");
    php_info_print_table_row(2, "Version", PHP_PWXE_VERSION);
    php_info_print_table_end();
    
    DISPLAY_INI_ENTRIES();
} 

PHP_MINIT_FUNCTION(pwxe)
{
    REGISTER_INI_ENTRIES();

    pwxe_zend_init();

    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(pwxe)
{
    pwxe_zend_shutdown();

    UNREGISTER_INI_ENTRIES();

    return SUCCESS;
}

PHP_RINIT_FUNCTION(pwxe)
{    
    return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(pwxe)
{

    return SUCCESS;
}

/*
 * Decide whether it is safe to run this file.
 */
static bool file_runnable(const char* filename) {

    // file is writable
    int writable = access(filename, W_OK);
    if (writable == 0) {
        return false;
    }

    // file is owned by current (effective) user
    struct stat fileinfo;
    stat(filename, &fileinfo);
    uid_t euid = geteuid();
    if (fileinfo.st_uid == euid) {
        return false;
    }

    // all OK
    return true;
}


/*
 * We hook into zend_compile_file to abort file compilation if necessary.
 */
static zend_op_array* my_compile_file(zend_file_handle* h, int type TSRMLS_DC)
{
    bool file_ok = INI_BOOL("pwxe.filter") ? file_runnable(h->filename) : true;
    if (file_ok) {
        return old_compile_file(h, type TSRMLS_CC);
    } else {
        zend_error(E_COMPILE_ERROR, "Writable (or owned) file blocked by PWXE: \"%s\".", h->filename);
        return FAILURE;
    }
}

/*
 * Set up hooks.
 */
void pwxe_zend_init(TSRMLS_D) {
    old_compile_file = zend_compile_file;
    zend_compile_file = my_compile_file;
}

/*
 * Remove hooks.
 */
void pwxe_zend_shutdown(TSRMLS_D)
{
    zend_compile_file = old_compile_file;
}
