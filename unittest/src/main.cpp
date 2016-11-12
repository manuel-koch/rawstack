/*
 * This file is part of Rawstack.
 *
 * Rawstack is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Rawstack is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Rawstack. If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2016 Manuel Koch
 */
#include "unittestenv.h"
#include <gtest/gtest.h>

int main(int argc, char *argv[])
{
    // Disables/enable elapsed time
    ::testing::GTEST_FLAG(print_time) = true;

    // This allows the user to override the flag on the command line.
    ::testing::InitGoogleTest(&argc, argv);

    AddGlobalTestEnvironment(new UnitTestEnv());
    return RUN_ALL_TESTS();
}
