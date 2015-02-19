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
#include "common/tokenizer.h"

#include "engines/kq8/kq8.h"
#include "engines/kq8/classes/kqinventoryitemtypelist.h"

namespace KQ8 {

KQInventoryItemTypeList::KQInventoryItemTypeList(const IniFile& init)
		: KQObject() {
			const Common::StringMap &sm = init.getSections()[0].entries;
			Common::StringMap::const_iterator it = sm.begin();
			for (; it != sm.end(); ++it) {
				if (! it->_key.hasPrefix("item"))
					continue;

				Common::StringMap vals;
				Common::StringTokenizer st(it->_value);
				vals["classType"] = st.nextToken();
				vals["IDName"] = st.nextToken();
				vals["IDName"].erase(0,8); // remove "INVITEM_"
				vals["IDNum"] = st.nextToken();
				vals["Hlth"] = st.nextToken();
				vals["SDam"] = st.nextToken();
				vals["LDam"] = st.nextToken();
				vals["Prot"] = st.nextToken();
				vals["Exp"] = st.nextToken();
				vals["Comb"] = st.nextToken();
				vals["category"] = st.nextToken();
				vals["GUIBitmap"] = st.nextToken();
				vals["Message"] = st.nextToken();
				vals["N"] = st.nextToken();
				vals["V"] = st.nextToken();
				vals["C"] = st.nextToken();
				vals["S"] = st.nextToken();
				vals["Mode"] = st.nextToken();
				vals["Dur"] = st.nextToken();
				vals["Disp"] = st.nextToken();
				vals["landSnd"] = st.nextToken();
				vals["pickupSnd"] = st.nextToken();
				vals["useSnd"] = st.nextToken();
				vals["DTSfile"] = st.nextToken();
				vals["cursor"] = st.nextToken();
				vals["Radius"] = st.nextToken();
				vals["Translucency"] = st.nextToken();
				vals["extraData"] = st.nextToken();

				_items[vals["IDName"]] = vals;
			}
	g_kq8->set<KQInventoryItemTypeList>(this);
}

KQObject *createKQInventoryItemTypeList(const IniFile& ini) {
	return new KQInventoryItemTypeList(ini);
}

} // end of namespace KQ8
