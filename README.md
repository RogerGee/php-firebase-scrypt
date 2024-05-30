# PHP Firebase `scrypt` extension

This project provides a PHP extension that wraps the Firebase `scrypt` functionality. The extension is compiled using the implementation in [`firebase / scrypt`](https://github.com/firebase/scrypt).

## Building

The `firebase / scrypt` repository is include as a submodule. When obtaining this project from source control, make sure to clone recursively or pull submodules after cloning:

~~~bash
# git clone --recursive <url>
~~~

The `firebase / scrypt` library must be patched using `patches/Add-Lib-To-Makefile.scrypt.patch`.

> The `firebase / scrypt` project - at this time - appears to solely be a reference implementation. To allow the project to compile to a static library, the patch alters the build system.

Run the patch at the subproject-level:

~~~bash
cd scrypt
patch < ../patches/Add-Lib-To-Makefile.scrypt.patch
~~~

Now build the `scrypt` sub-project following the instructions in `scrypt/BUILDING`:

~~~bash
autoreconf -i
./configure
make
~~~

> The `firebase / scrypt` sub-project requires OpenSSL. Make sure you have the development headers installed. If you have them installed in a non-standard location, use `CFLAGS` and `LDFLAGS` as needed to specify the location. This also applies to building the PHP extension mentioned below.

Now you are ready to build the PHP extension. If you haven't already, run `phpize` to create the required PHP build system. (You only have to do this once.)

~~~bash
phpize
~~~

Now you can configure the PHP extension project and build:

~~~bash
./configure

# If you need to specify another PHP (not in PATH):
./configure --with-php-config="./path/to/php-config"

make
~~~

## API Documentation

### `firebase_scrypt`

Hashes a password using the Firebase `scrypt` algorithm.

#### Signature

	string firebase_scrypt(string $passwd,string $salt,string $key,string $salt_separator,int $rounds = 8,int $memcost = 14)

#### Parameters

| Name             | Type     | Meaning                               | Optional? |
| ---------------- | -------- | ------------------------------------- | --------- |
| `passwd`         | `string` | The password to hash                  | no        |
| `salt`           | `string` | The password salt to use in the hash  | no        |
| `key`            | `string` | The encryption key to use in the hash | no        |
| `salt_separator` | `string` | The salt separator to use in the hash | no        |
| `rounds`         | `int`    | The rounds parameter                  | yes       |
| `memcost`        | `int`    | The mem_cost parameter                | yes       |

> Note: many of these parameters correspond to the Firebase project's hash parameters. See [the README](https://github.com/firebase/scrypt/blob/master/README.md) of the `firebase / scrypt` project for more.

### `firebase_scrypt_verify`

Verifies a password against an existing password hash using the Firebase `scrypt` algorithm.

This function is functionally equivalent to:

~~~php
function firebase_scrypt_verify($candidate_passwd,$hash,/* ... */)
  $candidate_hash = firebase_scrypt($candidate_passwd,/* ... */);

  return ($hash == $candidate_hash);
}
~~~

> Note that `firebase_scrypt_verify` performs a time-safe comparison operation. This method should be preferred over manually comparing password hashes.

#### Signature

	bool firebase_scrypt_verify(string $passwd,string $hash,string $salt,string $key,string $salt_separator,int $rounds = 8,int $memcost = 14)

#### Parameters

| Name             | Type     | Meaning                                             | Optional? |
| ---------------- | -------- | --------------------------------------------------- | --------- |
| `passwd`         | `string` | The password to hash                                | no        |
| `hash`           | `string` | The existing password hash against which to compare | no        |
| `salt`           | `string` | The password salt to use in the hash                | no        |
| `key`            | `string` | The encryption key to use in the hash               | no        |
| `salt_separator` | `string` | The salt separator to use in the hash               | no        |
| `rounds`         | `int`    | The rounds parameter                                | yes       |
| `memcost`        | `int`    | The mem_cost parameter                              | yes       |

> Note: many of these parameters correspond to the Firebase project's hash parameters. See [the README](https://github.com/firebase/scrypt/blob/master/README.md) of the `firebase / scrypt` project for more.
