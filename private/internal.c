#include <mimas/mimas.h>
#include <internal.h>

#include <stdlib.h>
#include <string.h>

static Mimas_Internal* _mimas = NULL;

void _mimas_init_internal(Mimas_Backend const backend) {
    _mimas = (Mimas_Internal*)malloc(sizeof(Mimas_Internal));
    memset(_mimas, 0, sizeof(Mimas_Internal));
    _mimas->backend = backend;
}

void _mimas_terminate_internal(void) {
    free(_mimas);
    _mimas = NULL;
}

mimas_bool _mimas_is_initialized(void) {
    return _mimas != NULL;
}

Mimas_Internal* _mimas_get_mimas_internal(void) {
    return _mimas;
}
