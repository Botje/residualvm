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

#ifndef KQ8_VOL_H
#define KQ8_VOL_H

#include "common/archive.h"
#include "common/fs.h"
#include "common/str.h"
#include "common/hash-str.h"
#include "common/hashmap.h"
#include "common/stream.h"

namespace KQ8 {

class VolArchive : public Common::Archive {
	public:
	struct Entry : public Common::ArchiveMember {
		VolArchive *_parent;
		Common::String _filename;
		uint32 _pos;
		uint32 _compressedSize;
		Entry() : _filename(""), _pos(0), _compressedSize(0) {}
		Entry(VolArchive *parent, const Common::String &filename, const uint32 pos, const uint32 compressedSize)
			: _parent(parent), _filename(filename), _pos(pos), _compressedSize(compressedSize) {}

		virtual ~Entry() { }
		virtual Common::SeekableReadStream *createReadStream() const;
		virtual Common::String getName() const;
		virtual Common::String getDisplayName() const { return getName(); }
	};

	bool open(const Common::String &filename);
	bool open(const Common::FSNode &path);
	VolArchive();
	virtual ~VolArchive();
	// Common::Archive implementation
	virtual bool hasFile(const Common::String &name) const override;
	virtual int listMembers(Common::ArchiveMemberList &list) const override;
	virtual const Common::ArchiveMemberPtr getMember(const Common::String &name) const override;
	virtual Common::SeekableReadStream *createReadStreamForMember(const Common::String &name) const override;

	private:
	bool open(Common::SeekableReadStream *s);
	Common::String _filename;
	typedef Common::SharedPtr<Entry> EntryPtr;
	typedef Common::HashMap<Common::String, EntryPtr, Common::IgnoreCase_Hash, Common::IgnoreCase_EqualTo> Entries;
	Entries _entries;
	Common::SeekableReadStream *_stream;
};

} // end of namespace KQ8

#endif
