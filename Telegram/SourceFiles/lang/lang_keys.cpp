/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#include "lang/lang_keys.h"

#include "base/const_string.h"
#include "lang/lang_file_parser.h"
#include "ui/integration.h"
#include "yukigram/settings/time_with_seconds.h"
#include "yukigram/settings/solar_date.h"

#include <QtCore/QLocale>

namespace {

constexpr auto kDefaultLanguage = "en"_cs;

struct SolarHijriDate {
        int year = 0;
        int month = 0;
        int day = 0;
};

[[nodiscard]] SolarHijriDate GregorianToSolarHijri(const QDate &date) {
        const auto gy = date.year();
        const auto gm = date.month();
        const auto gd = date.day();

        const int gdm[12] = {
                0, 31, 59, 90, 120, 151,
                181, 212, 243, 273, 304, 334
        };

        auto jy = (gy <= 1600) ? 0 : 979;
        const auto gy2 = gy - ((gy <= 1600) ? 621 : 1600);
        const auto gyAdjusted = (gm > 2) ? (gy2 + 1) : gy2;

        auto days = 365 * gy2
                + (gyAdjusted + 3) / 4
                - (gyAdjusted + 99) / 100
                + (gyAdjusted + 399) / 400
                - 80
                + gd
                + gdm[gm - 1];

        jy += 33 * (days / 12053);
        days %= 12053;

        jy += 4 * (days / 1461);
        days %= 1461;

        if (days > 365) {
                jy += (days - 1) / 365;
                days = (days - 1) % 365;
        }

        const auto jm = (days < 186)
                ? (1 + days / 31)
                : (7 + (days - 186) / 30);

        const auto jd = 1 + ((days < 186)
                ? (days % 31)
                : ((days - 186) % 30));

        return { jy, jm, jd };
}

[[nodiscard]] QString SolarMonthName(int month) {
        static const QStringList names = {
                QString::fromUtf8("\xD9\x81\xD8\xB1\xD9\x88\xD8\xB1\xD8\xAF\xDB\x8C\xD9\x86"),
                QString::fromUtf8("\xD8\xA7\xD8\xB1\xD8\xAF\xDB\x8C\xD8\xA8\xD9\x87\xD8\xB4\xD8\xAA"),
                QString::fromUtf8("\xD8\xAE\xD8\xB1\xD8\xAF\xD8\xA7\xD8\xAF"),
                QString::fromUtf8("\xD8\xAA\xDB\x8C\xD8\xB1"),
                QString::fromUtf8("\xD9\x85\xD8\xB1\xD8\xAF\xD8\xA7\xD8\xAF"),
                QString::fromUtf8("\xD8\xB4\xD9\x87\xD8\xB1\xDB\x8C\xD9\x88\xD8\xB1"),
                QString::fromUtf8("\xD9\x85\xD9\x87\xD8\xB1"),
                QString::fromUtf8("\xD8\xA2\xD8\xA8\xD8\xA7\xD9\x86"),
                QString::fromUtf8("\xD8\xA2\xD8\xB0\xD8\xB1"),
                QString::fromUtf8("\xD8\xAF\xDB\x8C"),
                QString::fromUtf8("\xD8\xA8\xD9\x87\xD9\x85\xD9\x86"),
                QString::fromUtf8("\xD8\xA7\xD8\xB3\xD9\x81\xD9\x86\xD8\xAF")
        };

        return (month >= 1 && month <= 12)
                ? names[month - 1]
                : u"MONTH_ERR"_q;
}

[[nodiscard]] QString PersianDigits(QString text) {
        static const QString latin = u"0123456789"_q;
        static const QString persian = QString::fromUtf8(
                "\xDB\xB0\xDB\xB1\xDB\xB2\xDB\xB3\xDB\xB4"
                "\xDB\xB5\xDB\xB6\xDB\xB7\xDB\xB8\xDB\xB9");

        for (auto i = 0; i != 10; ++i) {
                text.replace(latin[i], persian[i]);
        }
        return text;
}

[[nodiscard]] QString SolarDatePretty(const QDate &date) {
        const auto solar = GregorianToSolarHijri(date);

        static const auto months = std::array{
                u"Farvardin"_q,
                u"Ordibehesht"_q,
                u"Khordad"_q,
                u"Tir"_q,
                u"Mordad"_q,
                u"Shahrivar"_q,
                u"Mehr"_q,
                u"Aban"_q,
                u"Azar"_q,
                u"Dey"_q,
                u"Bahman"_q,
                u"Esfand"_q,
        };

        if (solar.month < 1 || solar.month > 12) {
                return QString::number(solar.day);
        }

        return QString::number(solar.day)
                + u" "_q
                + months[solar.month - 1];
}
template <typename WithYear, typename WithoutYear>
inline QString langDateMaybeWithYear(
		QDate date,
		WithYear withYear,
		WithoutYear withoutYear) {
	const auto month = date.month();
	if (month <= 0 || month > 12) {
		return u"MONTH_ERR"_q;
	};
	const auto year = date.year();
	const auto current = QDate::currentDate();
	const auto currentYear = current.year();
	const auto currentMonth = current.month();
	if (year != currentYear) {
		const auto yearIsMuchGreater = [](int year, int otherYear) {
			return (year > otherYear + 1);
		};
		const auto monthIsMuchGreater = [](
				int year,
				int month,
				int otherYear,
				int otherMonth) {
			return (year == otherYear + 1) && (month + 12 > otherMonth + 3);
		};
		if (false
			|| yearIsMuchGreater(year, currentYear)
			|| yearIsMuchGreater(currentYear, year)
			|| monthIsMuchGreater(year, month, currentYear, currentMonth)
			|| monthIsMuchGreater(currentYear, currentMonth, year, month)) {
			return withYear(month, year);
		}
	}
	return withoutYear(month, year);
}

using namespace Lang;

} // namespace

bool langFirstNameGoesSecond() {
	const auto kFirstName = QChar(0x0001);
	const auto kLastName = QChar(0x0002);
	const auto fullname = tr::lng_full_name(
		tr::now,
		lt_first_name,
		QString(1, kFirstName),
		lt_last_name,
		QString(1, kLastName));
	return fullname.indexOf(kLastName) < fullname.indexOf(kFirstName);
}

QString langFullName(
		const QString &firstName,
		const QString &lastName) {
	if (firstName.isEmpty()) {
		return lastName;
	} else if (lastName.isEmpty()) {
		return firstName;
	}
	return langFirstNameGoesSecond()
		? (lastName + u' ' + firstName)
		: (firstName + u' ' + lastName);
}

QString langDayOfMonth(const QDate &date) {
        if (Yukigram::Settings::SolarDate->current()) {
                return SolarDatePretty(date);
        }

        auto day = date.day();
	return langDateMaybeWithYear(date, [&](int month, int year) {
		return tr::lng_month_day_year(
			tr::now,
			lt_month,
			MonthSmall(month)(tr::now),
			lt_day,
			QString::number(day),
			lt_year,
			QString::number(year));
	}, [day](int month, int year) {
		return tr::lng_month_day(
			tr::now,
			lt_month,
			MonthSmall(month)(tr::now),
			lt_day,
			QString::number(day));
	});
}

QString langDayOfMonthFull(const QDate &date) {
        if (Yukigram::Settings::SolarDate->current()) {
                return SolarDatePretty(date);
        }

        auto day = date.day();
	return langDateMaybeWithYear(date, [day](int month, int year) {
		return tr::lng_month_day_year(
			tr::now,
			lt_month,
			MonthDay(month)(tr::now),
			lt_day,
			QString::number(day),
			lt_year,
			QString::number(year));
	}, [day](int month, int year) {
		return tr::lng_month_day(
			tr::now,
			lt_month,
			MonthDay(month)(tr::now),
			lt_day,
			QString::number(day));
	});
}

QString langDayOfMonthShort(const QDate &date) {
	auto day = date.day();
	return langDateMaybeWithYear(date, [&](int month, int year) {
		return QLocale().toString(date, QLocale::ShortFormat);
	}, [day](int month, int year) {
		return tr::lng_month_day(
			tr::now,
			lt_month,
			MonthSmall(month)(tr::now),
			lt_day,
			QString::number(day));
	});
}

QString langMonthOfYear(int month, int year) {
	return (month > 0 && month <= 12)
		? tr::lng_month_year(
			tr::now,
			lt_month,
			MonthSmall(month)(tr::now),
			lt_year,
			QString::number(year))
		: u"MONTH_ERR"_q;
}

QString langMonth(const QDate &date) {
	return langDateMaybeWithYear(date, [](int month, int year) {
		return langMonthOfYear(month, year);
	}, [](int month, int year) {
		return MonthSmall(month)(tr::now);
	});
}

QString langMonthOfYearFull(int month, int year) {
	return (month > 0 && month <= 12)
		? tr::lng_month_year(
			tr::now,
			lt_month,
			Month(month)(tr::now),
			lt_year,
			QString::number(year))
		: u"MONTH_ERR"_q;
}

QString langMonthFull(const QDate &date) {
	return langDateMaybeWithYear(date, [](int month, int year) {
		return langMonthOfYearFull(month, year);
	}, [](int month, int year) {
		return Month(month)(tr::now);
	});
}

QString langDayOfWeek(int index) {
	return (index > 0 && index <= 7)
		? Weekday(index)(tr::now)
		: u"DAY_ERR"_q;
}

QString langDayOfWeekFull(int index) {
	return (index > 0 && index <= 7)
		? WeekdayFull(index)(tr::now)
		: u"DAY_ERR"_q;
}

QString langDateTime(const QDateTime &date) {
	return tr::lng_mediaview_date_time(
		tr::now,
		lt_date,
		langDayOfMonth(date.date()),
		lt_time,
		QLocale().toString(date.time(), Lang::TimeFormat()));
}

QString langDateTimeFull(const QDateTime &date) {
	return tr::lng_mediaview_date_time(
		tr::now,
		lt_date,
		langDayOfMonthFull(date.date()),
		lt_time,
		QLocale().toString(date.time(), Lang::TimeFormat()));
}

namespace Lang {

QString longTimeFormat() {
	return QLocale::system().timeFormat(QLocale::LongFormat)
		.remove("t") // Convert to Medium format
		.remove("[]") // Fix for `yue`
		.remove("()") // Fix for `fa`
		.simplified();
}

QString shortTimeFormat() {
	return QLocale::system().timeFormat(QLocale::ShortFormat);
}

QString TimeFormat() {
	return Yukigram::Settings::TimeWithSeconds->current() ? longTimeFormat() : shortTimeFormat();
}

QString DateTimeFormat() {
	auto format = QLocale::system().dateTimeFormat(QLocale::ShortFormat);
	if (Yukigram::Settings::TimeWithSeconds->current()) {
		format.replace(shortTimeFormat(), longTimeFormat());
	}
	return format;
}

QString DefaultLanguageId() {
	return kDefaultLanguage.utf16();
}

QString LanguageIdOrDefault(const QString &id) {
	return !id.isEmpty() ? id : DefaultLanguageId();
}

tr::phrase<> Month(int index) {
	switch (index) {
	case 1: return tr::lng_month1;
	case 2: return tr::lng_month2;
	case 3: return tr::lng_month3;
	case 4: return tr::lng_month4;
	case 5: return tr::lng_month5;
	case 6: return tr::lng_month6;
	case 7: return tr::lng_month7;
	case 8: return tr::lng_month8;
	case 9: return tr::lng_month9;
	case 10: return tr::lng_month10;
	case 11: return tr::lng_month11;
	case 12: return tr::lng_month12;
	}
	Unexpected("Index in MonthSmall.");
}

tr::phrase<> MonthSmall(int index) {
	switch (index) {
	case 1: return tr::lng_month1_small;
	case 2: return tr::lng_month2_small;
	case 3: return tr::lng_month3_small;
	case 4: return tr::lng_month4_small;
	case 5: return tr::lng_month5_small;
	case 6: return tr::lng_month6_small;
	case 7: return tr::lng_month7_small;
	case 8: return tr::lng_month8_small;
	case 9: return tr::lng_month9_small;
	case 10: return tr::lng_month10_small;
	case 11: return tr::lng_month11_small;
	case 12: return tr::lng_month12_small;
	}
	Unexpected("Index in MonthSmall.");
}

tr::phrase<> MonthDay(int index) {
	switch (index) {
	case 1: return tr::lng_month_day1;
	case 2: return tr::lng_month_day2;
	case 3: return tr::lng_month_day3;
	case 4: return tr::lng_month_day4;
	case 5: return tr::lng_month_day5;
	case 6: return tr::lng_month_day6;
	case 7: return tr::lng_month_day7;
	case 8: return tr::lng_month_day8;
	case 9: return tr::lng_month_day9;
	case 10: return tr::lng_month_day10;
	case 11: return tr::lng_month_day11;
	case 12: return tr::lng_month_day12;
	}
	Unexpected("Index in MonthDay.");
}

tr::phrase<> Weekday(int index) {
	switch (index) {
	case 1: return tr::lng_weekday1;
	case 2: return tr::lng_weekday2;
	case 3: return tr::lng_weekday3;
	case 4: return tr::lng_weekday4;
	case 5: return tr::lng_weekday5;
	case 6: return tr::lng_weekday6;
	case 7: return tr::lng_weekday7;
	}
	Unexpected("Index in Weekday.");
}

tr::phrase<> WeekdayFull(int index) {
	switch (index) {
	case 1: return tr::lng_hours_monday;
	case 2: return tr::lng_hours_tuesday;
	case 3: return tr::lng_hours_wednesday;
	case 4: return tr::lng_hours_thursday;
	case 5: return tr::lng_hours_friday;
	case 6: return tr::lng_hours_saturday;
	case 7: return tr::lng_hours_sunday;
	}
	Unexpected("Index in WeekdayFull.");
}

} // namespace Lang
