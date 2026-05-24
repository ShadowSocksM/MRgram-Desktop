#include "yukigram/settings/disable_up_edit.h"
#include "yukigram/options.h"

namespace Yukigram::Settings {

rpl::variable<bool> *DisableUpEdit = Yukigram::Options::make<bool>(kOptionDisableUpEdit, {
	.keywords = { u"yukigram"_q, u"edit"_q, u"up"_q },
	.category = "chat",
});

const char kOptionDisableUpEdit[] = "disable-up-edit";

}
