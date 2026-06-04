#include "yukigram/settings/sticker_size.h"
#include "yukigram/options.h"

namespace Yukigram::Settings {

rpl::variable<int> *StickerSize = Yukigram::Options::make<int>(kOptionStickerSize, {
	.keywords = { u"yukigram"_q, u"sticker"_q, u"size"_q },
	.category = "style",
	.restartRequired = true,
	.mirrorExperimental = true,
	.checkValid = Yukigram::Options::Valid::Bounded(50, 512),
	.validHint = Yukigram::Options::Valid::BoundedHint(50, 512),
});

const char kOptionStickerSize[] = "sticker-size";

}
