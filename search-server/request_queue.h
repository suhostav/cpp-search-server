#pragma once
#include <deque>
#include <string>
#include <vector>

#include "document.h"
#include "search_server.h"

class RequestQueue {
public:
    explicit RequestQueue(const SearchServer& search_server): search_server_(search_server), empty_requests(0) {
    }
    // сделаем "обёртки" для всех методов поиска, чтобы сохранять результаты для нашей статистики
    template <typename DocumentPredicate>
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate) {
        auto result = search_server_.FindTopDocuments(raw_query, document_predicate);
        AddResult(result);
        return result;
    }
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentStatus status) ;
    std::vector<Document> AddFindRequest(const std::string& raw_query) ;
    int GetNoResultRequests() const ;
private:
    struct QueryResult {
        int number_of_docs;
    };
    std::deque<QueryResult> requests_;
    const static int min_in_day_ = 1440;
    const SearchServer& search_server_;
    int empty_requests;

    void RemoveOld();

    void AddResult(const std::vector<Document>& result);
};
