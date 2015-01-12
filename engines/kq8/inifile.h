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

#ifndef KQ8_INIFILE_H
#define KQ8_INIFILE_H

#include "common/scummsys.h"
#include "common/hashmap.h"
#include "common/str.h"
#include "common/hash-str.h"
#include "common/stream.h"

namespace KQ8 {

class IniFile {
	public:
	typedef Common::StringMap Section;
	typedef Common::HashMap<Common::String, Section, Common::IgnoreCase_Hash, Common::IgnoreCase_EqualTo> SectionsMap;

	IniFile(Common::SeekableReadStream *s);

	const SectionsMap &getSections() const {
		return _sections;
	}

	const Section &getSection(const Common::String &str) const {
		return _sections[str];
	}

	private:
	SectionsMap _sections;
};

} // end of namespace KQ8
#endif
