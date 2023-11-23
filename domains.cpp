#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>

using namespace std;


class Domain 
{
public:
    explicit Domain(const string& url) : url_(url)
    {
        reverse(url_.begin(), url_.end());
        url_.push_back('.');
    }

    bool operator==(const Domain& other) const
    {
        return url_ == other.url_;
    }

    bool operator<(const Domain& other) const
    {
        return url_ < other.url_;
    }

    // Принимает другой домен и возвращает true, если this его поддомен
    bool IsSubDomain(const Domain& main) const
    {
        if (url_.size() < main.url_.size())
            return false;
        auto [mismatched,_]  = std::mismatch(main.url_.begin(), main.url_.end(), url_.begin());
        return mismatched == main.url_.end();
    }

private:
    string url_;
};


class DomainChecker 
{
public:
    template <typename Iterator>
    DomainChecker(Iterator first, Iterator last) : forbidden_(first, last)
    {
        std::sort(forbidden_.begin(), forbidden_.end());
        last = std::unique(forbidden_.begin(), forbidden_.end(), [](const Domain& lhs, const Domain& rhs)
        {
            return rhs.IsSubDomain(lhs);
        });
        forbidden_.erase(last, forbidden_.end());
    }

    // Возвращает true, если домен запрещён
    bool IsForbidden(const Domain& domain)
    {
        auto it = std::upper_bound(forbidden_.begin(), forbidden_.end(), domain);
        if (it == forbidden_.begin())
            return false;
        return domain.IsSubDomain(*prev(it));
    }

private:
    vector<Domain> forbidden_;
};


vector<Domain> ReadDomains(istream& in, int count)
{
    vector<Domain> domains;
    domains.reserve(count);
    for (auto i = 0; i < count; i++) {
        string domain;
        getline(in, domain);
        domains.emplace_back(domain);
    }
    return domains;
}


template <typename Number>
Number ReadNumberOnLine(istream& input) 
{
    string line;
    getline(input, line);

    Number num;
    std::istringstream(line) >> num;

    return num;
}


int main() 
{
    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}
