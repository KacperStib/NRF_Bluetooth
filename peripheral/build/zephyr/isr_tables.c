
/* AUTO-GENERATED by gen_isr_tables.py, do not edit! */

#include <zephyr/toolchain.h>
#include <zephyr/linker/sections.h>
#include <zephyr/sw_isr_table.h>
#include <zephyr/arch/cpu.h>

typedef void (* ISR)(const void *);
uintptr_t __irq_vector_table _irq_vector_table[48] = {
	((uintptr_t)&_isr_wrapper),
	158735,
	((uintptr_t)&_isr_wrapper),
	((uintptr_t)&_isr_wrapper),
	((uintptr_t)&_isr_wrapper),
	((uintptr_t)&_isr_wrapper),
	((uintptr_t)&_isr_wrapper),
	((uintptr_t)&_isr_wrapper),
	158661,
	((uintptr_t)&_isr_wrapper),
	((uintptr_t)&_isr_wrapper),
	158713,
	((uintptr_t)&_isr_wrapper),
	((uintptr_t)&_isr_wrapper),
	((uintptr_t)&_isr_wrapper),
	((uintptr_t)&_isr_wrapper),
	((uintptr_t)&_isr_wrapper),
	((uintptr_t)&_isr_wrapper),
	((uintptr_t)&_isr_wrapper),
	((uintptr_t)&_isr_wrapper),
	((uintptr_t)&_isr_wrapper),
	((uintptr_t)&_isr_wrapper),
	((uintptr_t)&_isr_wrapper),
	((uintptr_t)&_isr_wrapper),
	((uintptr_t)&_isr_wrapper),
	((uintptr_t)&_isr_wrapper),
	((uintptr_t)&_isr_wrapper),
	((uintptr_t)&_isr_wrapper),
	((uintptr_t)&_isr_wrapper),
	((uintptr_t)&_isr_wrapper),
	((uintptr_t)&_isr_wrapper),
	((uintptr_t)&_isr_wrapper),
	((uintptr_t)&_isr_wrapper),
	((uintptr_t)&_isr_wrapper),
	((uintptr_t)&_isr_wrapper),
	((uintptr_t)&_isr_wrapper),
	((uintptr_t)&_isr_wrapper),
	((uintptr_t)&_isr_wrapper),
	((uintptr_t)&_isr_wrapper),
	((uintptr_t)&_isr_wrapper),
	((uintptr_t)&_isr_wrapper),
	((uintptr_t)&_isr_wrapper),
	((uintptr_t)&_isr_wrapper),
	((uintptr_t)&_isr_wrapper),
	((uintptr_t)&_isr_wrapper),
	((uintptr_t)&_isr_wrapper),
	((uintptr_t)&_isr_wrapper),
	((uintptr_t)&_isr_wrapper),
};
struct _isr_table_entry __sw_isr_table _sw_isr_table[48] = {
	{(const void *)0x26c3f, (ISR)0x26c7d}, /* 0 */
	{(const void *)0x0, (ISR)z_irq_spurious}, /* 1 */
	{(const void *)0x27d58, (ISR)0x26971}, /* 2 */
	{(const void *)0x1ff05, (ISR)0x26c7d}, /* 3 */
	{(const void *)0x0, (ISR)z_irq_spurious}, /* 4 */
	{(const void *)0x0, (ISR)z_irq_spurious}, /* 5 */
	{(const void *)0x1f8a1, (ISR)0x26c7d}, /* 6 */
	{(const void *)0x0, (ISR)z_irq_spurious}, /* 7 */
	{(const void *)0x0, (ISR)z_irq_spurious}, /* 8 */
	{(const void *)0x0, (ISR)z_irq_spurious}, /* 9 */
	{(const void *)0x0, (ISR)z_irq_spurious}, /* 10 */
	{(const void *)0x0, (ISR)z_irq_spurious}, /* 11 */
	{(const void *)0x0, (ISR)z_irq_spurious}, /* 12 */
	{(const void *)0x20003050, (ISR)0x1cf39}, /* 13 */
	{(const void *)0x0, (ISR)z_irq_spurious}, /* 14 */
	{(const void *)0x0, (ISR)z_irq_spurious}, /* 15 */
	{(const void *)0x0, (ISR)z_irq_spurious}, /* 16 */
	{(const void *)0x0, (ISR)0x1def5}, /* 17 */
	{(const void *)0x0, (ISR)z_irq_spurious}, /* 18 */
	{(const void *)0x0, (ISR)z_irq_spurious}, /* 19 */
	{(const void *)0x0, (ISR)z_irq_spurious}, /* 20 */
	{(const void *)0x0, (ISR)z_irq_spurious}, /* 21 */
	{(const void *)0x0, (ISR)z_irq_spurious}, /* 22 */
	{(const void *)0x0, (ISR)z_irq_spurious}, /* 23 */
	{(const void *)0x0, (ISR)z_irq_spurious}, /* 24 */
	{(const void *)0x0, (ISR)0x1ed59}, /* 25 */
	{(const void *)0x0, (ISR)z_irq_spurious}, /* 26 */
	{(const void *)0x0, (ISR)z_irq_spurious}, /* 27 */
	{(const void *)0x0, (ISR)z_irq_spurious}, /* 28 */
	{(const void *)0x0, (ISR)z_irq_spurious}, /* 29 */
	{(const void *)0x0, (ISR)z_irq_spurious}, /* 30 */
	{(const void *)0x0, (ISR)z_irq_spurious}, /* 31 */
	{(const void *)0x0, (ISR)z_irq_spurious}, /* 32 */
	{(const void *)0x0, (ISR)z_irq_spurious}, /* 33 */
	{(const void *)0x0, (ISR)z_irq_spurious}, /* 34 */
	{(const void *)0x0, (ISR)z_irq_spurious}, /* 35 */
	{(const void *)0x0, (ISR)z_irq_spurious}, /* 36 */
	{(const void *)0x0, (ISR)z_irq_spurious}, /* 37 */
	{(const void *)0x0, (ISR)z_irq_spurious}, /* 38 */
	{(const void *)0x0, (ISR)z_irq_spurious}, /* 39 */
	{(const void *)0x0, (ISR)z_irq_spurious}, /* 40 */
	{(const void *)0x0, (ISR)z_irq_spurious}, /* 41 */
	{(const void *)0x0, (ISR)z_irq_spurious}, /* 42 */
	{(const void *)0x0, (ISR)z_irq_spurious}, /* 43 */
	{(const void *)0x0, (ISR)z_irq_spurious}, /* 44 */
	{(const void *)0x0, (ISR)z_irq_spurious}, /* 45 */
	{(const void *)0x0, (ISR)z_irq_spurious}, /* 46 */
	{(const void *)0x0, (ISR)z_irq_spurious}, /* 47 */
};
