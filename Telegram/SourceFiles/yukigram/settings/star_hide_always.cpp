#include "yukigram/settings/star_hide_always.h"
#include "yukigram/options.h"

namespace Yukigram::Settings {

rpl::variable<bool> *StarHideAlways = Yukigram::Options::make<bool>(kOptionStarHideAlways, {
	.keywords = { u"yukigram"_q, u"star"_q, u"reaction"_q },
	.category = "reactions",
	.restartRequired = true,
});

const char kOptionStarHideAlways[] = "star-hide-always";

}
