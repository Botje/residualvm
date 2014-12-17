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

#include "common/archive.h"
#include "common/array.h"
#include "common/debug.h"
#include "common/foreach.h"
#include "common/stack.h"
#include "common/tokenizer.h"

#include "engines/kq8/script.h"

namespace KQ8 {

class Statement {
	public:
	virtual ~Statement() {}
	virtual void execute() {}

	protected:
	Statement() {}
};

class SimpleStatement : public Statement {
	public:
	SimpleStatement(uint16 lineNumber, Common::StringTokenizer& tok)
			: _lineNumber(lineNumber) {
		while (!tok.empty()) {
			_tokens.push_back(tok.nextToken());
		}
	}

	virtual void execute() {
		Common::String line;
		foreach(const Common::String& tok, _tokens) {
			if (tok != _tokens[0])
				line += " ";
			line += tok;
		}
		debug("%03d: %s", _lineNumber, line.c_str());
	}

	Common::Array<Common::String> _tokens;
	uint16 _lineNumber;
};

class BlockStatement : public Statement {
	public:
	StatementPtr & lastChild() {
		return _children.back();
	}

	virtual void addStatement(const StatementPtr p) {
		_children.push_back(p);
	}

	virtual void execute() {
		foreach(const StatementPtr& s, _children) {
			s->execute();
		}
	}

	Common::Array<StatementPtr> _children;
};

class IfStatement : public SimpleStatement {
	public:
	IfStatement(uint16 lineNumber, Common::StringTokenizer& tok)
			: SimpleStatement(lineNumber, tok) {
	}

	virtual void execute() {
		SimpleStatement::execute();
		_cons->execute();
	}

	BlockStatementPtr _cons;
	BlockStatementPtr _alt;
};

Script::Script(const Common::String& fname)
		: _fname(fname) {
	Common::SeekableReadStream *s = SearchMan.getMember(fname)->createReadStream();
	if (s) {
		parse(s);
		delete s;
	}
}

void Script::execute() {
	if (_body)
		_body->execute();
}



void Script::parse(Common::SeekableReadStream *s) {
	Common::Stack<BlockStatementPtr> nesting;
	nesting.push(BlockStatementPtr(new BlockStatement()));

	uint16 lineNumber = 0;

	do {
		Common::String line = s->readLine();
		++lineNumber;
		if (s->eos())
			break;
		line.trim();
		if (line.empty() or line[0] == '#')
			continue;

		Common::StringTokenizer tok(line);
		again:
		const Common::String first = tok.nextToken();
		if (first == "if") {
			IfStatement *i = new IfStatement(lineNumber, tok);
			BlockStatementPtr b(new BlockStatement());
			i->_cons = b;
			nesting.top()->addStatement(StatementPtr(i));
			nesting.push(b);
		} else if (first == "else") {
			nesting.pop();
			StatementPtr last = nesting.top()->lastChild();
			IfStatement *i = dynamic_cast<IfStatement *>(last.get());
			i->_alt = BlockStatementPtr(new BlockStatement());
			nesting.push(i->_alt);
			goto again;
		} else if (first == "endif") {
			nesting.pop();
		} else {
			tok.reset();
			nesting.top()->addStatement(StatementPtr(new SimpleStatement(lineNumber, tok)));
		}
	} while (true);

	assert(nesting.size() == 1);
	_body = nesting.top();
}

} // end of namespace KQ8
