#include "request_queue.h"

vector<Document> RequestQueue::AddFindRequest(const string& raw_query, DocumentStatus status) {
    auto result = search_server_.FindTopDocuments(raw_query, status);
    AddResult(result);
    return result;
}
vector<Document> RequestQueue::AddFindRequest(const string& raw_query) {
    auto result = search_server_.FindTopDocuments(raw_query);
    AddResult(result);
    return result;
}
int RequestQueue::GetNoResultRequests() const {
    return empty_requests;
}

void RequestQueue::RemoveOld(){
    while(requests_.size() >= min_in_day_){
        QueryResult last = requests_.back();
        if(last.number_of_docs == 0){
            --empty_requests;
        }
        requests_.pop_back();
    }
}

void RequestQueue::AddResult(const vector<Document>& result){
    if(result.size() == 0){
        ++empty_requests;
    }
    RemoveOld();
    requests_.push_front({static_cast<int>(result.size())});
}
