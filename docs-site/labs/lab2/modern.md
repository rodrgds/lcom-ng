# How it works today: timers

Modern machines still need timer interrupts, but they rarely use the PIT as the
main timing source.

## Modern timer sources

Common facilities include APIC timers, HPET, invariant TSC and TSC deadline
timers, paravirtualized clocks in VMs, scheduler timers, and high-resolution
timers. The operating system chooses the best source and hides most of that
choice from applications.

## What changed

Modern timers are more precise, scale across CPUs, and integrate with power
management. APIC and MSI/MSI-X replaced much of the older PIC-style routing.
Kernels also coalesce timers and use tickless idle to avoid waking CPUs, so real
APIs sometimes differ from a simple "interrupt every N milliseconds" model.

## What stayed the same

Time still arrives as events, handlers should be small, state should advance
predictably, and tests need deterministic time. lcom-ng keeps the PIT-shaped
interface because it is small enough to program directly.

Once you understand it, APIs such as `timerfd`, browser animation callbacks, SDL
timers, and fixed timestep loops are easier to reason about. They answer the same
question: when should the program advance?
