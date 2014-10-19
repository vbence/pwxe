PHP_ARG_ENABLE(pwxe, whether to enable PXWE support,
[ --enable-pwxe   PHP Write Xor Execute support])

if test "$PHP_PWXE" = "yes"; then
  AC_DEFINE(HAVE_PWXE, 1, [Whether you have PXWE])
  PHP_NEW_EXTENSION(pwxe, src/php_pwxe.c, $ext_shared)
fi