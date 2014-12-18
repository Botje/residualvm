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

#include "common/util.h"

#include "engines/kq8/inifile.h"

namespace KQ8 {

IniFile::IniFile(Common::SeekableReadStream *s) {
	if (!s)
		return;

	Section *currentSection;
	do {
		Common::String line = s->readLine();
		if (s->eos())
			break;

		line.trim();
		if (line.empty() or line[0] == ';')
			continue;

		if (line[0] == '[') {
			Common::String section(line.begin()+1, line.end()-1);
			currentSection = &_sections[section];
		} else {
			for (Common::String::const_iterator it = line.begin(); it != line.end(); ++it) {
				if (*it == '=')  {
					const Common::String name(line.begin(), it);

					do { ++it; } while (it != line.end() && Common::isSpace(*it));
					const Common::String value(it, line.end());
					(*currentSection)[name] = value;
				}
			}
		}
	} while (true);

	delete s;
}

} // end of namespace KQ8
