#include <vector>
#include <set>
#include <cmath>

// Diagram_point is the type of the persistence diagram's points
typedef typename std::pair<double,double> Diagram_point;

// Return the used index for encoding none of the points
int null_point_index();

// Persistence_diagrams_graph is the interface beetwen any external representation of the two persistence diagrams and
// the bottleneck distance computation. An interface is necessary to ensure basic functions complexity.
class Persistence_diagrams_graph{

public:
    // Persistence_diagram1 and 2 are the types of any externals representations of persistence diagrams. They have to have an iterator over points,
    // which have to have fields first (for birth) and second (for death).
    template<typename Persistence_diagram1, typename Persistence_diagram2>
    Persistence_diagrams_graph(Persistence_diagram1& diag1, Persistence_diagram2& diag2, double e = 0.);
    Persistence_diagrams_graph();
    bool on_the_u_diagonal(int u_point_index) const;
    bool on_the_v_diagonal(int v_point_index) const;
    int corresponding_point_in_u(int v_point_index) const;
    int corresponding_point_in_v(int u_point_index) const;
    double distance(int u_point_index, int v_point_index) const;
    int size() const;
    std::vector<double>* sorted_distances();

private:
    std::vector<Diagram_point> u;
    std::vector<Diagram_point> v;
    Diagram_point get_u_point(int u_point_index) const;
    Diagram_point get_v_point(int v_point_index) const;
};



inline int null_point_index()
{
    return -1;
}

template<typename Persistence_diagram1, typename Persistence_diagram2>
Persistence_diagrams_graph::Persistence_diagrams_graph(Persistence_diagram1& diag1, Persistence_diagram2& diag2, double e)
    : u(), v()
{
    for(auto it = diag1.begin(); it != diag1.end(); ++it)
        if(it->second - it->first > e)
            u.emplace_back(it->first, it->second);
    for(auto it = diag2.begin(); it != diag2.end(); ++it)
        if(it->second - it->first > e)
            v.emplace_back(it->first, it->second);
    if(u.size() < v.size())
        swap(u,v);
}

Persistence_diagrams_graph::Persistence_diagrams_graph::Persistence_diagrams_graph()
    : u(), v()
{}

inline bool Persistence_diagrams_graph::on_the_u_diagonal(int u_point_index) const
{
    return u_point_index >= (int) u.size();
}

inline bool Persistence_diagrams_graph::on_the_v_diagonal(int v_point_index) const
{
    return v_point_index >= (int) v.size();
}

inline int Persistence_diagrams_graph::corresponding_point_in_u(int v_point_index) const
{
    return on_the_v_diagonal(v_point_index) ? v_point_index - (int) v.size() : v_point_index + (int) u.size();
}

inline int Persistence_diagrams_graph::corresponding_point_in_v(int u_point_index) const
{
    return on_the_u_diagonal(u_point_index) ? u_point_index - (int) u.size() : u_point_index + (int) v.size();
}

inline double Persistence_diagrams_graph::distance(int u_point_index, int v_point_index) const
{
    // could be optimized for the case where one point is the projection of the other
    if(on_the_u_diagonal(u_point_index) && on_the_v_diagonal(v_point_index))
        return 0;
    Diagram_point p_u = get_u_point(u_point_index);
    Diagram_point p_v = get_v_point(v_point_index);
    return std::max(std::fabs(p_u.first - p_v.first), std::fabs(p_u.second - p_v.second));
}

inline int Persistence_diagrams_graph::size() const
{
    return (int) (u.size() + v.size());
}

inline std::vector<double>* Persistence_diagrams_graph::sorted_distances()
{
    // could be optimized
    std::set<double> sorted_distances;
    for(int u_point_index=0; u_point_index<size(); ++u_point_index)
        for(int v_point_index=0; v_point_index<size(); ++v_point_index)
            sorted_distances.emplace(distance(u_point_index, v_point_index));
    return new std::vector<double>(sorted_distances.cbegin(), sorted_distances.cend());
}

inline Diagram_point Persistence_diagrams_graph::get_u_point(int u_point_index) const
{
    if (!on_the_u_diagonal(u_point_index))
        return  u.at(u_point_index);
    Diagram_point projector = v.at(corresponding_point_in_v(u_point_index));
    double x = (projector.first + projector.second)/2;
    return Diagram_point(x,x);
}

inline Diagram_point Persistence_diagrams_graph::get_v_point(int v_point_index) const
{
    if (!on_the_v_diagonal(v_point_index))
        return  v.at(v_point_index);
    Diagram_point projector = u.at(corresponding_point_in_u(v_point_index));
    double x = (projector.first + projector.second)/2;
    return Diagram_point(x,x);
}