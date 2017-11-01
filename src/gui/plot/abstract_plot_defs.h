#ifndef ABSTRACT_PLOT_DEFS_H
#define ABSTRACT_PLOT_DEFS_H

enum class graph_role
{
  discrete,     // bool - very important role. If true a model must provide an array
                // of points, if false --- a function
  points_count, // int For true discrete
  color,        // QColor
  width,        // int
  shown         // bool
};

#endif // ABSTRACT_PLOT_DEFS_H
