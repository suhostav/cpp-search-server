#pragma once
#include <ostream>
#include "document.h"

using std::ostream;


template <typename It>
class IteratorRange{
public:
    IteratorRange(It b, It e): begin_(b), end_(e){}
    It begin(){
        return begin_;
    }
    It end(){
        return end_;
    }
private:
    It begin_;
    It end_;
};

template <typename It>
class Paginator{
public:
    Paginator(It b, It e, long long size): begin_(b), end_(e), size_(size){
        It page_begin = begin_;
        for(It i = begin_; i != end_; ++i){
            long long dist = distance(page_begin, i);
            if(dist  == size_){
                pages_.push_back(IteratorRange(page_begin, i));
                page_begin = i;
            }
        }
        if(page_begin != end_)
            pages_.push_back(IteratorRange(page_begin, end_));
    } 

    auto begin() const{
        return pages_.begin();
    }
    auto end() const{
        return pages_.end();
    }
private:
    It begin_;
    It end_;
    long long size_;
    vector<IteratorRange<It>> pages_;
};

template <typename Container>
auto Paginate(const Container& c, size_t page_size) {
    return Paginator(begin(c), end(c), page_size);
}

ostream& operator<<(ostream& os, const Document& doc){
    using namespace std;
    os << "document_id = "s << doc.id << ", relevance = " << doc.relevance << ", rating = " << doc.rating;
    return os;
}

template <typename It>
ostream& operator<<(ostream& os, IteratorRange<It> page){
    for(It i = page.begin(); i != page.end(); ++i){
        os << "{ " << *i << " }";
    }
    return os;
}
