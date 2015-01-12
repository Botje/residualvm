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

#ifndef KQ8_PRIMITIVES_H
#define KQ8_PRIMITIVES_H

#include "common/str.h"
#include "common/hashmap.h"
#include "common/hash-str.h"

#include "engines/kq8/statements.h"

namespace KQ8 {

class Primitives {
	public:
		typedef void (Primitives::* Command)(const Args& args);
		Primitives();
		bool hasPrimitive(const Common::String &str) const {
			return _primitives.contains(str);
		}
		void callPrimitive(const Common::String &nam, const Args& args) {
			Command &c = _primitives[nam];
			(this->*c)(args);
		}

		static Common::String join(const Args& args, const Common::String& sep = " ");
		Common::String resolve(const Args& args, const Common::String& token);
		Common::HashMap<Common::String, Common::String> getEnv() { return _env; }
	private:
		Common::HashMap<Common::String, Command> _primitives;
		Common::HashMap<Common::String, Common::String> _env;
		void echo(const Args& args);
		void loadKQ(const Args& args);
		void loadVolume(const Args& args);
		void deleteObject(const Args& args);
		void setcat(const Args& args);
		void set(const Args& args);

};

} // end of namespace KQ8

#endif
