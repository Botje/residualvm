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

#include "common/foreach.h"

#include "engines/kq8/kq8.h"
#include "engines/kq8/primitives.h"

namespace KQ8 {

Common::String Primitives::join(const Args& args, const Common::String& sep) {
	Common::String ret;
	foreach(const Common::String& tok, args) {
		if (&tok != &args[0])
			ret += sep;
		ret += tok;
	}
	return ret;
}


 Common::String Primitives::resolve(const Args& args, const Common::String& token) {
	const Common::String rest(token.begin()+1, token.end());
	if (token[0] != '$')
		return token;
	if (Common::isDigit(rest[0]))
		return args[int(rest[0] - '1')];
	if (_env.contains(rest))
		return _env[rest];

	warning("Unknown variable: %s", rest.c_str());
	return token;
}

 void Primitives::setcat(const Args& args) {
	const Common::String& var = args[0];
	Common::String val;
	for (Args::const_iterator it = args.begin() + 1; it != args.end(); ++it) {
		val += *it;
	}
	_env[var] = val;
}

 void Primitives::echo(const Args& args) {
	const Common::String str = join(args);
	warning("%s", str.c_str());
}

 void Primitives::loadKQ(const Args& args) {
	Common::String fname(args[0]);
	const Common::ArchiveMemberPtr ptr = SearchMan.getMember(fname);
	if (!ptr)
		error("KQ file %s not found!", fname.c_str());
}

 void Primitives::loadVolume(const Args& args) {
	const Common::String& volName = args[0];
	const Common::String& path = args[1];
	g_kq8->loadVolume(volName, path);
}

 void Primitives::deleteObject(const Args& args) {
	const Common::String& val = args[0];
	if (val.hasPrefix("KQWorld::")) {
		g_kq8->unloadVolume(val);
	} else {
		warning("Unknown argument to deleteObject '%s'", val.c_str());
	}
}

 void Primitives::set(const Args& args) {
	if (args[0] == "\"\"") {
		_env.erase(args[0]);
	} else {
		_env[args[0]] = args[1];
	}
}

Primitives::Primitives() {
	_env["KQWorld::Name"] = "Daventry";
	_env["KQWorld::Parent"] = "Daventry";
	_env["KQGame::BitDepth"] = "8";
	_env["KQGame::Language"] = "English";
	_env["KQGame::runOptimal"] = "yes";

	_primitives["echo"] = &Primitives::echo;
	_primitives["loadKQ"] = &Primitives::loadKQ;
	_primitives["loadVolume"] = &Primitives::loadVolume;
	_primitives["deleteObject"] = &Primitives::deleteObject;
	_primitives["setcat"] = &Primitives::setcat;
	_primitives["set"] = &Primitives::set;
}

} // end of namespace KQ8
