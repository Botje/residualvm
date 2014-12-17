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

#ifndef KQ8_SCRIPT_H
#define KQ8_SCRIPT_H

#include "common/str.h"
#include "common/array.h"
#include "common/stream.h"


namespace KQ8 {

class Statement;
class BlockStatement;
typedef Common::SharedPtr<Statement> StatementPtr;
typedef Common::SharedPtr<BlockStatement> BlockStatementPtr;


class Script {
	public:
	typedef Common::Array<Common::String> Args;

	Script(const Common::String &fname);

	void execute(const Args& args);

	private:
	void parse(Common::SeekableReadStream *s);

	const Common::String _fname;
	BlockStatementPtr _body;
};

} // end of namespace KQ8
#endif
