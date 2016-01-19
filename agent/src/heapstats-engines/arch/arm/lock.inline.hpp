/*!
 * \file lock.inline.hpp
 * \brief This file defines spinlock functions.
 * Copyright (C) 2015 Yasumasa Suenaga
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#ifndef LOCK_INLINE_HPP
#define LOCK_INLINE_HPP

#include <sched.h>

/*!
 * \brief Wait spin lock.
 * \param aLock [in] Target integer lock.
 */
inline void spinLockWait(volatile int *aLock) {
  asm volatile(
    "1:"
    "  ldrex   %%r0, [%0];"
    "  tst     %%r0, %%r0;"
    "  strexeq %%r0,   %1, [%0];"
    "  tsteq   %%r0, %%r0;"
    "  bne     1b;"
    :
    : "r"(aLock), "r"(1)
    : "cc", "memory", "%r0"
  );
};

/*!
 * \brief Release spin lock.
 * \param aLock [in] Target integer lock.
 */
inline void spinLockRelease(volatile int *aLock) {
  asm volatile(
    "1:"
    "  ldrex %%r0, [%0];"
    "  strex %%r0,   %1, [%0];"
    "  tst   %%r0, %%r0;"
    "  bne     1b;"
    :
    : "r"(aLock), "r"(0)
    : "cc", "memory", "%r0"
  );
};

#endif  // LOCK_INLINE_HPP
