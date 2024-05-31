PHP_ARG_ENABLE(firebase_scrypt,[Whether to enable the "firebase_scrypt" extension],
    [ --enable-firebase-scrypt      Enable "firebase_scrypt" extension support])

if test PHP_FIREBASE_SCRYPT != "no"; then
    CFLAGS="$CFLAGS -I./scrypt/lib"
    LDFLAGS="$LDFLAGS -L./scrypt"

    AC_SEARCH_LIBS([scryptenc_buf_saltlen],[scrypt],[],[AC_MSG_ERROR([Aborting since libscrypt.a was not found: did you build the scrypt submodule?])],[-lcrypto -lcperciva_aesni -lscrypt_sse2])
    AC_CHECK_HEADERS([scryptenc/scryptenc.h],[],[AC_MSG_ERROR([Aborting since scryptenc/scryptenc.h was not found],[1])])

    LDFLAGS="$LDFLAGS -lscrypt -lcrypto -lcperciva_aesni -lscrypt_sse2"

    # Add PHP_RPATHS to extension build via EXTRA_LDFLAGS.
    if test $PHP_RPATHS != ""; then
        PHP_UTILIZE_RPATHS()
        EXTRA_LDFLAGS="$PHP_RPATHS"
        PHP_SUBST([EXTRA_LDFLAGS])
    fi

    PHP_SUBST(FIREBASE_SCRYPT_LIBADD)
    PHP_NEW_EXTENSION(firebase_scrypt,php-firebase-scrypt.c,$ext_shared)
fi
