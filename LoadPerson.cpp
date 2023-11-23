enum class DBLogLevel {
    DISABLED,
    NORMAL,
    VERBOSE,
};


enum class DBExceptions {
    DISALLOWED,
    ALLOWED
};


struct TimeoutMilliseconds {
    int value;
    explicit TimeoutMilliseconds(int v) : value(v) {}
    operator int() const {return value;}
};

auto operator""_ms(unsigned long long value) {
    return TimeoutMilliseconds(value);
}


struct Years {
    int value;
    explicit Years(int v) : value(v) {}
    operator int() const {return value;}
};

auto operator""_years(unsigned long long value) {
    return Years(value);
}


struct DBSettings {
    string_view db_name;
    TimeoutMilliseconds db_connection_timeout;
    DBExceptions db_allow_exceptions;
    DBLogLevel db_log_level;
};


struct PersonFilter {
    Years min_age;
    Years max_age;
    string_view name_filter;
};


bool ConnectDB(const DBSettings& db_settings, DBHandler& db)
{
    DBConnector connector(db_settings.db_allow_exceptions, db_settings.db_log_level);
    if (db_settings.db_name.starts_with("tmp."s)) {
        db = connector.ConnectTmp(db_settings.db_name, db_settings.db_connection_timeout);
    } else {
        db = connector.Connect(db_settings.db_name, db_settings.db_connection_timeout);
    }

    if (db_settings.db_allow_exceptions == DBExceptions::ALLOWED) {
        return true;
    }

    return db.IsOK();
}


DBQuery MakeQuery(const PersonFilter& filter, string_view quote) {
    ostringstream query_str;
    query_str << "from Persons "s
              << "select Name, Age "s
              << "where Age between "s << filter.min_age << " and "s << filter.max_age << " "s
              << "and Name like '%"s << quote << "%'"s;
    return {DBQuery(query_str.str())};
}


// call example:
// DBSettings db_settings {"db_name", 100_ms, DBExceptions::ALLOWED, DBLogLevel::VERBOSE};
// PersonFilter person_filter {25_years, 30_years, "*Ivan*"};
// LoadPersons(db_settings, person_filter);
vector<Person> LoadPersons(const DBSettings& db_settings, const PersonFilter filter)
{
    DBHandler db;
    if (!ConnectDB(db_settings, db)) {
        return {};
    }

    DBQuery query = MakeQuery(filter, db.Quote(filter.name_filter));

    vector<Person> persons;
    for (auto [name, age] : db.LoadRows<string, int>(query)) {
        persons.push_back({std::move(name), age});
    }

    return persons;
}

