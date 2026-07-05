#include "yukigram/settings/preview_replace.h"
#include "yukigram/options.h"

std::vector<std::tuple<QRegularExpression, QString>> pats;

namespace Yukigram::Settings {

rpl::variable<bool> *PreviewReplace = Yukigram::Options::make<bool>(kOptionPreviewReplace, {
	.keywords = { u"yukigram"_q, u"web"_q, u"preview"_q, u"replace"_q, u"twitter"_q, u"bluesky"_q, u"bsky"_q },
	.category = "compose",
});

const char kOptionPreviewReplace[] = "preview-replace";

rpl::variable<QString> *PreviewReplacePatterns = Yukigram::Options::make<QString>(kOptionPreviewReplacePatterns, {
	.keywords = { u"yukigram"_q, u"web"_q, u"preview"_q, u"replace"_q, u"twitter"_q, u"bluesky"_q, u"bsky"_q },
	.category = "compose",
	.defaultValue =
		R"((?:twitter|x)\.com/(.*) girlcockx.com/\1)" "\n"
		R"(bsky.app/(.*) fxbsky.app/\1)" "\n"
	,
	.link = [](QString value) {
		pats.clear();
		for (const auto &line : value.split(u'\n', Qt::SkipEmptyParts)) {
			const auto words = line.split(u' ');
			pats.emplace_back(QRegularExpression("(?<=https?://|^)" + words[0]), words[1]);
		}
	},
});

const char kOptionPreviewReplacePatterns[] = "preview-replace-patterns";

}

QString YukigramReplaceLink(QString link) {
	if (!Yukigram::Settings::PreviewReplace->current()) {
		return link;
	}
	for (const auto &[pat, repl] : pats) {
		link.replace(pat, repl);
	}
	return link;
}
