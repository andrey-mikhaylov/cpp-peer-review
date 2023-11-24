// обрабатывает JSON-объект со списком городов конкретной страны
// вызывается только из функции ParseCountryJson
void ParseCitySubjson(vector<City>& cities_dst, const Json& cities_json, const Country& country)
{
    for (const auto& city_json : cities_json.AsList()) {
        const auto& city_obj = city_json.AsObject();
        cities_dst.push_back({
            .name             = city_obj["name"s].AsString(),
            .iso_code         = city_obj["iso_code"s].AsString(),
            .phone_code       = country.phone_code + city_obj["phone_code"s].AsString(),
            .country_name     = country.name,
            .country_iso_code = country.iso_code,
            .time_zone        = country.time_zone,
            .languages        = country.languages
        });
    }
}


// обрабатывает JSON-объект со списком языков конкретной страны
// вызывается только из функции ParseCountryJson
void ParseLanguagesList(vector<Language>& languages_dst, const Json& languages_json)
{
    for (const auto& lang_obj : languages_json.AsList()) {
        languages_dst.emplace_back(FromString<Language>(lang_obj.AsString()));
    }
}


void ParseCountryJson(vector<Country>& countries, vector<City>& cities, const Json& json)
{
    for (const auto& country_json : json.AsList()) {
        const auto& country_obj = country_json.AsObject();
        countries.push_back({
            .name       = country_obj["name"s].AsString(),
            .iso_code   = country_obj["iso_code"s].AsString(),
            .phone_code = country_obj["phone_code"s].AsString(),
            .time_zone  = country_obj["time_zone"s].AsString(),
        });
        Country& country = countries.back();
        ParseLanguagesList(country.languages, country_obj["languages"s]);
        ParseCitySubjson(cities, country_obj["cities"s], country);
    }
}
