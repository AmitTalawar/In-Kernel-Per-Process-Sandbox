from bcc import BPF

# BPF program to trace the syscalls
bpf_code = """
#include <linux/ptrace.h>
#include <linux/syscalls.h>

int trace_syscall(struct pt_regs *ctx) {
    u64 dummyID = PT_REGS_PARM2(ctx); // Has the SYSCALL ID
    u64 libcID = PT_REGS_PARM3(ctx); // Has the LIBC ID 
    bpf_trace_printk("DUMMY SYSCALL ID: %llu, LIBC ID: %llu\\n", dummyID, libcID); // Print syscall ID and argument 
    return 0;
}
"""

# Load BPF program
b = BPF(text=bpf_code)

dummy_fnname = b.get_syscall_fnname("dummy")

print(dummy_fnname)
# Attach to syscall entry
b.attach_kprobe(event=dummy_fnname, fn_name="trace_syscall")

# Print the output of trace_printk
print("Tracing syscalls.")
try:
    b.trace_print(fmt="{5}")
except KeyboardInterrupt:
    exit()