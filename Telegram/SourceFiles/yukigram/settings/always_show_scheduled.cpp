#include "yukigram/settings/always_show_scheduled.h"
#include "yukigram/options.h"

namespace Yukigram::Settings {

rpl::variable<bool> *AlwaysShowScheduled = Yukigram::Options::make<bool>(kOptionAlwaysShowScheduled, {
	.keywords = { u"yukigram"_q, u"scheduled"_q, u"delay"_q },
	.category = "compose",
});

const char kOptionAlwaysShowScheduled[] = "always-show-scheduled";

}
