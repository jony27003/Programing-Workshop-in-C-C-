#include <vector>
#include <cmath>
#include <algorithm>
#include <cassert>

#define DEFAULT_CAPACITY 16
#define UPPER_LOAD_FACTOR 0.75
#define LOWER_LOAD_FACTOR 0.25

using std::vector;
using std::pair;
using std::exception;
using std::make_pair;
using std::hash;
using std::find;




/**
 * @brief out of range exception
 */
class OutOfRange : public exception
{
public:
    /**
     * @brief out of range exception message.
     * @return out of range exception message.
     */
    const char* what() const throw() {return "This argument is out of range\n"; }
};




/**
 * @brief HashMap Class
 */
template<typename KeyT, typename ValueT>
class HashMap
{
private:
    /** number of elements in hash map */
    int _size;

    /** max capacity of the hash map */
    int _capacity;

    /** a bucket of pairs */
    template <typename KEY, typename VALUE>
    using bucket = vector<pair<KEY, VALUE>>;
    vector<bucket<KeyT, ValueT>> _pairBuckets;

    /** the upper load factor */
    double _upperLoadFactor;

    /** the upper load factor */
    double _lowerLoadFactor;

    /**
    * @brief rehashes this maps elements into a new one
    * @param newCapacity the hashMaps new capacity
    */
    void _fixHashMap(int newCapacity)
    {

        HashMap<KeyT, ValueT> temp = *this;
        this->_pairBuckets = vector<bucket<KeyT, ValueT>>((unsigned long)newCapacity);
        for(bucket<KeyT, ValueT> &in : this->_pairBuckets)
        {
            in.clear();
        }
        _size = 0;
        _capacity = newCapacity;
        for(const_iterator in = temp.cbegin(); in != temp.cend(); ++in)
        {
            this->insert(in->first, in->second);
        }
    }

    /**
    * @brief finds the hash of a givven key
    * @param key the key to hash
    * @return the hash
    */
    int _hashKey(KeyT key) const {return (hash<KeyT> {}(key) & (_capacity - 1)); }

    /**
    * @brief checks if the given bounds are valid
    * @param low the low bound
    * @param high the high bound
    * @return true if valid false otherwise
    */
    bool _validBounds(double low, double high)
    {
        return (low < 1) && (high < 1) && (low > 0)  && (high > 0) && (low < high);
    }
public:
    /**
    * @brief constructor
    * @param low the low bound
    * @param high the high bound
    */
    HashMap(double low = LOWER_LOAD_FACTOR, double high = UPPER_LOAD_FACTOR):
            _size(0),
            _capacity(DEFAULT_CAPACITY),
            _pairBuckets(vector<bucket<KeyT, ValueT>>(DEFAULT_CAPACITY)),
            _upperLoadFactor(high),
            _lowerLoadFactor(low)

    {
        if(!_validBounds(low, high))
        {
            throw OutOfRange();
        }
    }


    /**
    * @brief second constructor, gets two key and value vectors and saves them accordingly
    * @param keys the keys vector
    * @param values the values vector
    */
    HashMap(const vector<KeyT> &keys, const vector<ValueT> &values): HashMap()
    {
        if(keys.size() != values.size())
        {
            throw OutOfRange();
        }
        for(int i = 0; i < keys.size(); ++i)
        {
            insert(keys[i], values[i]);
        }
    }


    /** default */
    HashMap(const HashMap &other) = default;

    /**
    * @brief move constructor
    * @param other the hashMap to move
    */
    HashMap(HashMap<KeyT, ValueT> && other) noexcept
    {
        *this = other;
        other.clear();
    }

    /** default */
    ~HashMap() {};

    /** default */
    HashMap& operator=(const HashMap &other) = default;

    /**
    * @brief move operator
    * @param other the hashmap to move
    */
    HashMap& operator=(HashMap<KeyT, ValueT> && other)
    {
        *this = other;
        other.clear();
    }


    /**
    * @brief operator [] overload
    * @param key the key in the hashMap
    * @return the value of the given key
    */
    ValueT& operator[](KeyT key) noexcept
    {
        try
        {
            return this->at(key);
        }
        catch (...)
        {
            this->_pairBuckets[_hashKey(key)].push_back(make_pair(key, ValueT()));
            ++this->_size;
            return _pairBuckets[_hashKey(key)][_pairBuckets[_hashKey(key)].size() - 1].second;
        }
    }


    /**
    * @brief const operator [] overload
    * @param key the key in the hashMap
    * @return the value of the given key by value
    */
    const ValueT operator[](const KeyT key) const noexcept {return this->at(key); }


    /** operator overload */
    bool operator==(const HashMap &other) const
    {
        if((this->_size == other._size) && (this->_capacity == other._capacity) &&
           (this->_upperLoadFactor == other._upperLoadFactor) &&
           (this->_lowerLoadFactor == other._lowerLoadFactor))
        {
            return (this->_pairBuckets == other._pairBuckets);
        }
        return false;
    }


    /** operator overload */
    bool operator!=(const HashMap &other) const {return !(*this == other); }


    /**
    * @brief returns the number of elements in this hashMap
    * @return the number of elements in this hashMap
    */
    int size() { return _size; }


    /**
    * @brief returns the capacity of this hashMap
    * @return the capacity of this hashMap
    */
    int capacity() { return _capacity; }


    /**
    * @brief returns the current load factor of this hashMap
    * @return the current load factor of this hashMap
    */
    double getLoadFactor() { return ((double)this->_size) / ((double)this->_capacity); }


    /**
    * @brief checks if this hash map is empty
    * @return true if empty false othewise
    */
    bool empty() { return !this->_size; }


    /**
    * @brief inserts a given key and value
    * @param key the key
    * @param  value the value
    * @return true if the insert was successful or false otherwise
    */
    bool insert(KeyT key, ValueT value)
    {
        if(this->containsKey(key))
        {
            for(pair<KeyT, ValueT> &in : this->_pairBuckets[_hashKey(key)])
            {
                if(in.first == key)
                {
                    in.second = value;
                    return false;
                }
            }
        }
        if((((double) _size + 1) / _capacity) > _upperLoadFactor)
        {
            _fixHashMap(_capacity * 2);
        }
        this->_pairBuckets[_hashKey(key)].push_back(make_pair(key, value));
        ++_size;
        return true;
    }



    /**
    * @brief checks if a given key is in the hashmap
    * @param key the key to check
    * @return true if it is, false otherwise
    */
    bool containsKey(KeyT key) const
    {
        for(pair<KeyT, ValueT> in : _pairBuckets[_hashKey(key)])
        {
            if(in.first == key)
            {
                return true;
            }
        }
        return false;
    }

    /**
    * @brief finds the value of a given key
    * @param key the key
    * @return the value if it was found or an exception otherwise
    */
    ValueT& at(KeyT key)
    {
        for(pair<KeyT, ValueT> &in : _pairBuckets[_hashKey(key)])
        {
            if(in.first == key)
            {
                return in.second;
            }
        }
        throw OutOfRange();
    }

    /**
    * @brief finds the value of a given key
    * @param key the key
    * @return the value if it was found or an exception otherwise
    */
    ValueT at(KeyT key) const
    {
        for(const pair<KeyT, ValueT> &in : _pairBuckets[_hashKey(key)])
        {
            if(in.first == key)
            {
                return in.second;
            }
        }
        throw OutOfRange();
    }


    /**
    * @brief erases an element with the given key
    * @param key the key of the element to erase
    * @return true if it was erased successfully or false otherwise
    */
    bool erase(KeyT key)
    {
        if(!this->containsKey(key))
        {
            return false;
        }
        bucket<KeyT, ValueT> &tmp = _pairBuckets[_hashKey(key)];
        tmp.erase(find(tmp.begin(), tmp.end(), make_pair(key, (*this)[key])));
        --this->_size;
        if((((double) _size) / _capacity) < _lowerLoadFactor)
        {
            _fixHashMap(_capacity / 2);
        }
        return true;
    }


    /**
    * @brief returns the bucket size of a given key
    * @param key the key witch buckets size to return
    * @return this keys bucket size or exception otherwise
    */
    int bucketSize(KeyT key)
    {
        if(!this->containsKey(key))
        {
            throw OutOfRange();
        }
        return (int)this->_pairBuckets[_hashKey(key)].size();
    }


    /**
    * @brief clears this hashMap
    */
    void clear()
    {
        for(bucket<KeyT, ValueT> &in : this->_pairBuckets)
        {
            in.clear();
        }
        this->_size = 0;
    }




    /**
    * @brief a Hash Map Iterator
    */
    class HashMapIterator
    {
    private:
        friend class HashMap;

        /** Hash Map Pointer */
        const vector<bucket<KeyT, ValueT>> *_myHashMapPtr;

        /** bucket pointer */
        const bucket<KeyT, ValueT> *_bucketPtr;

        /** Hash Map Iterator */
        typename vector<bucket<KeyT, ValueT>>::const_iterator _myHashMapIterator;

        /** pair Iterator */
        typename bucket<KeyT, ValueT>::const_iterator _pairIterator;

        /**
        * @brief checks if we are in the end of this iterator
        * @return true if we are false otherwise
        */
        bool _isEndOfIterator()
        {
            if(this->_myHashMapIterator == this->_myHashMapPtr->cend())
            {
                this->_pairIterator = (*_myHashMapPtr)[(*_myHashMapPtr).size() - 1].cend();
                return true;
            }
            return false;
        }

        /**
        * @brief finds the next non empty bucket
        */
        void _findNextBucket()
        {
            do
                {
                    ++_myHashMapIterator;
                    if(_isEndOfIterator())
                    {
                        return;
                    }
                    this->_pairIterator = (*_myHashMapIterator).cbegin();
                    this->_bucketPtr = &(*_myHashMapIterator);
                } while (!(*_myHashMapIterator).size());
        }

        /**
        * @brief finds the next non empty pair
        */
        void _findNextPair()
        {
            ++_pairIterator;
            if(_pairIterator == _bucketPtr->cend())
            {
                ++(*this);
            }
        }
    public:
        /** operator overload */
        const pair<KeyT, ValueT>& operator*() const {return *_pairIterator; }


        /** operator overload */
        HashMapIterator operator++(int)
        {
            HashMapIterator toReturn = *this;
            if(this->_pairIterator == this->_bucketPtr->cend())
            {
                if(_isEndOfIterator())
                {
                    return toReturn;
                }
                _findNextBucket();
                return toReturn;
            }
            _findNextPair();
            return toReturn;
        }


        /** operator overload */
        HashMapIterator& operator++()
        {
            if(_pairIterator == _bucketPtr->cend())
            {
                if(_isEndOfIterator())
                {
                    return *this;
                }
                _findNextBucket();
                return *this;
            }
            _findNextPair();
            return *this;
        }


        /** operator overload */
        bool operator==(const HashMapIterator & other) const
        {
            bool flag1 = this->_myHashMapIterator == other._myHashMapIterator;
            bool flag2 = this->_pairIterator == other._pairIterator;
            return flag1 && flag2;
        }


        /** operator overload */
        bool operator!=(const HashMapIterator & other) const {return !(*this == other); }


        /** operator overload */
        const pair<KeyT, ValueT>* operator->() const {return &(*_pairIterator); }
    };


    /**
    * @brief now we can refer to this iterator by its formal name
    */
    typedef HashMapIterator const_iterator;


    /**
    * @brief returns a const iterator to the start of this hash map
    * @return a const iterator to the start of this hash map
    */
    const_iterator begin() const {return cbegin(); }


    /**
    * @brief returns a const iterator to the end of this hash map
    * @return a const iterator to the end of this hash map
    */
    const_iterator end() const {return cend(); }



    /**
    * @brief returns this iterator to its initial position
    * @return the const iterator
    */
    const_iterator cbegin() const
    {
        const_iterator iteratorToReturn;
        iteratorToReturn._myHashMapPtr = &(this->_pairBuckets);
        iteratorToReturn._myHashMapIterator = this->_pairBuckets.cbegin();
        iteratorToReturn._pairIterator = this->_pairBuckets[0].cbegin();
        iteratorToReturn._bucketPtr = &(*(this->_pairBuckets.cbegin()));
        while(iteratorToReturn != this->cend() && !(*iteratorToReturn._myHashMapIterator).size())
        {
            ++iteratorToReturn._myHashMapIterator;
            if(iteratorToReturn._myHashMapIterator == iteratorToReturn._myHashMapPtr->cend())
            {
                return cend();
            }
            iteratorToReturn._pairIterator = (*iteratorToReturn._myHashMapIterator).cbegin();
            iteratorToReturn._bucketPtr = &(*iteratorToReturn._myHashMapIterator);
        }
        if(iteratorToReturn._myHashMapIterator == iteratorToReturn._myHashMapPtr->cend())
        {
            return cend();
        }
        return iteratorToReturn;
    }


    /**
    * @brief relocates this iterator to its final position
    * @return the const iterator
    */
    const_iterator cend() const
    {
        const_iterator iteratorToReturn;
        iteratorToReturn._myHashMapPtr = &(this->_pairBuckets);
        iteratorToReturn._myHashMapIterator = this->_pairBuckets.cend();
        iteratorToReturn._pairIterator = this->_pairBuckets[this->_pairBuckets.size() - 1].cend();
        iteratorToReturn._bucketPtr = nullptr;
        return iteratorToReturn;
    }
};