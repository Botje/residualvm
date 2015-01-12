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
#include "engines/kq8/primitives.h"
#include "engines/kq8/statements.h"

namespace KQ8 {

SimpleStatement::SimpleStatement(uint16 lineNumber, Common::StringTokenizer& tok)
		: _lineNumber(lineNumber) {
	while (!tok.empty()) {
		_tokens.push_back(tok.nextToken());
	}
}

void SimpleStatement::execute(const Args& args) {
	Primitives &p = g_kq8->getPrimitives();
	Common::Array<Common::String> actual;
	foreach (const Common::String &tok, _tokens) {
		actual.push_back(p.resolve(args,tok));
	}

	Common::String line = Primitives::join(actual);
	debug("%s:%03d: %s", g_scriptStack.top()->getName().c_str(), _lineNumber, line.c_str());

	if (actual[0].hasSuffix(".cs")) {
		Script s(actual[0]);
		Args scriptArgs(actual.begin()+1, actual.size()-1);
		s.execute(scriptArgs);
	} else if (p.hasPrimitive(actual[0])) {
		Args primArgs(actual.begin()+1, actual.size()-1);
		p.callPrimitive(actual[0], primArgs);
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
	Primitives &p = g_kq8->getPrimitives();
	Common::Array<Common::String> actual;
	foreach (const Common::String &tok, _tokens) {
		actual.push_back(p.resolve(args,tok));
	}
	Common::String line = Primitives::join(actual);
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
