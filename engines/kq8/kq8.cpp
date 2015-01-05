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
#include "engines/kq8/script.h"
#include "engines/kq8/vol/archive.h"

namespace KQ8 {

KQ8Engine *g_kq8 = nullptr;

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

	g_kq8 = this;

	settingsInitDefaults();
}

KQ8Engine::~KQ8Engine() {
	DebugMan.clearAllDebugChannels();
}

bool KQ8Engine::hasFeature(EngineFeature f) const {
	return false;
}

Common::Error KQ8Engine::run() {
	_rnd = new Common::RandomSource("kq8");

	_system->showMouse(true);

	Script s("Mask.cs");
	Args args;
	args.push_back("Mask");
	args.push_back("Init");
	s.execute(args);

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

void KQ8Engine::loadVolume(const Common::String& volName, const Common::String& path) {
	if (SearchMan.hasArchive(volName))
		SearchMan.remove(volName);

	bool found = false;
	Common::FSNode volPath(ConfMan.get("path") + "/data/" + path + ".vol");
	if (volPath.exists()) {
		Common::ScopedPtr<VolArchive> v(new VolArchive());
		if (v->open(volPath)) {
			SearchMan.add(volName + "_vol", v.release(), 100, true);
		}
		found = true;
	}

	Common::FSNode fullPath(ConfMan.get("path") + "/data/" + path);
	if (fullPath.isDirectory()) {
		SearchMan.addDirectory(volName + "_dir", fullPath, 101, 3, false);
		found = true;
	}

	if (!found)
		warning("Could not load volume %s from %s", volName.c_str(), path.c_str());
}

void KQ8Engine::unloadVolume(const Common::String& volName) {
	if (SearchMan.hasArchive(volName + "_vol"))
		SearchMan.remove(volName + "_vol");
	if (SearchMan.hasArchive(volName + "_dir"))
		SearchMan.remove(volName + "_dir");
}

} // end of namespace KQ8
