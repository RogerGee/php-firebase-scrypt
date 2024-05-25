/**
 * php-firebase-scrypt.c
 *
 * php-firebase-scrypt
 */

#include "php-firebase-scrypt.h"
#include "scrypt/config.h"
#include <scryptenc/scryptenc.h>
#include <ext/standard/info.h>

/* Module/request functions */
static PHP_MINIT_FUNCTION(firebase_scrypt);
static PHP_MINFO_FUNCTION(firebase_scrypt);
static PHP_MSHUTDOWN_FUNCTION(firebase_scrypt);
static PHP_RINIT_FUNCTION(firebase_scrypt);
static PHP_RSHUTDOWN_FUNCTION(firebase_scrypt);

/* TODO: PHP userspace functions */

/* Function entries */
static zend_function_entry php_firebase_scrypt_functions[] = {
    {NULL, NULL, NULL}
};

/* Define the module entry */
zend_module_entry firebase_scrypt_module_entry = {
    STANDARD_MODULE_HEADER,
    PHP_FIREBASE_SCRYPT_EXTNAME,
    php_firebase_scrypt_functions,
    PHP_MINIT(firebase_scrypt),
    PHP_MSHUTDOWN(firebase_scrypt),
    PHP_RINIT(firebase_scrypt),
    PHP_RSHUTDOWN(firebase_scrypt),
    PHP_MINFO(firebase_scrypt),
    PHP_FIREBASE_SCRYPT_EXTVER,
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_FIREBASE_SCRYPT
ZEND_GET_MODULE(firebase_scrypt)
#endif

PHP_MINIT_FUNCTION(firebase_scrypt)
{
    //scryptenc_buf_saltlen(NULL,0,NULL,NULL,0,NULL,0,0,0);
}

PHP_MINFO_FUNCTION(firebase_scrypt)
{
    php_info_print_table_start();
    php_info_print_table_row(2,PHP_FIREBASE_SCRYPT_EXTNAME,"enabled");
    php_info_print_table_row(2,"extension version",PHP_FIREBASE_SCRYPT_EXTVER);
    php_info_print_table_row(2,"firebase/scrypt version",PACKAGE_VERSION);
    php_info_print_table_end();
}

PHP_MSHUTDOWN_FUNCTION(firebase_scrypt)
{

}

PHP_RINIT_FUNCTION(firebase_scrypt)
{

}

PHP_RSHUTDOWN_FUNCTION(firebase_scrypt)
{

}

