# 3. Kernel

The skiftOS kernel is called **hjert**, It's a modulare(wip) preemptive kernel for x86 but should be easly portable to other architecture (see: [porting](porting.md)).

The kernel is split into two major part:
 - the [system](system.md) - *architecture indepandent* code.
 - the [arch](arch.md) - *architecture independante* code

Also note that the kernel use code from [libruntime](../2-libraries/libruntime.md), [libsystem](../2-libraries/libsystem.md) and [libc](../2-libraries/libc.md).