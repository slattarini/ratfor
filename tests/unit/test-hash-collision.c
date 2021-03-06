/* This file is part of the C Ratfor testsuite.
 * Copyright (C) 2010 Stefano Lattarini <stefano.lattarini@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* Check that an hash table can cointin distinct elements whose keys are
   hashed to the same value. */

#include "tests-common.h"
#include "rat4-common.h"
#include "rat4-hash.h"

int
main(void)
{
    int hash(const char *);

    /* sanity checks */
    ASSERT(hash("ad") == hash("da"));
    ASSERT(hash("bc") == hash("cb"));
    ASSERT(hash("bc") == hash("ad"));

    /* test setup */
    hash_install("ad", "A-D");
    hash_install("da", "D-A");
    hash_install("bc", "B-C");
    hash_install("cb", "C-B");

    /* real tests */
    ASSERT(STREQ(hash_lookup("ad"), "A-D"));
    ASSERT(STREQ(hash_lookup("da"), "D-A"));
    ASSERT(STREQ(hash_lookup("bc"), "B-C"));
    ASSERT(STREQ(hash_lookup("cb"), "C-B"));

    return(EXIT_SUCCESS);
}

/* vim: set ft=c ts=4 sw=4 et : */
