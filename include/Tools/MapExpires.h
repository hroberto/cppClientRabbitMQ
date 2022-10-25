#ifndef __MAP_EXPIRES_H__
#define __MAP_EXPIRES_H__


#include <chrono>
#include <utility>
#include <vector>
#include <memory>
#include <cassert>
#include <vector>
#include <map>

#include "Tools/MarkValue.h"



namespace libapp {

template <class K, class V>
class MapExpires {
public:
    typedef std::chrono::_V2::system_clock::time_point time_point;

    typedef typename std::pair<K, genialcpp::MarkValue<V> >     PAIR_MAP_TYPE;
    typedef typename std::map<K, genialcpp::MarkValue<V> >      MAP_TYPE;
    typedef genialcpp::MarkValue<MAP_TYPE>              VALUE_PAIR_TYPE;

    typedef std::pair<time_point, VALUE_PAIR_TYPE >             value_vector_type;
    typedef std::vector< value_vector_type >                    vector_type;
    typedef vector_type::iterator                              iterator_vector;
    typedef vector_type::const_iterator                        const_iterator_vector;


    MapExpires() {}
    MapExpires(MapExpires&& old) {
        this->container_ = std::move(old.container_);
    }

    auto& operator[](const std::size_t& n) & noexcept { return container_[n]; }
    const auto& operator[](const std::size_t& n) const& noexcept { return container_[n]; }

    void reserve(const size_t len) { container_.reserve(len); }
    size_t size() & noexcept { return container_.size(); }
    size_t size() const& noexcept  { return container_.size(); }

    bool empty() const& noexcept  { return container_.empty(); }

    void emplace_back(const MAP_TYPE& value) {
        container_.emplace_back(std::make_pair( now(), value));
    }

    void emplace_back(MAP_TYPE&& value) {
        container_.emplace_back( std::make_pair( now(), std::move(value)) );
    }

    auto begin() noexcept { return container_.begin(); }
    auto begin() const noexcept { return container_.begin(); }

    auto end() noexcept { return container_.end(); }
    auto end() const noexcept { return container_.end(); }


    MAP_TYPE::iterator& endField() & { static typename MAP_TYPE::iterator v__; return v__; }
    MAP_TYPE::const_iterator& endField() const& { static typename MAP_TYPE::const_iterator v__; return v__; }
    

    /**
     * @brief Find Const
     * 
     * @param field_key 
     * @return MAP_TYPE::const_iterator 
     */
    MAP_TYPE::const_iterator find(const K& field_key) const & noexcept {
        typename vector_type::const_iterator  itEntryMap;
        return find( field_key, itEntryMap );
    }


    MAP_TYPE::const_iterator find(const K& field_key, const_iterator_vector& itEntryMap ) const & noexcept {
        typename MAP_TYPE::const_iterator itEntry{};
        itEntryMap = end();

        for( const_iterator_vector it = container_.begin() ; it != container_.end() ; ++it ) {
            const VALUE_PAIR_TYPE& entryValue = it->second;

            typename MAP_TYPE::const_iterator itValue = entryValue->find( field_key );

            if ( itValue != entryValue->end() ) {
                itEntryMap = it;
                return itValue;
            }
        }        
        return itEntry;
    }

    /**
     * @brief Find Mutable
     * 
     * @param field_key 
     * @return MAP_TYPE::iterator 
     */
    MAP_TYPE::iterator find(const K& field_key ) & noexcept {
        iterator_vector  itEntryMap;
        return find( field_key, itEntryMap );
    }

    MAP_TYPE::iterator find(const K& field_key, iterator_vector&  itEntryMap ) & noexcept {
        typename MAP_TYPE::iterator itEntry{};
        itEntryMap = end();

        for( iterator_vector it = container_.begin() ; it != container_.end() ; ++it ) {
            VALUE_PAIR_TYPE& entryValue = it->second;

            typename MAP_TYPE::iterator itValue = entryValue->find( field_key );

            if ( itValue != entryValue->end() ) {
                itEntryMap = it;
                return itValue;
            }
        }        
        return itEntry;
    }
  



    std::shared_ptr< MapExpires::vector_type > reduce(const std::chrono::system_clock::time_point&  timePurge )
    {
        std::shared_ptr< MapExpires::vector_type > collected = std::make_shared< MapExpires::vector_type > ();

        const auto shrink_to_fit = [](VALUE_PAIR_TYPE& entryList) {
            if( ! entryList->empty()) {

                VALUE_PAIR_TYPE newEntryList;

                for (auto& itValueMap : entryList) {
                    if (!itValueMap.second.getMark()) {
                        newEntryList->emplace(std::move(itValueMap));
                    }
                }
                entryList = std::move(newEntryList);
            }
        };



        for( auto& [ktime, entryList ] : container_ ) {
            if (entryList.getMark()) {
                shrink_to_fit( entryList );
            }

            if (entryList->empty()) continue;

            else if ( ktime < timePurge) {                
                shrink_to_fit( entryList );
                collected->emplace_back( std::make_pair( ktime, std::move(entryList) ));
            }
            else {
                bool allMessageMarked{ true };
                
                for( const auto& [key, value] : entryList ) {
                    if ( ! value.getMark() ) {
                        allMessageMarked = false;
                        break;
                    }
                    else {
                        entryList.mark();
                    }
                }

                // descarta listas sem pendencias de leitura
                if (allMessageMarked && !entryList->empty()) {
                    auto local_swp = std::move( entryList.get() );
                    assert( entryList->empty());
                    assert( ! local_swp.empty() );
                }
            }
        }

        if( ! container_.empty() ) {
            const auto itBegin = container_.begin();

            if( itBegin->second->empty() ) {
                auto itNext = container_.begin();
                do {
                    ++itNext;
                } while( itNext != container_.end() && itNext->second->empty() );
                container_.erase( itBegin, itNext );
            }
        }
        collected->shrink_to_fit();

        return collected;
    }

    

protected: 
    constexpr std::chrono::system_clock::time_point now() { return std::chrono::system_clock::now(); }

private:
    MapExpires::vector_type container_;

};


}

#endif // __MAP_EXPIRES_H__