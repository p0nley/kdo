/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php_tl_toolkit.h"
#include "zend_API.h"
#include "ext/standard/md5.h"

#define PHP_TL_AUTHCODE_DEFAULT_OP   "DECODE"
#define PHP_TL_AUTHCODE_DEFAULT_KEY  "6713wj3NZqPxPILb7MyzF2nFGc3DNoSW9yWMRA"
#define PHP_TL_AUTHCODE_CKEY_LENGTH  4

/* {{{ tl_md5
 */
zend_string *tl_md5(zend_string *str,zend_bool raw_output)
{
    zend_string *result;
    char md5str[33];
    PHP_MD5_CTX context;
    unsigned char digest[16];

    md5str[0] = '\0';
    PHP_MD5Init(&context);
    PHP_MD5Update(&context, ZSTR_VAL(str), ZSTR_LEN(str));
    PHP_MD5Final(digest, &context);
    if (raw_output) {
        return zend_string_init((char *) digest, 16,0);
    } else {
        make_digest_ex(md5str, digest, 16);
       return zend_string_init(md5str,33,0);
    }
}
/* }}} */

/** {{{ tl_concat_function
 */
zend_string *tl_concat_function()
{

}
/* }}} */

/*{{ tl_authcode
 */
PHP_FUNCTION(tl_authcode)
{
    zend_string *input;
    zend_string *operate = zend_string_init(PHP_TL_AUTHCODE_DEFAULT_OP, sizeof(PHP_TL_AUTHCODE_DEFAULT_OP) - 1, 0);
    zend_string *key = zend_string_init(PHP_TL_AUTHCODE_DEFAULT_KEY, sizeof(PHP_TL_AUTHCODE_DEFAULT_KEY) - 1, 0);
    zend_long expiry = 0;
    zend_string *output = NULL;

    ZEND_PARSE_PARAMETERS_START(1, 4)
        Z_PARAM_STR(input)
        Z_PARAM_OPTIONAL
        Z_PARAM_STR(operate)
        Z_PARAM_STR(key)
        Z_PARAM_LONG(expiry)
    ZEND_PARSE_PARAMETERS_END();

    key = tl_md5(key,0);
    zend_string *key_a = tl_md5(zend_string_init(ZSTR_VAL(key),16,0),0);
    zend_string *key_b = tl_md5(zend_string_init(ZSTR_VAL(key) + 16,16,0),0);
    zend_string *key_c;
    if(strcmp(ZSTR_VAL(operate), PHP_TL_AUTHCODE_DEFAULT_OP) == 0){
        key_c = zend_string_init(ZSTR_VAL(input),PHP_TL_AUTHCODE_CKEY_LENGTH,0);
    }else{
        zval microtime;
        zval funcname;
        ZVAL_STRING(&funcname,"microtime");
        call_user_function(CG(function_table), NULL, &funcname, &microtime, 0, NULL);
        zend_string *md5microtime = tl_md5(zend_string_init(Z_STRVAL(microtime),Z_STRLEN(microtime),0),0);
        key_c = zend_string_init(
                ZSTR_VAL(md5microtime)+(ZSTR_LEN(md5microtime)-PHP_TL_AUTHCODE_CKEY_LENGTH),
                PHP_TL_AUTHCODE_CKEY_LENGTH,
                0
                );

    }
    zval keyac,keya_md5keyac;
    zval z_key_a,z_key_c,z_md5keyac;
    ZVAL_STR_COPY(&z_key_a, key_a);
    ZVAL_STR_COPY(&z_key_c, key_c);
    concat_function(&keyac, &z_key_a, &z_key_c);
    zend_string *md5keyac = tl_md5(zend_string_init(Z_STRVAL(keyac),Z_STRLEN(keyac),0),0);
    ZVAL_STR_COPY(&z_md5keyac,md5keyac);
    concat_function(&keya_md5keyac,&z_key_a,&z_md5keyac);
    zend_string *cryptkey = Z_STR(keya_md5keyac);
    RETURN_STR(cryptkey);
}
/* }}} */
