/*
This file is part of Kotatogram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/kotatogram/kotatogram-desktop/blob/dev/LEGAL
*/
#pragma once

namespace Yukigram {
namespace Lang {

struct Var {
	Var() = default;
	Var(const QString &k, const QString &v) : key(k), value(v) {}
	QString key;
	QString value;
};

struct EntVar {
	EntVar() = default;
	EntVar(const QString &k, const TextWithEntities &v) : key(k), value(v) {}
	EntVar(const Var &other) : key(other.key), value(other.value) {}
	QString key;
	TextWithEntities value;
};

void Reload();

TextWithEntities TranslateWithEntities(const QString &key,
	EntVar var1 = EntVar{},
	EntVar var2 = EntVar{},
	EntVar var3 = EntVar{},
	EntVar var4 = EntVar{});
TextWithEntities TranslateWithEntities(const QString &key, float64 value,
	EntVar var1 = EntVar{},
	EntVar var2 = EntVar{},
	EntVar var3 = EntVar{},
	EntVar var4 = EntVar{});

rpl::producer<> Events();

} // namespace Lang
} // namespace Yukigram

// Shorthands

inline TextWithEntities ktre(const QString &key,
	Yukigram::Lang::EntVar var1 = Yukigram::Lang::EntVar{},
	Yukigram::Lang::EntVar var2 = Yukigram::Lang::EntVar{},
	Yukigram::Lang::EntVar var3 = Yukigram::Lang::EntVar{},
	Yukigram::Lang::EntVar var4 = Yukigram::Lang::EntVar{}) {
	return Yukigram::Lang::TranslateWithEntities(key, var1, var2, var3, var4);
}

inline TextWithEntities ktre(const QString &key, float64 value,
	Yukigram::Lang::EntVar var1 = Yukigram::Lang::EntVar{},
	Yukigram::Lang::EntVar var2 = Yukigram::Lang::EntVar{},
	Yukigram::Lang::EntVar var3 = Yukigram::Lang::EntVar{},
	Yukigram::Lang::EntVar var4 = Yukigram::Lang::EntVar{}) {
	return Yukigram::Lang::TranslateWithEntities(key, value, var1, var2, var3, var4);
}

inline QString ktr(const QString &key,
	Yukigram::Lang::Var var1 = Yukigram::Lang::Var{},
	Yukigram::Lang::Var var2 = Yukigram::Lang::Var{},
	Yukigram::Lang::Var var3 = Yukigram::Lang::Var{},
	Yukigram::Lang::Var var4 = Yukigram::Lang::Var{}) {
	return ktre(key, var1, var2, var3, var4).text;
}

inline QString ktr(const QString &key, float64 value,
	Yukigram::Lang::Var var1 = Yukigram::Lang::Var{},
	Yukigram::Lang::Var var2 = Yukigram::Lang::Var{},
	Yukigram::Lang::Var var3 = Yukigram::Lang::Var{},
	Yukigram::Lang::Var var4 = Yukigram::Lang::Var{}) {
	return ktre(key, value, var1, var2, var3, var4).text;
}

inline rpl::producer<QString> rktr(const QString &key,
	Yukigram::Lang::Var var1 = Yukigram::Lang::Var{},
	Yukigram::Lang::Var var2 = Yukigram::Lang::Var{},
	Yukigram::Lang::Var var3 = Yukigram::Lang::Var{},
	Yukigram::Lang::Var var4 = Yukigram::Lang::Var{}) {
	return rpl::single(ktr(key, var1, var2, var3, var4)) | rpl::then(Yukigram::Lang::Events() | rpl::map([=]{
		return ktr(key, var1, var2, var3, var4);
	}));
}

inline rpl::producer<QString> rktr(const QString &key, float64 value,
	Yukigram::Lang::Var var1 = Yukigram::Lang::Var{},
	Yukigram::Lang::Var var2 = Yukigram::Lang::Var{},
	Yukigram::Lang::Var var3 = Yukigram::Lang::Var{},
	Yukigram::Lang::Var var4 = Yukigram::Lang::Var{}) {
	return rpl::single(ktr(key, value, var1, var2, var3, var4)) | rpl::then(Yukigram::Lang::Events() | rpl::map([=]{
		return ktr(key, value, var1, var2, var3, var4);
	}));
}

inline rpl::producer<TextWithEntities> rktre(const QString &key,
	Yukigram::Lang::EntVar var1 = Yukigram::Lang::EntVar{},
	Yukigram::Lang::EntVar var2 = Yukigram::Lang::EntVar{},
	Yukigram::Lang::EntVar var3 = Yukigram::Lang::EntVar{},
	Yukigram::Lang::EntVar var4 = Yukigram::Lang::EntVar{}) {
	return rpl::single(ktre(key, var1, var2, var3, var4)) | rpl::then(Yukigram::Lang::Events() | rpl::map([=]{
		return ktre(key, var1, var2, var3, var4);
	}));
}

inline rpl::producer<TextWithEntities> rktre(const QString &key, float64 value,
	Yukigram::Lang::EntVar var1 = Yukigram::Lang::EntVar{},
	Yukigram::Lang::EntVar var2 = Yukigram::Lang::EntVar{},
	Yukigram::Lang::EntVar var3 = Yukigram::Lang::EntVar{},
	Yukigram::Lang::EntVar var4 = Yukigram::Lang::EntVar{}) {
	return rpl::single(ktre(key, value, var1, var2, var3, var4)) | rpl::then(Yukigram::Lang::Events() | rpl::map([=]{
		return ktre(key, value, var1, var2, var3, var4);
	}));
}
