#include "yukigram/settings/hide_popular_sticker_suggest.h"
#include "yukigram/options.h"

namespace Yukigram::Settings {

rpl::variable<bool> *HidePopularStickerSuggest = Yukigram::Options::make<bool>(kOptionHidePopularStickerSuggest, {
	.keywords = { u"yukigram"_q, u"stickers"_q, u"suggested"_q, u"pack"_q, u"popular"_q },
	.category = "compose",
});

const char kOptionHidePopularStickerSuggest[] = "hide-popular-sticker-suggest";

}
