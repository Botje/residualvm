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

#include "common/debug.h"
#include "common/foreach.h"
#include "common/memstream.h"

#include "engines/kq8/vol/archive.h"

extern "C" {
#include "engines/kq8/vol/blast.h"
}

namespace KQ8 {

VolArchive::VolArchive()
		: _stream(nullptr) {
}

VolArchive::~VolArchive() {
	delete _stream;
}

bool VolArchive::open(const Common::FSNode &path) {
	if (!path.exists())
		return false;
	_filename = path.getPath();
	return open(path.createReadStream());
}

bool VolArchive::open(const Common::String &filename) {
	Common::SeekableReadStream *s = SearchMan.createReadStreamForMember(filename);
	_filename = filename;
	if (!s)
		return false;

	return open(s);
}

bool VolArchive::open(Common::SeekableReadStream* s) {
	char magic[8];
	s->read(magic, 8);
	if (0 != memcmp(magic, "RESLIST", 8)) {
		error("%s is not a .vol archive", _filename.c_str());
		delete s;
		return false;
	}

	while (true) {
		char fname[20];
		s->read(fname, 20);
		fname[19] = '\0';
		if (s->eos())
			break;

		uint32 nextEntry = s->readUint32LE();
		uint32 pos = (uint32) s->pos();
		_entries[fname] = EntryPtr(new Entry(this, fname, pos, nextEntry - pos));
		s->seek(nextEntry);
	}

	_stream = s;

	return true;
}

Common::SeekableReadStream * VolArchive::Entry::createReadStream() const {
	return _parent->createReadStreamForMember(_filename);
}

Common::String VolArchive::Entry::getName() const {
	return _filename;
}

bool VolArchive::hasFile(const Common::String &name) const {
	return _entries.contains(name);
}

int VolArchive::listMembers(Common::ArchiveMemberList &list) const {
	for (Entries::const_iterator it = _entries.begin(); it != _entries.end(); ++it) {
		list.push_back(it->_value);
	}
	return _entries.size();
}

const Common::ArchiveMemberPtr VolArchive::getMember(const Common::String &name) const {
	return _entries[name];
}

static unsigned blastRead(void *how, unsigned char **buf) {
	byte *inbuf = reinterpret_cast<byte *>(how);
	int size = (int)(* reinterpret_cast<uint32 *>(how));
	*buf = inbuf + sizeof(uint32);
	return size;
}

static int blastWrite(void *how, unsigned char *buf, unsigned len) {
	Common::MemoryWriteStreamDynamic *str = reinterpret_cast<Common::MemoryWriteStreamDynamic *>(how);
	str->write(buf, len);
	return 0;
}

Common::SeekableReadStream * VolArchive::createReadStreamForMember(const Common::String &name) const {
	const EntryPtr &e = _entries[name];
	_stream->seek(e->_pos);

	byte *inbuf = new byte[e->_compressedSize + sizeof(uint32)];
	(*(uint32 *)inbuf) = e->_compressedSize;
	_stream->read(inbuf + sizeof(uint32), e->_compressedSize);

	Common::MemoryWriteStreamDynamic outstream;

	(void)blast(&blastRead, (void *) inbuf, &blastWrite, (void *) &outstream);

	delete[] inbuf;

	return new Common::MemoryReadStream(outstream.getData(), outstream.size(), DisposeAfterUse::YES);
}

} // end of namespace KQ8
