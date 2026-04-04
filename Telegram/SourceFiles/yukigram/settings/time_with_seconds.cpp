#include "yukigram/settings/time_with_seconds.h"
#include "yukigram/options.h"

namespace Yukigram::Settings {

rpl::variable<bool> *TimeWithSeconds = Yukigram::Options::make<bool>(kOptionTimeWithSeconds, {
	.keywords = { u"yukigram"_q, u"time"_q, u"seconds"_q },
	.category = "interface",
	.restartRequired = true,
});

const char kOptionTimeWithSeconds[] = "time-with-seconds";

}
