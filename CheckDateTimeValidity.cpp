bool IsLeapYear(int year)
{
    return (year % 4 == 0) && !(year % 100 == 0 && year % 400 != 0);
}


int DaysInMonth(int year, int month)
{
    const array month_lengths = {31, IsLeapYear(year) ? 29 : 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    return month_lengths[month-1];
}


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
    ThrowIfInvalid(dt.day, "day"s, 1, DaysInMonth(dt.year, dt.month));
    ThrowIfInvalid(dt.hour,   "hour"s,   0, 23);
    ThrowIfInvalid(dt.minute, "minute"s, 0, 59);
    ThrowIfInvalid(dt.second, "second"s, 0, 59);
}
