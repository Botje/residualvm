/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
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

#include "common/scummsys.h"

#include "image/pbmp.h"

#include "graphics/pixelformat.h"
#include "graphics/surface.h"

#include "common/stream.h"

namespace Image {

PBMPDecoder::PBMPDecoder() : _outputSurface(0), _palette(0), _paletteColorCount(0), _stream(0) {
}

PBMPDecoder::~PBMPDecoder() {
	destroy();
}

void PBMPDecoder::destroy() {
	if (_outputSurface) {
		_outputSurface->free();
		delete _outputSurface;
		_outputSurface = 0;
	}
	delete[] _palette;
	_palette = NULL;
}


bool PBMPDecoder::loadStream(Common::SeekableReadStream &stream) {
	destroy();
	_stream = &stream;
	// First, check the PBMP signature
	if (_stream->readUint32BE() != MKTAG('P', 'B', 'M', 'P')) {
		delete _stream;
		return false;
	}

	_paletteColorCount = 256;
	_palette = new byte[_paletteColorCount * 3];
	for (int i = 0; i < _paletteColorCount; i++) {
		_palette[(i * 3)] = i;
		_palette[(i * 3) + 1] = i;
		_palette[(i * 3) + 2] = i;
	}

	_stream->readUint32LE();
	uint32 head = _stream->readUint32BE();
	assert(head == MKTAG('h','e','a','d'));
	uint csize = _stream->readUint32LE(); // head chunk size
	assert(csize == 0x14);
	_stream->readUint32LE(); // 2
	uint32 width = _stream->readUint32LE();
	uint32 height = _stream->readUint32LE();
	_stream->readUint32LE(); // 8
	_stream->readUint32LE(); // 8

	uint32 data = _stream->readUint32BE();
	assert(data == MKTAG('d','a','t','a'));

	uint32 size = _stream->readUint32LE();
	assert(size == (width * height));

	_outputSurface = new Graphics::Surface();
	_outputSurface->create(width, height, Graphics::PixelFormat::createFormatCLUT8());

	_stream->read(_outputSurface->getPixels(), size);


	// We no longer need the file stream, thus close it here
	_stream = 0;

	return true;
}

} // End of namespace Image
