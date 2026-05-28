/*
This file is part of Kotatogram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/kotatogram/kotatogram-desktop/blob/dev/LEGAL
*/
#include "yukigram/lang.h"

#include "base/parse_helper.h"
#include "lang/lang_instance.h"
#include "lang/lang_tag.h"
#include "ui/text/text_utilities.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <unordered_map>

namespace Yukigram {
namespace Lang {
namespace {

const auto kDefaultLanguage = u"en"_q;
const std::vector<QString> kPostfixes = { "#zero", "#one", "#two", "#few", "#many", "#other" };

QString BaseLangCode;
QString LangCode;

std::unordered_map<QString, QString> Values;

rpl::event_stream<> LangChanges;

void ParseLanguageData(const QString &langCode) {
	const auto filename = u":/yukigram_lang/%1.json"_q.arg(langCode);

	QFile file(filename);
	if (!file.exists()) {
		return;
	}
	if (!file.open(QIODevice::ReadOnly)) {
		LOG(("Yukigram::Lang Info: file %1 could not be read.").arg(filename));
		return;
	}
	auto error = QJsonParseError{ 0, QJsonParseError::NoError };
	const auto document = QJsonDocument::fromJson(base::parse::stripComments(file.readAll()), &error);
	file.close();

	if (error.error != QJsonParseError::NoError) {
		LOG(("Yukigram::Lang Info: file %1 has failed to parse. Error: %2").arg(filename, error.errorString()));
		return;
	} else if (!document.isObject()) {
		LOG(("Yukigram::Lang Info: file %1 has failed to parse. Error: object expected").arg(filename));
		return;
	}

	for (const auto [key, value] : document.object().asKeyValueRange()) {
		if (key.front() == QChar('_')) {
			continue;
		}
		if (!value.isString()) {
			LOG(("Yukigram::Lang Info: wrong value for key %1 in file %2, string expected").arg(key, filename));
		}
		Values[key.toString()] = value.toString();
	}
}

} // namespace

QString TransformLangCode(QString &&langCode) {
	if (langCode.endsWith("-raw")) {
		return langCode.chopped(4);
	}
	return langCode;
}

void Reload() {
	BaseLangCode = TransformLangCode(::Lang::GetInstance().baseId());

	LangCode = TransformLangCode(::Lang::GetInstance().id());
	if (LangCode.isEmpty()) {
		LangCode = BaseLangCode;
	}

	Values.clear();

	if (BaseLangCode != kDefaultLanguage) {
		ParseLanguageData(kDefaultLanguage);
	}

	ParseLanguageData(BaseLangCode);

	if (LangCode != BaseLangCode) {
		ParseLanguageData(LangCode);
	}

	LangChanges.fire({});
}

TextWithEntities TranslateWithEntities(const QString &key, EntVar var1, EntVar var2, EntVar var3, EntVar var4) {
	if (!Values.contains(key)) {
		return Ui::Text::Italic(key);
	}
	TextWithEntities phrase = { Values[key] };

	for (const auto &v : { var1, var2, var3, var4 }) {
		if (!v.key.isEmpty()) {
			auto skipNext = false;
			const auto key = u"{%1}"_q.arg(v.key);
			const auto neededLength = phrase.text.length() - key.length();
			for (auto i = 0; i <= neededLength; i++) {
				if (skipNext) {
					skipNext = false;
					continue;
				}

				if (phrase.text.at(i) == QChar('\\')) {
					skipNext = true;
				} else if (phrase.text.at(i) == QChar('{') && phrase.text.mid(i, key.length()) == key) {
					phrase.text.replace(i, key.length(), v.value.text);
					const auto endOld = i + key.length();
					const auto endNew = i + v.value.text.length();

					// Shift existing entities
					if (endNew > endOld) {
						const auto diff = endNew - endOld;
						for (auto &entity : phrase.entities) {
							if (entity.offset() > endOld) {
								entity.shiftRight(diff);
							} else if (entity.offset() <= i && entity.offset() + entity.length() >= endOld) {
								entity.shrinkFromRight(-diff);
							}
						}
					} else if (endNew < endOld) {
						const auto diff = endOld - endNew;
						for (auto &entity : phrase.entities) {
							if (entity.offset() > endNew) {
								entity.shiftLeft(diff);
							} else if (entity.offset() <= i && entity.offset() + entity.length() >= endNew) {
								entity.shrinkFromRight(diff);
							}
						}
					}

					// Add new entities
					for (auto entity : v.value.entities) {
						phrase.entities.append(EntityInText(entity.type(), entity.offset() + i, entity.length(), entity.data()));
					}
					break;
				}
			}
		}
	}

	return phrase;
}

TextWithEntities TranslateWithEntities(const QString &key, float64 value, EntVar var1, EntVar var2, EntVar var3, EntVar var4) {
	const auto shift = ::Lang::Plural(0, value, lt_count).keyShift;
	return TranslateWithEntities(key + kPostfixes.at(shift), var1, var2, var3, var4);
}

rpl::producer<> Events() {
	return LangChanges.events();
}

} // namespace Lang
} // namespace Yukigram
