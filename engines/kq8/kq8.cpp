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

#include "common/debug-channels.h"
#include "common/events.h"
#include "common/error.h"
#include "common/config-manager.h"
#include "common/file.h"
#include "common/util.h"
#include "common/textconsole.h"
#include "common/translation.h"

#include "audio/mixer.h"

#include "engines/engine.h"

#include "engines/kq8/kq8.h"

namespace KQ8 {

KQ8Engine::KQ8Engine(OSystem *syst, const KQ8GameDescription *version) :
		Engine(syst), _system(syst), _gameDescription(version),
		_quitting(false)
		{

	const Common::FSNode baseDir(ConfMan.get("path"));
	const Common::FSNode gameDir(baseDir.getChild("GAME"));
	SearchMan.addDirectory("GAME/8Gui",gameDir.getChild("8Gui"),0);
	SearchMan.addDirectory("GAME/Common",gameDir.getChild("Common"),0);
	SearchMan.addDirectory("GAME/ENGLISH",gameDir.getChild("ENGLISH"),0);
	SearchMan.addDirectory("GAME/Kq",gameDir.getChild("Kq"),0);
	SearchMan.addDirectory("GAME/Resource",gameDir.getChild("Resource"),0);
	SearchMan.addDirectory("GAME/SOUND",gameDir.getChild("SOUND"),0);

	SearchMan.addDirectory("GAME/patch",gameDir.getChild("patch"),50);


	settingsInitDefaults();
}

KQ8Engine::~KQ8Engine() {
	DebugMan.clearAllDebugChannels();
}

bool KQ8Engine::hasFeature(EngineFeature f) const {
	return false;
}

Common::Error KQ8Engine::run() {
	_rnd = new Common::RandomSource("sprint");

	_system->showMouse(true);

	while (!_quitting) {
		processInput();
	}

	_system->lockMouse(false);

	return Common::kNoError;
}

void KQ8Engine::processInput() {
	Common::Event event;

	while (getEventManager()->pollEvent(event)) {
		if (event.type == Common::EVENT_KEYDOWN) {
			switch (event.kbd.keycode) {
			case Common::KEYCODE_ESCAPE:
				_quitting = true;
				break;
			default:
				break;
			}
		}
	}
}

void KQ8Engine::settingsInitDefaults() {
	ConfMan.registerDefault("overall_volume", Audio::Mixer::kMaxMixerVolume);
	ConfMan.registerDefault("music_volume", Audio::Mixer::kMaxMixerVolume / 2);
	ConfMan.registerDefault("mouse_speed", 50);
}


void KQ8Engine::syncSoundSettings() {
	Engine::syncSoundSettings();

	uint soundOverall = ConfMan.getInt("overall_volume");
	uint soundVolumeMusic = ConfMan.getInt("music_volume");

	_mixer->setVolumeForSoundType(Audio::Mixer::kSFXSoundType, soundOverall);
	_mixer->setVolumeForSoundType(Audio::Mixer::kMusicSoundType, soundVolumeMusic * soundOverall / 256);
}

} // end of namespace KQ8
