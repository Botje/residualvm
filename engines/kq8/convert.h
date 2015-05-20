/* ResidualVM - A 3D game interpreter
 *
 * ResidualVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the AUTHORS
 * file distributed with this source distribution.
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

namespace KQ8 {

#ifndef KQ8_CONVERT_H
#define KQ8_CONVERT_H

template <class T>
T convert(const Common::String &str);

template <>
inline uint32 convert(const Common::String &str) {
	uint32 ret;
	int num_converted = sscanf(str.c_str(), "%u", &ret);
	assert(num_converted == 1);
	return ret;
}

template <>
inline float convert(const Common::String &str) {
	float ret;
	int num_converted = sscanf(str.c_str(), "%f", &ret);
	assert(num_converted == 1);
	return ret;
}

#endif

} // end of namespace KQ8
