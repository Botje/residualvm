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
#include "image/bmp.h"

#include "engines/kq8/convert.h"
#include "engines/kq8/classes/kqterrain.h"

namespace KQ8 {

static void loadDib(const Common::String& fname) {
	Common::SeekableReadStream* s = SearchMan.createReadStreamForMember(fname);
	Image::BitmapDecoder b;
	b.loadStream(*s);
	delete s;
}

KQTerrain::KQTerrain(const IniFile& ini)
		: KQObject() {
	const Common::String& heightFile       = ini[0]["heightBMP"];
	const Common::String& materialFile     = ini[0]["materialBMP"];
	const Common::String& materialListFile = ini[0]["materialListFile"];

	uint32 heightBMPScale = convert<uint32>(ini[0]["heightBMPScale"]);
	uint32 groundScale    = convert<uint32>(ini[0]["groundScale"]);
	
	loadDib(heightFile);
	loadDib(materialFile);
}

KQObject *createKQTerrain(const IniFile& ini) {
	return new KQTerrain(ini);
}

} // end of namespace KQ8
