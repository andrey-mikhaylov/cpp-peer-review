#include <iostream>
#include <iomanip>
#include <string>
#include <vector>


namespace detail
{

std::string ReadLine(std::istream& input)
{
    std::string s;
    getline(input, s);
    return s;
}


int ReadLineWithNumber(std::istream& input)
{
    int result;
    input >> result;
    ReadLine(input);
    return result;
}


std::vector<std::string_view> SplitIntoWords(std::string_view text)
{
    std::vector<std::string_view> words;
    while (true)
    {
        while (text.find(' ') == 0) {
            text.remove_prefix(1);
        }

        if (text.empty()) {
            break;
        }

        auto space = text.find(' ');
        if (space == std::string::npos) {
            words.push_back(text);
            break;
        }

        words.push_back(text.substr(0, space));
        text.remove_prefix(space);
    }

    return words;
}

} // namespace detail


class BookReaders
{
public:
    void ProcessRequests(std::istream& input, std::ostream& output)
    {
        output << std::setprecision(6);

        int requests_count = detail::ReadLineWithNumber(input);
        for (int i = 0; i < requests_count; i++) {
            auto line = detail::ReadLine(input);
            auto words = detail::SplitIntoWords(line);

            if (words.size() == 2 && words[0] == "CHEER") {
                output << Cheer(stoi(std::string(words[1]))) << std::endl;
            }

            else if (words.size() == 3 && words[0] == "READ") {
                Read(stoi(std::string(words[1])), stoi(std::string(words[2])));
            }
        }
    }

private:
    std::vector<int> pages_;    // from reader id
    std::vector<int> readers_;  // from page
    int count_ = 0;

    double Cheer(int reader)
    {
        if (pages_.size() <= reader) {
            pages_.resize(reader + 1);
        }
        int page = pages_[reader];

        if (readers_.size() <= page) {
            readers_.resize(page+1);
        }

        if (count_ == 0) {
            return 0.0;
        }

        if (count_ == 1) {
            if (page == 0)
                return 0.0;
            return 1.0;
        }

        if (page == 0) {
            return 0;
        }

        int readers_with_less_pages = count_ - readers_[page];
        return double(readers_with_less_pages) / (count_ - 1);
    }

    void Read(int reader, int new_page)
    {
        if (pages_.size() <= reader) {
            pages_.resize(reader + 1);
        }

        if (readers_.size() <= new_page) {
            readers_.resize(new_page+1);
        }

        for (int page = pages_[reader]+1; page <= new_page; page++) {
            readers_[page]++;
        }

        if (pages_[reader] == 0) {
            count_++;
        }

        pages_[reader] = new_page;
    }
};


int main()
{
    BookReaders book_readers;
    book_readers.ProcessRequests(std::cin, std::cout);
}
