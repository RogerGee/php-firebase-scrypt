/**
 * php-firebase-scrypt.c
 *
 * php-firebase-scrypt
 */

#include "php-firebase-scrypt.h"
#include "firebase_scrypt_arginfo.h"
#include "scrypt/config.h"
#include <scryptenc/scryptenc.h>
#include <Zend/zend_exceptions.h>
#include <ext/standard/base64.h>
#include <ext/standard/info.h>

/* Module/request functions */
static PHP_MINIT_FUNCTION(firebase_scrypt);
static PHP_MINFO_FUNCTION(firebase_scrypt);
static PHP_MSHUTDOWN_FUNCTION(firebase_scrypt);
static PHP_RINIT_FUNCTION(firebase_scrypt);
static PHP_RSHUTDOWN_FUNCTION(firebase_scrypt);

/* PHP userspace functions */
static PHP_FUNCTION(firebase_scrypt);

/* Function entries */
static zend_function_entry php_firebase_scrypt_functions[] = {
    PHP_FE(firebase_scrypt,arginfo_firebase_scrypt)
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

PHP_FUNCTION(firebase_scrypt)
{
    int result;
    uint8_t* outbuf;
    zend_string* zstr_saltbase_decoded;
    zend_string* zstr_saltsep_decoded;
    zend_string* zstr_key_decoded;
    zend_string* zstr_salt_decoded;
    zend_string* zstr_result;
    zend_string* zstr_saltsep_default = zend_string_init("Bw==",sizeof("Bw==")-1,0);

    const char* passwd;
    size_t passwd_len;
    const char* saltbase;
    size_t saltbase_len;
    const char* key;
    size_t key_len;
    zend_string* zstr_saltsep = zstr_saltsep_default;
    zend_long rounds = 8;
    zend_long memcost = 14;

    if (zend_parse_parameters(
            ZEND_NUM_ARGS(),
            "sss|Sll",
            &passwd, &passwd_len,
            &saltbase, &saltbase_len,
            &key, &key_len,
            &zstr_saltsep,
            &rounds,
            &memcost) == FAILURE)
    {
        zend_string_release(zstr_saltsep_default);
        return;
    }

    zstr_key_decoded = php_base64_decode(key,key_len);
    if (zstr_key_decoded == NULL) {
        zend_throw_error(zend_ce_value_error,"Argument 'key' is not a properly-formatted base64 string");
        return;
    }
    zstr_saltbase_decoded = php_base64_decode(saltbase,saltbase_len);
    if (zstr_saltbase_decoded == NULL) {
        zend_throw_error(zend_ce_value_error,"Argument 'salt' is not a properly-formatted base64 string");
        return;
    }
    zstr_saltsep_decoded = php_base64_decode_str(zstr_saltsep);
    if (zstr_saltsep_decoded == NULL) {
        zend_throw_error(zend_ce_value_error,"Argument 'salt_separator' is not a properly-formatted base64 string");
        return;
    }

    zstr_salt_decoded = zend_string_alloc(
        ZSTR_LEN(zstr_saltbase_decoded) + ZSTR_LEN(zstr_saltsep_decoded),
        0);
    memcpy(
        ZSTR_VAL(zstr_salt_decoded),
        ZSTR_VAL(zstr_saltbase_decoded),
        ZSTR_LEN(zstr_saltbase_decoded));
    memcpy(
        ZSTR_VAL(zstr_salt_decoded) + ZSTR_LEN(zstr_saltbase_decoded),
        ZSTR_VAL(zstr_saltsep_decoded),
        ZSTR_LEN(zstr_saltsep_decoded));
    ZSTR_VAL(zstr_salt_decoded)
        [ZSTR_LEN(zstr_saltsep_decoded) + ZSTR_LEN(zstr_saltbase_decoded)] = 0;

    outbuf = alloca(ZSTR_LEN(zstr_key_decoded) + 1);
    outbuf[ZSTR_LEN(zstr_key_decoded)] = 0;

    result = scryptenc_buf_saltlen(
        ZSTR_VAL(zstr_key_decoded),
        ZSTR_LEN(zstr_key_decoded),
        outbuf,
        passwd,
        passwd_len,
        ZSTR_VAL(zstr_salt_decoded),
        ZSTR_LEN(zstr_salt_decoded),
        (uint32_t)rounds,
        (uint32_t)memcost);

    zend_string_release(zstr_saltsep_default);
    zend_string_release(zstr_saltbase_decoded);
    zend_string_release(zstr_saltsep_decoded);
    zend_string_release(zstr_key_decoded);
    zend_string_release(zstr_salt_decoded);

    if (result != 0) {
        zend_throw_error(NULL,"firebase_scrypt: Failed to encrypt password");
        return;
    }

    zstr_result = php_base64_encode(outbuf,ZSTR_LEN(zstr_key_decoded));

    RETURN_STR(zstr_result);
}
