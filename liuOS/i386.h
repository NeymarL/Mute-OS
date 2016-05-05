 /*!     \file include/arch/i386.h
  *      \brief Generic (IA-32) routines and headers
  *      \author Andrea Righi <drizzt@inwind.it>
  *      \date Last update: 2004-02-19
  *      \note Copyright (&copy;) 2003 Andrea Righi
  *
  *      This file contains the definition of a lot useful function for
  *      the x86 architecture.
  *      The greater part of them are in assembler...
 */

#ifndef I386_H
#define I386_H

#include <stdint.h>

/** \ingroup Kernel
 *  \defgroup Ki386Routines Generic (IA-32) Routines
 *  Some useful asm routines for the IA-32 architecture.
 *  @{
 */

//! \brief
//!     Read the timestamp counter from the model-specific register
//!     and put it into two 32-bit registers.
#define rdtsc(low, high) \
        __asm__ __volatile__( "rdtsc" : "=a"(low), "=d"(high) )

//! \brief
//!     Read the timestamp counter from the model-specific register
//!     and put it into eax:edx registers. This is a 64-bit value.
#define rdtscll(val) \
        __asm__ __volatile__( "rdtsc" : "=A"(val) )

//! \brief
//!     Force strict CPU ordering (required for all out-of-order
//!     Intel CPUs to preserve the order of execution).
static __inline__ void mb()
{
        __asm__ __volatile__ ("lock; addl $0,0(%%esp)": : :"memory");
}

//! \brief
//!     A macro equivalent to the function mb().
#define wmb()   mb()

//! \brief
//!     A macro that returns current instruction pointer.
#define get_eip() ({ void *eip; __asm__("movl $1f,%0\n1:" : "=g" (eip)); eip; })

//! \brief
//!     Enable all interrupts.
static __inline__ void enable()
{
        __asm__ ("sti" : : : "memory");
}

//! \brief
//!     Disable all interrupts.
static __inline__ void disable()
{
        __asm__ __volatile__ ("cli" : : : "memory");
}

//! \brief
//!     Get the IF (interrupt enable flag) value from the EFLAGS register.
//! \return The interrupt enable flag (IF).
static __inline__ dword GET_IF()
{
        register uint32_t IF;
        __asm__ __volatile__ (
                "pushfl                 \n"
                "popl   %%eax           \n"
                "shrl   $9, %%eax       \n"
                "andl   $1, %%eax       \n" : "=a" (IF) : );
        __asm__ __volatile__("" : : : "eax");
        return( IF );
}

//! \brief
//!     Set the IF (interrupt enable flag) value into the EFLAGS register.
//! \param IF The interrupt flag:
//!     \li \e 1 = ON
//!     \li \e 0 = OFF
static __inline__ void SET_IF(dword IF)
{
        __asm__ __volatile__ (
                "pushfl                 \n"
                "popl   %%eax           \n"
                "orl    $(1 << 9), %%eax\n"
                "andl   %0, %%eax       \n"
                "pushl  %%eax           \n"
                "popfl                  \n" : : "d" ((IF & 0x01) << 9) );
        __asm__ __volatile__("" : : : "eax", "edx");
}

// --- inport#x --------------------------------------------------------//

//! A macro equivalent to the inportb(word port) function.
#define in      inportb
//! A macro equivalent to the inportw(word port) function.
#define in16    inportw
//! A macro equivalent to the inportl(word port) function.
#define in32    inportl

//! \brief
//!     Get a \c byte from an I/O port.
//! \param port The I/O port.
//! \return The value of the byte.
static __inline__ byte inportb(word port)
{
        // Get a byte from I/O port                                             //
        register uint8_t val;
        __asm__ __volatile__ ("inb %%dx, %%al" : "=a" (val) : "d" (port));
        return( val );
}

//! \brief
//!     Get a \c word from an I/O port.
//! \param port The I/O port.
//! \return The value of the word.
static __inline__ word inportw(word port)
{
        register uint16_t val;
        __asm__ __volatile__ ("inw %%dx, %%ax" : "=a" (val) : "d" (port));
        return( val );
}

//! \brief
//!     Get a \c dword from an I/O port.
//! \param port The I/O port.
//! \return The value of the double word.
static __inline__ dword inportl(word port)
{
        register uint32_t val;
        __asm__ __volatile__ ("inl %%dx, %%eax" : "=a" (val) : "d" (port));
        return( val );
}

// --- outport#x -------------------------------------------------------//

//! A macro equivalent to the outportb(word port, byte val) function.
#define out     outportb
//! A macro equivalent to the outportw(word port, word val) function.
#define out16   outportw
//! A macro equivalent to the outportl(word port, dword val) function.
#define out32   outportl

//! \brief
//!     Put a \c byte to an I/O port.
//! \param port The I/O port.
//! \param val The value you want to put.
static __inline__ void outportb(word port, byte val)
{
        __asm__ __volatile__ ("outb %%al, %%dx" : : "d" (port), "a" (val));
}

//! \brief
//!     Put a \c word to an I/O port.
//! \param port The I/O port.
//! \param val The value you want to put.
static __inline__ void outportw(word port, word val)
{
        __asm__ __volatile__ ("outw %%ax, %%dx" : : "d" (port), "a" (val));
}

//! \brief
//!     Put a \c dword to an I/O port.
//! \param port The I/O port.
//! \param val The value you want to put.
static __inline__ void outportl(word port, dword val)
{
        __asm__ __volatile__ ("outl %%eax, %%dx" : : "d" (port), "a" (val));
}

// --- ins* ------------------------------------------------------------//

//! \brief
//!     The string version of in().
//! \param port The I/O port.
//! \param addr A memory buffer where the values will be copied.
//! \param count How many bytes we want to read from the port.
/*!     Copy a sequence of \a count bytes to the buffer \a addr from
 *      the port \a port.
 */
static inline void insb(unsigned short port, void * addr, unsigned long count)
{
        __asm__ __volatile__ ("rep ; insb": "=D"(addr), "=c"(count) : "d"(port), "0"(addr), "1"(count));
}

//! \brief
//!     The string version of in16().
//! \param port The I/O port.
//! \param addr A memory buffer where the values will be copied.
//! \param count How many words we want to read from the port.
/*!     Copy a sequence of \a count words to the buffer \a addr from
 *      the port \a port.
 */
static inline void insw(unsigned short port, void * addr, unsigned long count)
{
        __asm__ __volatile__ ("rep ; insw": "=D"(addr), "=c"(count) : "d"(port), "0"(addr), "1"(count));
}

//! \brief
//!     The string version of in32().
//! \param port The I/O port.
//! \param addr A memory buffer where the values will be copied.
//! \param count How many double words we want to read from the port.
/*!     Copy a sequence of \a count double words to the buffer \a addr
 *      from the port \a port.
 */
static inline void insl(unsigned short port, void * addr, unsigned long count)
{
        __asm__ __volatile__ ("rep ; insl": "=D"(addr), "=c"(count) : "d"(port), "0"(addr), "1"(count));
}

//! A macro equivalent to the
//! insb(unsigned short port, void * addr, unsigned long count) function.
#define inportbm insb
//! A macro equivalent to the
//! insw(unsigned short port, void * addr, unsigned long count) function.
#define inportwm insw
//! A macro equivalent to the
//! insl(unsigned short port, void * addr, unsigned long count) function.
#define inportlm insl

// --- outs* -----------------------------------------------------------//

//! \brief
//!     The string version of out().
//! \param port The I/O port.
//! \param addr A memory buffer that contains the values to be copied.
//! \param count How many bytes we want to write to the port.
/*!     Copy a sequence of \a count bytes to the port \a port
 *      from the buffer \a addr.
 */
static inline void outsb(unsigned short port, void * addr, unsigned long count)
{
        __asm__ __volatile__ ("rep ; outsb": "=S"(addr), "=c"(count) : "d"(port), "0"(addr), "1"(count));
}

//! \brief
//!     The string version of out16().
//! \param port The I/O port.
//! \param addr A memory buffer that contains the values to be copied.
//! \param count How many words we want to write to the port.
/*!     Copy a sequence of \a count words to the port \a port
 *      from the buffer \a addr.
 */
static inline void outsw(unsigned short port, void * addr, unsigned long count)
{
        __asm__ __volatile__ ("rep ; outsw": "=S"(addr), "=c"(count) : "d"(port), "0"(addr), "1"(count));
}

//! \brief
//!     The string version of out32().
//! \param port The I/O port.
//! \param addr A memory buffer that contains the values to be copied.
//! \param count How many double words we want to write to the port.
/*!     Copy a sequence of \a count double words to the port \a port
 *      from the buffer \a addr.
 */
static inline void outsl(unsigned short port, void * addr, unsigned long count)
{
        __asm__ __volatile__ ("rep ; outsl": "=S"(addr), "=c"(count) : "d"(port), "0"(addr), "1"(count));
}

//! A macro equivalent to the
//! outsb(unsigned short port, void * addr, unsigned long count) function.
#define outportbm outsb
//! A macro equivalent to the
//! outsw(unsigned short port, void * addr, unsigned long count) function.
#define outportwm outsw
//! A macro equivalent to the
//! outsl(unsigned short port, void * addr, unsigned long count) function.
#define outportlm outsl

// --- CPUID -----------------------------------------------------------//

//! \brief
//!     Get the CPUID information.
//! \param op The operation code to perform.
//! \param eax EAX register value after the CPUID execution.
//! \param ebx EBX register value after the CPUID execution.
//! \param ecx ECX register value after the CPUID execution.
//! \param edx EDX register value after the CPUID execution.
//! \warning
//!     Not all the Intel CPUs support the CPUID instruction!!!
//!     Only some Intel486 family and subsequent Intel processors
//!     provide this method for determinig the architecture flags.
//!     Execution of CPUID on a processor that does not support this
//!     instruction will result in an invalid opcode exception.
//!     \n \n
//!     To determine if it is possible to use this instruction we can
//!     use bit 21 of the EFLAGS register. If software can change the
//!     value of this flag, the CPUID instruction is executable.
static __inline__ void cpuid(int op, int *eax, int *ebx, int *ecx, int *edx)
{
        __asm__ __volatile__(
                "cpuid"
                :
                "=a" (*eax),
                "=b" (*ebx),
                "=c" (*ecx),
                "=d" (*edx)
                :
                "0" (op)
        );
        __asm__ __volatile__ ("" : : : "eax", "ebx", "ecx", "edx");
}

// ---Atomic operators -------------------------------------------------//

//! The atomic variable structure.
typedef struct atomic
{
        volatile int counter;
} atomic_t;

//! \brief Set the atomic value of \p v to \p i (guaranteed only 24 bits)
//! \param v The atomic variable.
//! \param i The value to assign.
#define atomic_set(v, i)        (((v)->counter) = (i))
//! \brief Read the atomic value of \p v (guaranteed only 24 bits)
//! \param v The atomic variable.
#define atomic_read(v)          ((v)->counter)

//! \brief Perform an atomic increment.
//! \param v The atomic value to increment.
//! \warning Guaranteed only 24 bits.
static __inline__ void atomic_inc(atomic_t *v)
{
        __asm__ __volatile__ (  "lock; incl %0"
                                : "=m"(v->counter) : "m"(v->counter));
}

//! \brief Perform an atomic decrement.
//! \param v The atomic value to decrement.
//! \warning Guaranteed only 24 bits.
static __inline__ void atomic_dec(atomic_t *v)
{
        __asm__ __volatile__ (  "lock; decl %0"
                                : "=m"(v->counter) : "m"(v->counter));
}

//! \brief Halt the system by disabling the CPU.
//! \warning
//!     This is truly the end. You have to reboot the system
//!     after this...
static __inline__ void halt()
{
        __asm__ __volatile__ ("cli\n hlt");
}

// --- Debug operators ------------------------------------------------- //

//! \brief Perform a breakpoint exeception.
static __inline__ void breakpoint()
{
        __asm__ __volatile__ ("int3" : : : "memory");
}

/** @} */ // end of Ki386Routines

#endif