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
#include "common/debug.h"
#include "common/hashmap.h"
#include "common/hash-str.h"

#include "engines/kq8/kq8.h"
#include "engines/kq8/statements.h"

namespace KQ8 {

typedef void (*Command)(const Args& args);
static Common::HashMap<Common::String, Command> g_primitives;
Common::HashMap<Common::String, Common::String> g_env;

static Common::String join(const Args& args, const Common::String& sep = " ") {
	Common::String ret;
	foreach(const Common::String& tok, args) {
		if (&tok != &args[0])
			ret += sep;
		ret += tok;
	}
	return ret;
}

static Common::String resolve(const Args& args, const Common::String& token) {
	const Common::String rest(token.begin()+1, token.end());
	if (token[0] != '$')
		return token;
	if (Common::isDigit(rest[0]))
		return args[int(rest[0] - '1')];
	if (g_env.contains(rest))
		return g_env[rest];

	warning("Unknown variable: %s", rest.c_str());
	return token;
}

void setcat(const Args& args) {
	const Common::String& var = args[0];
	Common::String val;
	for (Args::const_iterator it = args.begin() + 1; it != args.end(); ++it) {
		val += *it;
	}
	g_env[var] = val;
}

void echo(const Args& args) {
	const Common::String str = join(args);
	warning("%s", str.c_str());
}

void loadVolume(const Args& args) {
	const Common::String& volName = args[0];
	const Common::String& path = args[1];
	g_kq8->loadVolume(volName, path);
}

void deleteObject(const Args& args) {
	const Common::String& val = args[0];
	if (val.hasPrefix("KQWorld::")) {
		g_kq8->unloadVolume(val);
	} else {
		warning("Unknown argument to deleteObject '%s'", val.c_str());
	}
}

void set(const Args& args) {
	if (args[0] == "\"\"") {
		g_env.erase(args[0]);
	} else {
		g_env[args[0]] = args[1];
	}
}

bool registerPrimitives() {
	g_primitives["echo"] = &echo;
	g_primitives["loadVolume"] = &loadVolume;
	g_primitives["deleteObject"] = &deleteObject;
	g_primitives["setcat"] = &setcat;
	g_primitives["set"] = &set;
	return true;
}

SimpleStatement::SimpleStatement(uint16 lineNumber, Common::StringTokenizer& tok)
		: _lineNumber(lineNumber) {
	while (!tok.empty()) {
		_tokens.push_back(tok.nextToken());
	}
}

void SimpleStatement::execute(const Args& args) {
	Common::Array<Common::String> actual;
	foreach (const Common::String &tok, _tokens) {
		actual.push_back(resolve(args,tok));
	}

	Common::String line = join(actual);
	debug("%s:%03d: %s", g_scriptStack.top()->getName().c_str(), _lineNumber, line.c_str());

	static bool registered = registerPrimitives();
	registered = !!registered;

	if (actual[0].hasSuffix(".cs")) {
		Script s(actual[0]);
		Args scriptArgs(actual.begin()+1, actual.size()-1);
		s.execute(scriptArgs);
	} else if (g_primitives.contains(actual[0])) {
		Args primArgs(actual.begin()+1, actual.size()-1);
		g_primitives[actual[0]](primArgs);
	}
}

void BlockStatement::addStatement(const StatementPtr p) {
	_children.push_back(p);
}

void BlockStatement::execute(const Args& args) {
	foreach(const StatementPtr& s, _children) {
		s->execute(args);
	}
}

IfStatement::IfStatement(uint16 lineNumber, Common::StringTokenizer& tok)
		: SimpleStatement(lineNumber, tok), _cons(new BlockStatement()), _alt(new BlockStatement()) {
}

void IfStatement::execute(const Args& args) {
	Common::Array<Common::String> actual;
	foreach (const Common::String &tok, _tokens) {
		actual.push_back(resolve(args,tok));
	}
	Common::String line = join(actual);
	debug("%s:%03d: if %s", g_scriptStack.top()->getName().c_str(), _lineNumber, line.c_str());
	assert(actual[0] == "test");
	assert(actual[2] == "==");
	// FIXME: other conditions not yet handled.
	if (0 == actual[1].compareToIgnoreCase(actual[3])) {
		_cons->execute(args);
	} else {
		_alt->execute(args);
	}
}

} // end of namespace KQ8
