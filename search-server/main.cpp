#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

string ReadLine() {
    string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber() {
    int result = 0;
    cin >> result;
    ReadLine();
    return result;
}

vector<string> SplitIntoWords(const string& text) {
    vector<string> words;
    string word;
    for (const char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        }
        else {
            word += c;
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }

    return words;
}

struct Document {
    int id;
    double relevance;
};

struct Query {
    set<string> plus_words;
    set<string> minus_words;

    //add check the word if it present or not here
    bool contains(const string& w) const {
        return minus_words.count(w) == 0 && plus_words.count(w) == 1;
    }
};

class SearchServer {
public:
    void SetStopWords(const string& text) {
        for (const string& word : SplitIntoWords(text)) {
            stop_words_.insert(word);
        }
    }

    void SetDocumentCount(int n) {
        document_count_ = n;
    }

    void AddDocument(int document_id, const string& document) {
        const vector<string> words = SplitIntoWordsNoStop(document);
        double ratio = 1. / static_cast<double>(words.size());
        for (const string& word : words) {
            word_to_document_freqs_[word][document_id] += ratio;
        }

    }

    vector<Document> FindTopDocuments(const string& raw_query) const {
        const Query query = ParseQuery(raw_query);
        auto matched_documents = FindAllDocuments(query);

        sort(matched_documents.begin(), matched_documents.end(),
            [](const Document& lhs, const Document& rhs) {
                return lhs.relevance > rhs.relevance;
            });
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;
    }

private:

    map<string, map<int, double>> word_to_document_freqs_;
    int document_count_ = 0;

    set<string> stop_words_;

    bool IsStopWord(const string& word) const {
        return stop_words_.count(word) > 0;
    }

    vector<string> SplitIntoWordsNoStop(const string& text) const {
        vector<string> words;
        for (const string& word : SplitIntoWords(text)) {
            if (!IsStopWord(word)) {
                words.push_back(word);
            }
        }
        return words;
    }

    Query ParseQuery(const string& text) const {
        Query query;
        vector<string> words = SplitIntoWordsNoStop(text);
        //process minus words
        for (const string& word : words) {
            if (!word.empty() && word[0] == '-') {
                query.minus_words.insert(word.substr(1));
            }
        }
        for (const string& word : words) {
            if (!word.empty() && word[0] != '-' && query.minus_words.count(word) == 0)
                query.plus_words.insert(word);
        }
        return query;
    }

    double CalcIDF(int docs_count) const {
        return log(static_cast<double>(document_count_) / static_cast<double>(docs_count));
    }

    vector<Document> FindAllDocuments(const Query& query) const {
        vector<Document> matched_documents;
        map<int, double> matches;
        //add plus words
        for (const auto& plus_word : query.plus_words) {
            if (word_to_document_freqs_.count(plus_word)) {
                double IDF = CalcIDF(word_to_document_freqs_.at(plus_word).size());
                for (auto [doc_id, TF] : word_to_document_freqs_.at(plus_word)) {
                    matches[doc_id] += IDF * TF;
                }
            }
        }
        //remove minus words
        for (const auto& minus_word : query.minus_words)
            if (word_to_document_freqs_.count(minus_word)) {
                for (const auto& [id, TF] : word_to_document_freqs_.at(minus_word))
                    if (matches.count(id) == 1)
                        matches.erase(id);
            }
        for (const auto& [id, rel] : matches)
            matched_documents.push_back({ id, rel });

        return matched_documents;
    }

};

SearchServer CreateSearchServer() {
    SearchServer search_server;
    search_server.SetStopWords(ReadLine());

    const int document_count = ReadLineWithNumber();
    search_server.SetDocumentCount(document_count);
    for (int document_id = 0; document_id < document_count; ++document_id) {
        search_server.AddDocument(document_id, ReadLine());
    }

    return search_server;
}

int main() {
    const SearchServer search_server = CreateSearchServer();

    const string query = ReadLine();
    for (const auto& [document_id, relevance] : search_server.FindTopDocuments(query)) {
        cout << "{ document_id = "s << document_id << ", "
            << "relevance = "s << relevance << " }"s << endl;
    }
}