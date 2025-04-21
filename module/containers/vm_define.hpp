#define VM_COMMAND_DEFINE_UINT(name) name##u8_, name##u16, name##u32, name##u64,
#define VM_COMMAND_DEFINE_INT(name) name##i8_, name##i16, name##i32, name##i64,
#define VM_COMMAND_DEFINE_FLOAT(name) name##f32, name##f64,
#define VM_COMMAND_DEFINE_ALL(name) VM_COMMAND_DEFINE_UINT(name) VM_COMMAND_DEFINE_INT(name) VM_COMMAND_DEFINE_FLOAT(name)
#define VM_COMMAND_DEFINE_ALL_INT(name) VM_COMMAND_DEFINE_UINT(name)
#define VM_COMMAND_DEFINE_ALL_FLOAT(name) VM_COMMAND_DEFINE_FLOAT(name)

#define VM_COMMAND_CASE_UINT(asm, fun) fun(asm, u8_, u8) fun(asm, u16, u16) fun(asm, u32, u32) fun(asm, u64, u64)
#define VM_COMMAND_CASE_INT(asm, fun) fun(asm, i8_, i8) fun(asm, i16, i16) fun(asm, i32, i32) fun(asm, i64, i64)
#define VM_COMMAND_CASE_FLOAT(asm, fun) fun(asm, f32, f32) fun(asm, f64, f64)
#define VM_COMMAND_CASE_ALL(asm, fun) VM_COMMAND_CASE_UINT(asm, fun) VM_COMMAND_CASE_INT(asm, fun) VM_COMMAND_CASE_FLOAT(asm, fun)
#define VM_COMMAND_CASE_ALL_INT(asm, fun) VM_COMMAND_CASE_UINT(asm, fun) VM_COMMAND_CASE_INT(asm, fun)
#define VM_COMMAND_CASE_ALL_FLOAT(asm, fun) VM_COMMAND_CASE_FLOAT(asm, fun)