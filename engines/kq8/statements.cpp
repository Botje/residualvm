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

#include "engines/kq8/statements.h"

namespace KQ8 {

static Common::String resolve(const Args& args, const Common::String& token) {
	if (token[0] == '$' && Common::isDigit(token[1]))
		return args[int(token[1] - '1')];
	if (token[0] == '$')
		return ""; // FIXME: look up in global environment
	return token;
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

	Common::String line;
	foreach(const Common::String& tok, actual) {
		if (&tok != &actual[0])
			line += " ";
		line += tok;
	}
	debug("%03d: %s", _lineNumber, line.c_str());
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
		: SimpleStatement(lineNumber, tok) {
}

void IfStatement::execute(const Args& args) {
	Common::Array<Common::String> actual;
	foreach (const Common::String &tok, _tokens) {
		actual.push_back(resolve(args,tok));
	}
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
