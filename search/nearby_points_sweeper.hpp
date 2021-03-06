#pragma once

#include "base/assert.hpp"

#include "std/algorithm.hpp"
#include "std/cstdint.hpp"
#include "std/limits.hpp"
#include "std/set.hpp"
#include "std/utility.hpp"
#include "std/vector.hpp"

namespace search
{
// This class can be used to greedily sweep points on a plane that are
// too close to each other.  Two points are considered to be "too
// close" when Manhattan distance between them is less than or equal
// to some preselected epsilon. Note, the result is not the largest
// subset of points that can be selected, but it can be computed quite
// fast and gives satisfactory results.
//
// *NOTE* The class is NOT thread-safe.
class NearbyPointsSweeper
{
public:
  explicit NearbyPointsSweeper(double eps);

  // Adds a new point (|x|, |y|) on the plane. |index| is used to
  // identify individual points, and will be reported for survived
  // points during the Sweep phase.
  void Add(double x, double y, size_t index);

  // Emits indexes of all survived points. Complexity: O(n * log(n)),
  // where n is the number of already added points.
  template <typename TEmitter>
  void Sweep(TEmitter && emitter)
  {
    sort(m_events.begin(), m_events.end());

    set<pair<double, size_t>> line;

    for (auto const & event : m_events)
    {
      switch (event.m_type)
      {
      case Event::TYPE_SEGMENT_START:
      {
        if (line.empty())
        {
          line.emplace(event.m_x, event.m_index);
          break;
        }

        auto it = line.upper_bound(make_pair(event.m_x, numeric_limits<size_t>::max()));

        bool add = true;
        while (true)
        {
          if (line.empty())
            break;

          if (it == line.end())
          {
            --it;
            continue;
          }

          double const x = it->first;
          if (fabs(x - event.m_x) <= m_eps)
          {
            add = false;
            break;
          }

          if (x + m_eps < event.m_x)
            break;

          if (it == line.begin())
            break;

          --it;
        }

        if (add)
          line.emplace(event.m_x, event.m_index);

        break;
      }

      case Event::TYPE_SEGMENT_END:
      {
        auto it = line.find(make_pair(event.m_x, event.m_index));
        if (it != line.end())
        {
          emitter(event.m_index);
          line.erase(it);
        }
        break;
      }
      };
    }

    ASSERT(line.empty(), ("Sweep line must be empty after events processing:", line));
  }

private:
  struct Event
  {
    enum Type
    {
      TYPE_SEGMENT_START,
      TYPE_SEGMENT_END
    };

    Event(Type type, double y, double x, size_t index);

    bool operator<(Event const & rhs) const;

    Type m_type;

    double m_y;
    double m_x;
    size_t m_index;
  };

  vector<Event> m_events;
  double const m_eps;
  double const m_heps;
};
}  // namespace search
