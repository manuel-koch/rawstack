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
#ifndef TESTHELPERMACROS_H_39A50EC4_6EC0_4F0F_B1CC_23C5CBA4D62B
#define TESTHELPERMACROS_H_39A50EC4_6EC0_4F0F_B1CC_23C5CBA4D62B

#define EXPECT_NULL(ptr)     EXPECT_EQ(ptr,nullptr)
#define EXPECT_NOT_NULL(ptr) EXPECT_NE(ptr,nullptr)
#define ASSERT_NULL(ptr)     ASSERT_EQ(ptr,nullptr)
#define ASSERT_NOT_NULL(ptr) ASSERT_NE(ptr,nullptr)

#endif // TESTHELPERMACROS_H_39A50EC4_6EC0_4F0F_B1CC_23C5CBA4D62B
