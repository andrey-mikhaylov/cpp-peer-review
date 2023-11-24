void CheckDateTimeValidity(const DateTime& dt)
{
    auto ThrowIfInvalid = [](int value, const string& name, int min, int max) {
        if (value < min)
            throw domain_error(name + "is too small"s);

        if (value > max)
            throw domain_error(name + "is too big"s);
    };

    ThrowIfInvalid(dt.year,  "year"s,  1, 9999);
    ThrowIfInvalid(dt.month, "month"s, 1, 12);

    const bool is_leap_year = (dt.year % 4 == 0) && !(dt.year % 100 == 0 && dt.year % 400 != 0);
    const array month_lengths = {31, 28 + is_leap_year, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    ThrowIfInvalid(dt.day, "day"s, 1, month_lengths[dt.month-1]);

    ThrowIfInvalid(dt.hour,   "hour"s,   0, 23);
    ThrowIfInvalid(dt.minute, "minute"s, 0, 59);
    ThrowIfInvalid(dt.second, "second"s, 0, 59);
}
