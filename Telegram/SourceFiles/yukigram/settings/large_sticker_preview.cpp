#include "yukigram/settings/large_sticker_preview.h"
#include "yukigram/options.h"

namespace Yukigram::Settings {

rpl::variable<bool> *LargeStickerPreview = Yukigram::Options::make<bool>(kOptionLargeStickerPreview, {
	.keywords = { u"yukigram"_q, u"sticker"_q, u"preview"_q },
	.category = "style",
});

const char kOptionLargeStickerPreview[] = "large-sticker-preview";

}
