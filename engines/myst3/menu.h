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

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef MENU_H_
#define MENU_H_

#include "engines/myst3/gfx.h"

#include "common/rect.h"
#include "common/memstream.h"
#include "common/str-array.h"

#include "video/bink_decoder_seek.h"

namespace Myst3 {

class Myst3Engine;
class SpotItemFace;

class Menu {
public:
	Menu(Myst3Engine *vm);
	virtual ~Menu();

	void draw();

	void updateMainMenu(uint16 action);
	void goToNode(uint16 node);

	void loadMenuOpen();
	void loadMenuSelect(uint16 item);
	void loadMenuChangePage();
	void setSaveLoadSpotItem(SpotItemFace *spotItem) { _saveLoadSpotItem = spotItem; }

private:
	Myst3Engine *_vm;

	Common::StringArray _saveLoadFiles;
	SpotItemFace *_saveLoadSpotItem;

	void saveLoadUpdateVars();
};

class Dialog : public Drawable {
public:
	Dialog(Myst3Engine *vm, uint id);
	~Dialog();
	void draw();
	int16 update();

private:
	Myst3Engine *_vm;

	Common::MemoryReadStream *_movieStream;
	Video::SeekableBinkDecoder _bink;
	uint16 _previousframe;
	uint16 _frameToDisplay;

	uint _buttonCount;
	Common::Rect _buttons[3];

	Texture *_texture;

	Common::Rect getPosition();
};

} /* namespace Myst3 */
#endif /* MENU_H_ */
