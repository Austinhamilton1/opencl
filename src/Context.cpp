#include "Context.h"

Context::~Context() {
    clReleaseContext(context);
}