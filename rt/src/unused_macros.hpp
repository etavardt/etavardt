#pragma once

// TCE: this is good to keep.
#ifdef __GNUC__
// __attribute__((unused)) may only be gcc usable and it clutters up the code and is just as
// likely to be ignored if a developer comes along and add usage of a parameter I like the
// following define better because it can cover more compilers and doesn't clutter the code.
// It will produde a var not declared in this scope indicating to the dev that the macro is
// no longer needed and should be removed.
//
// Only use as a last resort, removal of the var declaration is prefered.
//
// Usage: void foo(Bar UNUSED_VAR(bar)) { ... }
#  define UNUSED_VAR(x) UNUSED_ ## x __attribute__((__unused__))
#else
// __attribute__((unused)) may only be gcc usable and it clutters up the code and is just as
// likely to be ignored if a developer comes along and add usage of a parameter I like the
// following define better because it can cover more compilers and doesn't clutter the code.
// It will produde a var not declared in this scope indicating to the dev that the macro is
// no longer needed and should be removed.
//
// Only use as a last resort, removal of the var declaration is prefered.
//
// Usage: void foo(Bar UNUSED_VAR(bar)) { ... }
#  define UNUSED_VAR(x) UNUSED_ ## x
#endif

#ifdef __GNUC__
// __attribute__((unused)) may only be gcc usable and it clutters up the code and is just as
// likely to be ignored if a developer comes along and add usage of a parameter I like the
// following define better because it can cover more compilers and doesn't clutter the code.
// It will produde a var not declared in this scope indicating to the dev that the macro is
// no longer needed and should be removed.
//
// Only use as a last resort, removal of the var declaration is prefered.
//
// Usage: void UNUSED_FUNCTION(foo)(int bar) { ... }
#  define UNUSED_FUNCTION(x) __attribute__((__unused__)) UNUSED_ ## x
#else
// __attribute__((unused)) may only be gcc usable and it clutters up the code and is just as
// likely to be ignored if a developer comes along and add usage of a parameter I like the
// following define better because it can cover more compilers and doesn't clutter the code.
// It will produde a var not declared in this scope indicating to the dev that the macro is
// no longer needed and should be removed.
//
// Only use as a last resort, removal of the var declaration is prefered.
//
// Usage: void UNUSED_FUNCTION(foo)(int bar) { ... }
#  define UNUSED_FUNCTION(x) UNUSED_ ## x
#endif
