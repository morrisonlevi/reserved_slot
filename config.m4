dnl config.m4 for extension reserved_slot

PHP_ARG_ENABLE([reserved_slot],
  [whether to enable reserved_slot support],
  [AS_HELP_STRING([--enable-reserved_slot],
    [Enable reserved_slot support])],
  [no])

if test "$PHP_RESERVED_SLOT" != "no"; then
  AC_DEFINE(HAVE_RESERVED_SLOT, 1, [ Have reserved_slot support ])
  PHP_NEW_EXTENSION(reserved_slot, reserved_slot.c, $ext_shared)
fi

