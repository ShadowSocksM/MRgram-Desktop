#include "yukigram/settings/hide_rich_paste_toast.h"
#include "yukigram/options.h"

namespace Yukigram::Settings {

rpl::variable<bool> *HideRichPasteToast = Yukigram::Options::make<bool>(kOptionHideRichPasteToast, {
	.keywords = { u"yukigram"_q, u"rich"_q, u"paste"_q, u"plain"_q, u"toast"_q },
	.category = "compose",
});

const char kOptionHideRichPasteToast[] = "hide-rich-paste-toast";

}
