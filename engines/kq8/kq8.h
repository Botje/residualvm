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

#ifndef KQ8_ENGINE_H
#define KQ8_ENGINE_H

#include <typeinfo>

#include "engines/advancedDetector.h"
#include "engines/engine.h"

#include "engines/kq8/primitives.h"
#include "engines/kq8/registry.h"

#include "common/hashmap.h"
#include "common/hash-str.h"
#include "common/noncopyable.h"
#include "common/ptr.h"
#include "common/random.h"
#include "common/system.h"

namespace Graphics {
struct Surface;
}

namespace KQ8 {

enum GameVersionFlags {
	kFlagNone      = 0,
	kFlagVersion10 = (1 << 0), // v1.0
	kFlagDVD       = (1 << 1)  // DVD version
};

struct KQ8GameDescription {
	ADGameDescription desc;
};

class KQ8Engine : public Engine, private Common::NonCopyable {

protected:
	// Engine APIs
	virtual Common::Error run();
	virtual void syncSoundSettings();
	virtual GUI::Debugger *getDebugger() { return nullptr; }
public:
	Common::RandomSource *_rnd;

	KQ8Engine(OSystem *syst, const KQ8GameDescription *version);
	virtual ~KQ8Engine();

	bool hasFeature(EngineFeature f) const;
	Common::Platform getPlatform() const;
	Common::Language getDefaultLanguage() const;

	Primitives &getPrimitives() { return _primitives; }
	Registry &getRegistry() { return _registry; }

	virtual void settingsInitDefaults();
	void loadVolume(const Common::String& volName, const Common::String& path);
	void unloadVolume(const Common::String& volName);

	template <class K>
	K& get() {
		return *static_cast<K*>(_singletons[typeid(K).name()].get());
	}

	template <class K>
	void set(Common::SharedPtr<K> ptr) {
		_singletons[typeid(K).name()] = ptr;
	}

	template <class K>
	void set(K* ptr) {
		_singletons[typeid(K).name()] = Common::SharedPtr<K>(ptr);
	}

	template <class K>
	void erase() {
		_singletons.erase(typeid(K).name());
	}

private:
	OSystem *_system;
	const KQ8GameDescription *_gameDescription;

	bool checkDatafiles();
	void processInput();

	bool _quitting;
	Primitives _primitives;
	Registry _registry;
	Common::HashMap<Common::String, Common::SharedPtr<KQObject> > _singletons;
};

extern KQ8Engine *g_kq8;

} // end of namespace KQ8

#endif
