#include "yukigram/settings/round_size.h"
#include "yukigram/options.h"

namespace Yukigram::Settings {

rpl::variable<int> *RoundSize = Yukigram::Options::make<int>(kOptionRoundSize, {
	.keywords = { u"yukigram"_q, u"round"_q, u"video"_q, u"size"_q },
	.category = "style",
	.restartRequired = true,
	.defaultValue = 240,
	.checkValid = Yukigram::Options::Valid::Bounded(80, 384),
	.validHint = Yukigram::Options::Valid::BoundedHint(80, 384),
});

const char kOptionRoundSize[] = "round-size";

}
