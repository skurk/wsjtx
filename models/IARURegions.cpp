#include "models/IARURegions.hpp"

#include <algorithm>

#include <QString>
#include <QVariant>
#include <QModelIndex>
#include <QMetaType>

namespace
{
  // human readable strings for each Region enumeration value
  char const * const region_names[] =
  {
   QT_TRANSLATE_NOOP ("IARURegions", "All"),
   QT_TRANSLATE_NOOP ("IARURegions", "Region 1"),
   QT_TRANSLATE_NOOP ("IARURegions", "Region 2"),
   QT_TRANSLATE_NOOP ("IARURegions", "Region 3"),
  };
  std::size_t constexpr region_names_size = sizeof (region_names) / sizeof (region_names[0]);
}

#include "moc_IARURegions.cpp"

IARURegions::IARURegions (QObject * parent)
  : QAbstractListModel {parent}
{
  static_assert (region_names_size == SENTINAL,
                 "region_names array must match Region enumeration");
}

char const * IARURegions::name (Region r)
{
  return region_names[static_cast<int> (r)];
}

IARURegions::Region IARURegions::value (QString const& s)
{
  auto end = region_names + region_names_size;
  auto p = std::find_if (region_names, end
                         , [&s] (char const * const name) {
                           return name == s;
                         });
  return p != end ? static_cast<Region> (p - region_names) : ALL;
}

QVariant IARURegions::data (QModelIndex const& index, int role) const
{
  QVariant item;

  if (index.isValid ())
    {
      auto const& row = index.row ();
      switch (role)
        {
        case Qt::ToolTipRole:
        case Qt::AccessibleDescriptionRole:
          item = tr ("IARU Region");
          break;

        case Qt::EditRole:
          item = static_cast<Region> (row);
          break;

        case Qt::DisplayRole:
        case Qt::AccessibleTextRole:
          item = region_names[row];
          break;

        case Qt::TextAlignmentRole:
          item = Qt::AlignHCenter + Qt::AlignVCenter;
          break;
        }
    }

  return item;
}

QVariant IARURegions::headerData (int section, Qt::Orientation orientation, int role) const
{
  QVariant result;

  if (Qt::DisplayRole == role && Qt::Horizontal == orientation)
    {
      result = tr ("IARU Region");
    }
  else
    {
      result = QAbstractListModel::headerData (section, orientation, role);
    }

  return result;
}

ENUM_QDATASTREAM_OPS_IMPL (IARURegions, Region);
ENUM_CONVERSION_OPS_IMPL (IARURegions, Region);
