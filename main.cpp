#include <array>
#include <iostream>
#include <map>
#include <optional>
#include <random>
#include <set>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

struct Vertex
{
  size_t x;
  size_t y;
  size_t z;
  size_t t;
  int    Vertex_handle;

  Vertex()
      : x{1 + rand() / ((RAND_MAX + 1u) / 6)}
      , y{1 + rand() / ((RAND_MAX + 1u) / 6)}
      , z{1 + rand() / ((RAND_MAX + 1u) / 6)}
      , t{1 + rand() / ((RAND_MAX + 1u) / 6)}
      , Vertex_handle{rand()}
  {}

  auto info() { return t; }
  auto Point() { return make_tuple(x, y, z); }
  void print_point()
  {
    auto points = Point();
    cout << "x:" << get<0>(points) << " y:" << get<1>(points)
         << " z:" << get<2>(points) << " with handle " << Vertex_handle;
  }
};

struct Cell
{
  int                   Cell_handle;
  std::array<Vertex, 4> vertices;

  Cell() : Cell_handle{rand()}, vertices{Vertex{}, Vertex{}, Vertex{}, Vertex{}}
  {}
  void print_cell()
  {
    cout << "Cell handle is " << Cell_handle << "\n";
    int i = 0;
    for (auto v : vertices)
    {
      cout << "Vertex(" << i << ") = ";
      v.print_point();
      cout << " with info " << v.info() << "\n";
      i++;
    }
  }
  auto vertex(int v) { return vertices[v]; }
};

struct Triangulation
{
  std::vector<Cell> cells;

  Triangulation() : cells{Cell{}, Cell{}, Cell{}, Cell{}, Cell{}, Cell{}} {}
  void print_triangulation()
  {
    cout << "There are " << cells.size() << " simplices.\n";
    for (auto c : cells) { c.print_cell(); }
  }
};

template <typename Triangulation>
[[nodiscard]] auto check_timeslices(Triangulation&& t)
    -> optional<vector<Vertex>>
{
  vector<Vertex> invalid_vertices;
  // Iterate through each cell in the triangulation
  for (auto& c : t.cells)
  {
    std::multimap<int, Vertex> this_cell;
    // Look at the vertices in each cell
    for (auto i = 0; i < 4; ++i)
    { this_cell.emplace(make_pair(c.vertex(i).info(), c.vertex(i))); }
    auto minvalue = this_cell.cbegin()->first;
    auto maxvalue = this_cell.crbegin()->first;
    cout << "Smallest timevalue in this cell is: " << minvalue << "\n";
    cout << "Largest timevalue in this cell is: " << maxvalue << "\n";
    auto min_vertex = this_cell.cbegin()->second;
    auto max_vertex = this_cell.crbegin()->second;
    cout << "Min vertex info() " << min_vertex.info() << "\n";
    cout << "Max vertex info() " << max_vertex.info() << "\n";
    if (maxvalue - minvalue == 1) { cout << "This cell is valid.\n"; }
    else
    {
      cout << "This cell is invalid.\n";

      auto minvalue_count = this_cell.count(minvalue);
      auto maxvalue_count = this_cell.count(maxvalue);
      cout << "There are " << minvalue_count
           << " vertices with the minvalue.\n";
      cout << "There are " << maxvalue_count
           << " vertices with the maxvalue.\n";
      cout << "So we should remove ";
      if (minvalue_count > maxvalue_count)
      {
        invalid_vertices.emplace_back(this_cell.rbegin()->second);
        cout << "maxvalue.\n";
      }
      else
      {
        invalid_vertices.emplace_back(this_cell.begin()->second);
        cout << "minvalue.\n";
      }
    }
  }

  if (invalid_vertices.empty()) { return nullopt; }
  else
  {
    cout << "Removing ...\n";
    for (auto& v : invalid_vertices)
    {
      cout << "Vertex " << v.Vertex_handle << " with timevalue " << v.info()
           << " and ";
      v.print_point();
      cout << endl;
    }
    return invalid_vertices;
  }
}

int main()
{
  Triangulation t;
  t.print_triangulation();

  auto bad_vertices = check_timeslices(t);

  // Print invalid vertices
  if (bad_vertices)
  {
    cout << "Printing invalid vertices.\n";
    for (auto& v : bad_vertices.value())
    {
      cout << "Timevalue is: " << v.info() << " vertex is " << v.Vertex_handle
           << "\n";
    }
  }
  else
  {
    cout << "No invalid vertices.\n";
  }

  return 0;
}