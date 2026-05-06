#include "yukigram/settings/star_hide_empty.h"
#include "yukigram/options.h"

namespace Yukigram::Settings {

rpl::variable<bool> *StarHideEmpty = Yukigram::Options::make<bool>(kOptionStarHideEmpty, {
	.keywords = { u"yukigram"_q, u"star"_q, u"reaction"_q },
	.category = "reactions",
	.restartRequired = true,
});

const char kOptionStarHideEmpty[] = "star-hide-empty";

}
