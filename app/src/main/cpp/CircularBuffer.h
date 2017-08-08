//
// Created by pgg on 07/08/17.
//

#ifndef APISCORE_CIRCULARBUFFER_H
#define APISCORE_CIRCULARBUFFER_H


#include <vector>
#include <algorithm>

template <typename T>
class CircularBuffer {
public:
    CircularBuffer(unsigned int capacity) : begin_(0), size_(0), capacity_(capacity), data_(capacity, 0) {}

    unsigned long size() const { return size_; }

    T& operator[](unsigned int index)
    {
        return data_[bufferIndex_to_vectorIndex(index)];
    }

    T const& operator[](unsigned int index) const
    {
        return data_[bufferIndex_to_vectorIndex(index)];
    }

    void push(T const& value)
    {
        if(size_ == capacity_) {
            // buffer is full -> rotate
            data_[begin_] = value;
            if(++begin_ == capacity_)
                begin_ = 0;
        }
        else {
            // add value at the end, increase size
            unsigned int idxInsert = bufferIndex_to_vectorIndex(size_);

            data_[idxInsert] = value;
            size_++;
        }
    }

    void push(std::vector<T> const& array)
    {
        unsigned int copyLength = std::min(capacity_, (unsigned int) array.size());
        auto src_it = array.end() - copyLength;
        auto dst_it = data_.begin() + bufferIndex_to_vectorIndex(size_);

        // copy data
        while(src_it != array.end()) {
            *dst_it = *src_it;
            ++src_it;
            if(++dst_it == data_.end())
                dst_it = data_.begin();
        }

        // Update size and begin
        if(copyLength + size_ > capacity_) {
            size_ = capacity_;
            begin_ = (begin_+size_+copyLength) % capacity_;
        }
        else {
            size_ += copyLength;
        }
    }

private:
    //! @param index must be < capacity_
    unsigned int bufferIndex_to_vectorIndex(unsigned int index) const
    {
        unsigned int i = index + begin_;
        if (i >= capacity_)
            i -= capacity_;
        return i;
    }

    unsigned int begin_;
    unsigned int size_;
    unsigned capacity_;
    std::vector<T> data_;
};


#endif //APISCORE_CIRCULARBUFFER_H
