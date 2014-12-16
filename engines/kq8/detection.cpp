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

#include "engines/advancedDetector.h"

#include "engines/kq8/kq8.h"

#include "common/savefile.h"
#include "common/serializer.h"

#include "graphics/scaler.h"

namespace KQ8 {

static const char *directoryGlobs[] = {
	"Kq",
	0
};

static const PlainGameDescriptor kq8Games[] = {
	{ "kq8", "Kings Quest 8" },
	{ 0, 0 }
};

#define KQ8ENTRY(platform, lang, extra, exe, md5exe) 				\
{																				\
	{																			\
		"kq8",																\
		extra,																	\
		{																		\
			{ exe, 0, md5exe, -1 },												\
		},																		\
		lang,																	\
		platform,																\
		ADGF_NO_FLAGS,															\
		GUIO_NONE																\
	},																			\
},

static const KQ8GameDescription gameDescriptions[] = {
	KQ8ENTRY(Common::kPlatformWindows, Common::EN_ANY, 0,     "Mask.exe", "080b200be608c5f35f55f86e0c612b22")
	{ AD_TABLE_END_MARKER }
};

class KQ8MetaEngine : public AdvancedMetaEngine {
public:
	KQ8MetaEngine() : AdvancedMetaEngine(gameDescriptions, sizeof(KQ8GameDescription), kq8Games) {
		_singleid = "kq8";
		_guioptions = GUIO4(GUIO_NOMIDI, GUIO_NOSFX, GUIO_NOSPEECH, GUIO_NOSUBTITLES);
		_maxScanDepth = 3;
		_directoryGlobs = directoryGlobs;
	}

	virtual const char *getName() const {
		return "Kings Quest 8 Engine";
	}

	virtual const char *getOriginalCopyright() const {
		return "Kings Quest 8 (C) Sierra";
	}

	virtual bool hasFeature(MetaEngineFeature f) const {
		return false;
	}


	virtual bool createInstance(OSystem *syst, Engine **engine, const ADGameDescription *gd) const;
};

bool KQ8MetaEngine::createInstance(OSystem *syst, Engine **engine, const ADGameDescription *gd) const {
	const KQ8GameDescription *desc = (const KQ8GameDescription *)gd;
	if (desc) {
		*engine = new KQ8Engine(syst, desc);
	}
	return desc != nullptr;
}

Common::Platform KQ8Engine::getPlatform() const {
	return _gameDescription->desc.platform;
}

Common::Language KQ8Engine::getDefaultLanguage() const {
	return _gameDescription->desc.language;
}

} // End of namespace KQ8

#if PLUGIN_ENABLED_DYNAMIC(KQ8)
	REGISTER_PLUGIN_DYNAMIC(KQ8, PLUGIN_TYPE_ENGINE, KQ8::KQ8MetaEngine);
#else
	REGISTER_PLUGIN_STATIC(KQ8, PLUGIN_TYPE_ENGINE, KQ8::KQ8MetaEngine);
#endif
