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

#ifndef KQ8_STATEMENTS_H
#define KQ8_STATEMENTS_H

#include "common/array.h"
#include "common/hash-str.h"
#include "common/hashmap.h"
#include "common/ptr.h"
#include "common/str.h"
#include "common/tokenizer.h"

namespace KQ8 {

class Statement;
class BlockStatement;
typedef Common::SharedPtr<Statement> StatementPtr;
typedef Common::SharedPtr<BlockStatement> BlockStatementPtr;
typedef Common::Array<Common::String> Args;
}

#include "engines/kq8/script.h"

namespace KQ8 {

class Statement {
	public:
	virtual ~Statement() {}
	virtual void execute(const Args& args) {}

	protected:
	Statement() {}
};

class SimpleStatement : public Statement {
	public:
	SimpleStatement(uint16 lineNumber, Common::StringTokenizer& tok);

	virtual void execute(const Args& args);

	Common::Array<Common::String> _tokens;
	uint16 _lineNumber;
};

class BlockStatement : public Statement {
	public:
	StatementPtr & lastChild() {
		return _children.back();
	}

	virtual void addStatement(const StatementPtr p);

	virtual void execute(const Args& args);

	Common::Array<StatementPtr> _children;
};

class IfStatement : public SimpleStatement {
	public:
	IfStatement(uint16 lineNumber, Common::StringTokenizer& tok);

	virtual void execute(const Args& args);

	BlockStatementPtr _cons;
	BlockStatementPtr _alt;
};

} // end of namespace KQ8

#endif
