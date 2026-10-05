#include "yukigram/settings/solar_date.h"
#include "yukigram/options.h"

namespace Yukigram::Settings {

rpl::variable<bool> *SolarDate = Yukigram::Options::make<bool>(
        kOptionSolarDate,
        {
                .keywords = {
                        u"mrgram"_q,
                        u"solar"_q,
                        u"date"_q,
                        u"persian"_q,
                        u"shamsi"_q,
                },
                .category = "interface",
                .restartRequired = false,
        });

const char kOptionSolarDate[] = "solar-date";

} // namespace Yukigram::Settings
