#pragma once

#include "../feature.hpp"

#include "../../std/map.hpp"


namespace stats
{
  struct GeneralInfo
  {
    uint64_t m_count, m_size;

    GeneralInfo() : m_count(0), m_size(0) {}

    void Add(uint64_t sz)
    {
      if (sz > 0)
      {
        ++m_count;
        m_size += sz;
      }
    }
  };

  template <class TKey>
  struct GeneralInfoKey
  {
    TKey m_key;
    GeneralInfo m_info;

    GeneralInfoKey(TKey key) : m_key(key) {}

    bool operator< (GeneralInfoKey const & rhs) const
    {
      return m_key < rhs.m_key;
    }
  };

  struct TypeTag
  {
    uint32_t m_val;

    TypeTag(uint32_t v) : m_val(v) {}

    bool operator< (TypeTag const & rhs) const
    {
      return m_val < rhs.m_val;
    }
  };

  struct MapInfo
  {
    set<GeneralInfoKey<FeatureBase::FeatureType> > m_byGeomType;
    set<GeneralInfoKey<TypeTag> > m_byClassifType;
    set<GeneralInfoKey<uint32_t> > m_byPointsCount;

    GeneralInfo m_all, m_names, m_types;

    template <class TKey, class TSet>
    void AddToSet(TKey key, uint32_t sz, TSet & theSet)
    {
      theSet.insert(GeneralInfoKey<TKey>(key)).first->m_info.Add(sz);
    }
  };

  void CalcStatistic(string const & fName, MapInfo & info);
  void PrintStatistic(MapInfo & info);
}
