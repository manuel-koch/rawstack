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
#ifndef UNITTESTENV_H
#define UNITTESTENV_H

#include <gtest/gtest.h>

class UnitTestEnv : public testing::Environment
{
public:

    /// Constructor
    UnitTestEnv();

    /// Destructor
    virtual ~UnitTestEnv();

    /// overwritten: \see testing::Environment::SetUp
	virtual void SetUp();

    /// overwritten: \see testing::Environment::TearDown
    virtual void TearDown();

private:

   UnitTestEnv(const UnitTestEnv& rhs) = delete;
   UnitTestEnv& operator=(const UnitTestEnv& rhs) = delete;
};

#endif // UNITTESTENV_H
