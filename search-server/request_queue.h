#pragma once
#include <deque>
#include <string>
#include <vector>

#include "document.h"
#include "search_server.h"

using std::deque, std::string, std::vector;

class RequestQueue {
public:
    explicit RequestQueue(const SearchServer& search_server): search_server_(search_server), empty_requests(0) {
    }
    // сделаем "обёртки" для всех методов поиска, чтобы сохранять результаты для нашей статистики
    template <typename DocumentPredicate>
    vector<Document> AddFindRequest(const string& raw_query, DocumentPredicate document_predicate) {
        auto result = search_server_.FindTopDocuments(raw_query, document_predicate);
        AddResult(result);
        return result;
    }
    vector<Document> AddFindRequest(const string& raw_query, DocumentStatus status) ;
    vector<Document> AddFindRequest(const string& raw_query) ;
    int GetNoResultRequests() const ;
private:
    struct QueryResult {
        int number_of_docs;
    };
    deque<QueryResult> requests_;
    const static int min_in_day_ = 1440;
    const SearchServer& search_server_;
    int empty_requests;

    void RemoveOld();

    void AddResult(const vector<Document>& result);
};
