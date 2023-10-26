#pragma once
#include <cstddef>
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
    Paginator(It b, It e, size_t size): begin_(b), end_(e), size_(static_cast<long long>(size)){
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
    std::vector<IteratorRange<It>> pages_;
};

template <typename Container>
auto Paginate(const Container& c, int page_size) {
    return Paginator(begin(c), end(c), static_cast<size_t>(page_size));
}

template <typename It>
ostream& operator<<(ostream& os, IteratorRange<It> page){
    for(It i = page.begin(); i != page.end(); ++i){
        os << "{ " << *i << " }";
    }
    return os;
}
