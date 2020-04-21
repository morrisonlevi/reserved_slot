/* reserved_slot extension for PHP (c) 2020 Levi Morrison <levim@php.net> */

#include "php_reserved_slot.h"

#include <php_version.h>
#include <Zend/zend_extensions.h>
#include <Zend/zend_smart_str.h>
#include <main/php.h>

#if PHP_VERSION_ID < 70400
#error "This extension requires PHP 7.4 or above"
#endif

ZEND_API zend_extension_version_info extension_version_info = {
	ZEND_EXTENSION_API_NO,
	ZEND_EXTENSION_BUILD_ID
};

int reserved_slot_resource = -1;
union reserved_slot_cache {
	uintptr_t num;
	void *ptr;
};
typedef union reserved_slot_cache reserved_slot_cache;

#if __STDC_VERSION__ >= 201112L
_Static_assert(sizeof(reserved_slot_cache) == sizeof(void*),
	"The size of the reserved_slot_cache does not match a pointer");
#endif

int reserved_slot_startup(zend_extension *extension) {
	reserved_slot_resource = zend_get_op_array_extension_handle();
	return SUCCESS;
}

void reserved_slot_activate(void) {
	CG(compiler_options) |= ZEND_COMPILE_EXTENDED_FCALL;
}

void reserved_slot_startup_fcall_begin_handler(zend_execute_data *ex) {
	zend_function *fbc;
	if (!ex || !ex->call || !(fbc = ex->call->func)) {
		return;
	}
	if (fbc->common.fn_flags & ZEND_ACC_IMMUTABLE) {
		php_printf("Function %s is immutable\n", ZSTR_VAL(fbc->common.function_name));
	}
	if (fbc->type == ZEND_USER_FUNCTION) {
		reserved_slot_cache cache = {
			.ptr = ZEND_OP_ARRAY_EXTENSION(&fbc->op_array, reserved_slot_resource),
		};
		php_printf("Call %lu to %s\n", cache.num, ZSTR_VAL(fbc->common.function_name));
		cache.num += 1;
		ZEND_OP_ARRAY_EXTENSION(&fbc->op_array, reserved_slot_resource) = cache.ptr;
	}
}

ZEND_API zend_extension zend_extension_entry = {
	"reserved_slot",
	PHP_RESERVED_SLOT_VERSION,
	"Levi Morrison",
	"https://github.com/morrisonlevi/reserved_slot",
	"Copyright 2020",
	reserved_slot_startup,
	NULL,
	reserved_slot_activate,
	NULL,
	NULL,
	NULL,
	NULL,
	reserved_slot_startup_fcall_begin_handler,
	NULL,
	NULL,
	NULL,
	STANDARD_ZEND_EXTENSION_PROPERTIES
};

