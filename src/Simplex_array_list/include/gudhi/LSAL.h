#ifndef LSAL_H
#define LSAL_H

#include <gudhi/SAL.h>

namespace Gudhi {

class LSAL {
    
public:

    void insert_max(const Simplex& sigma);
    bool add(const Simplex& sigma);
    void remove(const Simplex& tau);
    
    bool membership(const Simplex& tau);
    bool all_facets_inside(const Simplex& sigma);

    Vertex contraction(const Vertex x, const Vertex y);

    std::size_t size() const;
private:

    void erase_max(const Simplex& sigma);
    Vertex best_index(const Simplex& tau);
    void clean(const Vertex v);
    
    std::unordered_map<Vertex, Simplex_ptr_set> t0;
    bool max_empty_face; // Is the empty simplex a maximal face ?

    std::unordered_map<Vertex, std::size_t> estimated_gamma0;
    std::size_t estimated_total_size = 0;
    std::size_t total_size = 0;
    const double alpha = 3;
    const double betta = 5;

};

void LSAL::insert_max(const Simplex& sigma){
    for(const Vertex& v : sigma)
        if(!estimated_gamma0.count(v)) estimated_gamma0.emplace(v,1);
        else estimated_gamma0[v]++;
    estimated_total_size++;
    add(sigma);
}

bool LSAL::add(const Simplex& sigma){
    max_empty_face = (sigma.size()==0); //vérifier la gestion de empty face
    Simplex_ptr sptr = std::make_shared<Simplex>(sigma);
    bool inserted = false;
    for(const Vertex& v : sigma){
        if(!t0.count(v)) t0.emplace(v, Simplex_ptr_set());
        inserted = t0.at(v).emplace(sptr).second;
    }
    if(inserted)
        total_size++;
    if(total_size > estimated_total_size * betta)
        clean(best_index(sigma));
    return inserted;
}

void LSAL::remove(const Simplex& tau){
    if(tau.size()==0){
        t0.clear();
        estimated_gamma0.clear();
        estimated_total_size = 0;
        total_size = 0;
        max_empty_face = false;
    }
    else {
        const Vertex& v = best_index(tau);
        //Copy constructor needed because the set is modified
        if(t0.count(v)) for(const Simplex_ptr& sptr : Simplex_ptr_set(t0.at(v)))
            if(included(tau, *sptr)){
                erase_max(*sptr);
                for(const Simplex& f : facets(tau))
                    insert_max(f);
            }
    }
}

bool LSAL::membership(const Simplex& tau){
    if(t0.size()==0 && !max_empty_face) return false; //empty complex
    if(tau.size()==0) return true; //empty query simplex
    Vertex v = best_index(tau);
    if(!t0.count(v))  return false;
    for(const Simplex_ptr& sptr : t0.at(v))
        if(included(tau, *sptr)) return true;
    return false;
}

bool LSAL::all_facets_inside(const Simplex& sigma){
    Vertex v = best_index(sigma);
    if(!t0.count(v))  return false;
    Simplex f = sigma; f.erase(v);
    if(!membership(f)) return false;
    std::unordered_set<Vertex> facets_inside;
    for(const Simplex_ptr& sptr : t0.at(v))
        for(const Vertex& w : sigma){
            f = sigma; f.erase(w);
            if(included(f, *sptr)) facets_inside.insert(w);
        }
    return facets_inside.size() == sigma.size() - 1;
}

/* Returns the remaining vertex */
Vertex LSAL::contraction(const Vertex x, const Vertex y){
    if(!t0.count(x)) return y;
    if(!t0.count(y)) return x;
    Vertex k, d;
    if(t0.at(x).size() > t0.at(y).size())
        k=x, d=y;
    else
        k=y, d=x;
    //Copy constructor needed because the set is modified
    for(const Simplex_ptr& sptr : Simplex_ptr_set(t0.at(d))){
        Simplex sigma(*sptr);
        erase_max(sigma);
        sigma.erase(d);
        sigma.insert(k);
        add(sigma);
    }
    t0.erase(d);
    return k;
}

/* No facets added */
inline void LSAL::erase_max(const Simplex& sigma){
    max_empty_face = false;
    Simplex_ptr sptr = std::make_shared<Simplex>(sigma);
    bool erased;
    for(const Vertex& v : sigma){
        erased = t0.at(v).erase(sptr) > 0;
        if(t0.at(v).size()==0)
            t0.erase(v);
    }
    if (erased)
        total_size--;
}

Vertex LSAL::best_index(const Simplex& tau){
    std::size_t min = std::numeric_limits<size_t>::max(); Vertex arg_min = -1;
    for(const Vertex& v : tau)
        if(!t0.count(v)) return v;
        else if(t0.at(v).size() < min)
            min = t0.at(v).size(), arg_min = v;
    if(alpha * (estimated_gamma0.count(arg_min) ? estimated_gamma0.at(arg_min) : 1) <  min)
        clean(arg_min);
    return arg_min;
}

void LSAL::clean(const Vertex v){
    SAL max_simplices;
    std::unordered_map<int, std::vector<Simplex>> dsorted_simplices;
    int max_dim = 0;
    for(const Simplex_ptr& sptr : Simplex_ptr_set(t0.at(v))){
        if(sptr->size() > max_dim){
            for(int d = max_dim+1; d<=sptr->size(); d++)
                dsorted_simplices.emplace(d, std::vector<Simplex>());
            max_dim = sptr->size();
        }
        dsorted_simplices[sptr->size()].emplace_back(*sptr);
        erase_max(*sptr);
    }
    for(int d = max_dim; d>=1; d--)
        for(const Simplex& s : dsorted_simplices.at(d))
            if(!max_simplices.membership(s))
                max_simplices.insert_max(s);
    Simplex sv; sv.insert(v);
    /*
    auto clean_cofaces = max_simplices.max_cofaces(sv);
    estimated_total_size = estimated_total_size - (estimated_gamma0.count(v) ? estimated_gamma0.at(v) : 0) + clean_cofaces.size();
    estimated_gamma0[v] = clean_cofaces.size();
    for(const Simplex_ptr& sptr : clean_cofaces)
       insert_max(*sptr);
       */
}

std::size_t LSAL::size() const{
    return total_size;
}

} //namespace Gudhi

#endif /* LSAL_H */
