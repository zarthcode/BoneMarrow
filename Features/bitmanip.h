// Utility macros for bitwise operations

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

#define CHECK_FLAGS(var,flags) ((var & flags) == flags)
