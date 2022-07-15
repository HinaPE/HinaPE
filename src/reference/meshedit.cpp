
#include <queue>
#include <set>
#include <unordered_map>

#include "../geometry/halfedge.h"
#include "debug.h"

/******************************************************************
*********************** Local Operations **************************
******************************************************************/

/* Note on local operation return types:

    The local operations all return a std::optional<T> type. This is used so that your
    implementation can signify that it does not want to perform the operation for
    whatever reason (e.g. you don't want to allow the user to erase the last vertex).

    An optional can have two values: std::nullopt, or a value of the type it is
    parameterized on. In this way, it's similar to a pointer, but has two advantages:
    the value it holds need not be allocated elsewhere, and it provides an API that
    forces the user to check if it is null before using the value.

    In your implementation, if you have successfully performed the operation, you can
    simply return the required reference:

            ... collapse the edge ...
            return collapsed_vertex_ref;

    And if you wish to deny the operation, you can return the null optional:

            return std::nullopt;

    Note that the stubs below all reject their duties by returning the null optional.
*/

/* 
    This method splits the given edge in half, but does not split the
    adjacent faces. Returns an iterator to the new vertex which splits
    the original edge.

    Example function for how to go about implementing local operations
*/
std::optional<Halfedge_Mesh::VertexRef> Halfedge_Mesh::bisect_edge(EdgeRef e)
{

    // Phase 1: collect all elements
    HalfedgeRef h = (e->halfedge()->is_boundary()) ? e->halfedge()->twin() : e->halfedge();
    HalfedgeRef ht = h->twin();
    HalfedgeRef preh = h;
    HalfedgeRef nexht = ht->next();
    do
    {
        preh = preh->next();
    } while (preh->next() != h);
    Vec3 vpos = (h->vertex()->pos + ht->vertex()->pos) / 2;

    // Phase 2: Allocate new elements
    VertexRef c = new_vertex();
    c->pos = vpos;
    HalfedgeRef hn = new_halfedge();
    HalfedgeRef hnt = new_halfedge();
    EdgeRef e0 = new_edge();

    // The following elements aren't necessary for the bisect_edge, but they are here to demonstrate phase 4
    FaceRef f_not_used = new_face();
    HalfedgeRef h_not_used = new_halfedge();

    // Phase 3: Reassign elements
    e0->halfedge() = hn;
    hn->twin() = hnt;
    hn->edge() = e0;
    hn->vertex() = h->vertex();
    hn->face() = h->face();
    preh->next() = hn;
    hn->next() = h;
    h->vertex() = c;
    ht->next() = hnt;
    c->halfedge() = h;
    hn->vertex()->halfedge() = hn;
    // is_new parameter is used for global operations
    c->is_new = true;

    // example of set_neighbors:
    // condenses hnt->next() = nexht; hnt->twin() = hn; hnt->vertex() = c; hnt->edge() = e0; hnt->face() = ht->face(); into one line
    hnt->set_neighbors(nexht, hn, c, e0, ht->face());

    // Phase 4: Delete unused elements
    erase(f_not_used);
    erase(h_not_used);

    // Phase 5: Return the correct iterator
    return c;
}

/*
    This method should replace the given vertex and all its neighboring
    edges and faces with a single face, returning the new face.
 */
std::optional<Halfedge_Mesh::FaceRef> Halfedge_Mesh::erase_vertex(Halfedge_Mesh::VertexRef v)
{
    if (v->on_boundary())
    {
        return std::nullopt;
    }
    FaceRef newf = new_face();
    HalfedgeRef h1 = v->halfedge();
    // redirection
    HalfedgeRef travel = h1;
    do
    {
        travel->twin()->vertex()->halfedge() = travel->next();
        travel = travel->twin()->next();
    } while (travel != h1);
    std::vector<HalfedgeRef> ring;
    unsigned int outdeg = v->degree();
    HalfedgeRef cur = h1;
    std::vector<HalfedgeRef> h2erase;
    for (unsigned int i = 0; i < outdeg; i++)
    {
        FaceRef f = cur->face();
        EdgeRef e = cur->edge();
        unsigned int deg = f->degree();
        h2erase.push_back(cur);
        cur = cur->next(); // first edge
        for (unsigned int j = 0; j < deg - 2; j++)
        {
            ring.push_back(cur);
            cur = cur->next();
        }
        assert(cur->next() == travel);
        h2erase.push_back(cur);
        cur = cur->twin();
        assert(cur->vertex() == v);
        travel = cur;
        erase(f);
        erase(e);
    }
    for (unsigned int i = 0; i < h2erase.size(); i++)
    {
        HalfedgeRef hdel = h2erase[i];
        erase(hdel);
    }
    for (unsigned int i = 0; i < ring.size(); i++)
    {
        HalfedgeRef hcur = ring[i];
        HalfedgeRef hnext = ring[(i + 1) % ring.size()];
        hcur->face() = newf;
        newf->halfedge() = hcur;
        hcur->next() = hnext;
    }
    erase(v);
    return newf;
}

/*
    This method should erase the given edge and return an iterator to the
    merged face.
 */
std::optional<Halfedge_Mesh::FaceRef> Halfedge_Mesh::erase_edge(Halfedge_Mesh::EdgeRef e)
{

    if (e->on_boundary())
    {
        return std::nullopt;
    }
    HalfedgeRef h1 = e->halfedge();
    HalfedgeRef ha = h1->twin();
    if (h1->face() == ha->face())
    {
        return std::nullopt;
    }
    HalfedgeRef h1next = h1->next();
    HalfedgeRef h1prev = h1->next();
    while (h1prev->next() != h1)
    {
        h1prev = h1prev->next();
    }
    HalfedgeRef hanext = ha->next();
    HalfedgeRef haprev = ha->next();
    while (haprev->next() != ha)
    {
        haprev = haprev->next();
    }
    FaceRef f1 = h1->face();
    FaceRef fa = ha->face();
    VertexRef v1 = h1->vertex();
    VertexRef va = ha->vertex();
    erase(fa);
    erase(e);
    f1->halfedge() = h1next;
    haprev->next() = h1next;
    h1prev->next() = hanext;
    v1->halfedge() = hanext;
    va->halfedge() = h1next;
    h1next->vertex() = va;
    hanext->vertex() = v1;
    erase(h1);
    erase(ha);
    hanext->face() = f1;
    HalfedgeRef htravel = hanext->next();
    while (htravel != hanext)
    {
        htravel->face() = f1;
        htravel = htravel->next();
    }
    return f1;
}

/*
    This method should collapse the given edge and return an iterator to
    the new vertex created by the collapse.
*/
std::optional<Halfedge_Mesh::VertexRef> Halfedge_Mesh::collapse_edge(Halfedge_Mesh::EdgeRef e)
{

    HalfedgeRef h1 = e->halfedge();
    HalfedgeRef ha = h1->twin();
    if (h1->face()->is_boundary())
    {
        h1 = h1->twin();
        ha = ha->twin();
    }
    if (h1->next() == h1->twin())
    {
        return std::nullopt;
    }
    if (h1->next()->next()->next() == h1 && ha->next()->next()->next() == ha)
    {
        if (h1->next()->edge() == ha->next()->next()->edge() &&
            h1->next()->next()->edge() == ha->next()->edge())
        {
            return std::nullopt;
        }
    }
    // Check if all edges on boundary for either
    HalfedgeRef travel = h1->next();
    bool allBoundary = true;
    while (travel != h1)
    {
        if (!travel->edge()->on_boundary())
        {
            allBoundary = false;
            break;
        }
        travel = travel->next();
    }
    if (allBoundary && !e->on_boundary())
    {
        return std::nullopt;
    }
    allBoundary = true;
    travel = ha->next();
    while (travel != ha)
    {
        if (!travel->edge()->on_boundary())
        {
            allBoundary = false;
            break;
        }
        travel = travel->next();
    }
    if (allBoundary && !e->on_boundary())
    {
        return std::nullopt;
    }
    VertexRef v2backup = ha->vertex();
    VertexRef vnew = new_vertex();
    vnew->pos = e->center();
    // Connect all out going edges from both old vertices to vnew
    travel = h1->twin()->next();
    while (true)
    {
        travel->vertex() = vnew;
        travel = travel->twin()->next();
        if (travel == h1)
        {
            break;
        }
    }
    travel = ha->twin()->next();
    while (true)
    {
        travel->vertex() = vnew;
        travel = travel->twin()->next();
        if (travel == ha)
        {
            break;
        }
    }
    HalfedgeRef boundaryhc = h1->next();
    while (boundaryhc->next() != h1)
    {
        boundaryhc = boundaryhc->next();
    }
    boundaryhc = boundaryhc->twin();

    // Check if either side is triangle
    if (h1->next()->next()->next() == h1)
    {
        // triangle, need to replace degen poly with edge
        HalfedgeRef h2 = h1->next();
        EdgeRef e2 = h2->edge();
        HalfedgeRef hb = h2->twin();
        HalfedgeRef h3 = h2->next();
        VertexRef v3 = h3->vertex();
        HalfedgeRef hc = h3->twin();
        EdgeRef ec = hc->edge();
        hc->set_neighbors(hc->next(), hb, vnew, hb->edge(), hc->face());
        hb->set_neighbors(hb->next(), hc, v3, hb->edge(), hb->face());
        FaceRef f = h1->face();
        e2->halfedge() = hc;
        v3->halfedge() = hb;
        travel = v2backup->halfedge();

        erase(f);
        erase(ec);
        erase(h1->vertex());
        erase(h1);
        erase(v2backup);
        erase(h2);
        erase(h3);
        vnew->halfedge() = hc;
    } else
    {
        // Not triangle, normal sequence
        HalfedgeRef h2 = h1->next();
        HalfedgeRef hprev1 = h2;
        FaceRef f = h1->face();
        f->halfedge() = h2;
        while (hprev1->next() != h1)
        {
            hprev1 = hprev1->next();
        }
        hprev1->next() = h2;
        h2->vertex() = vnew;
        erase(h1->vertex());
        erase(h1);
        vnew->halfedge() = h2;
    }
    if (ha->face()->is_boundary())
    {
        boundaryhc->vertex() = vnew;
        HalfedgeRef hpreva = ha->next();
        while (hpreva->next() != ha)
        {
            hpreva = hpreva->next();
        }
        hpreva->next() = ha->next();
        ha->face()->halfedge() = ha->next();
        erase(ha);
        erase(e);

        return vnew;
    }
    if (ha->next()->next()->next() == ha)
    {
        // triangle, replace degen
        h1 = ha;
        HalfedgeRef h2 = h1->next();
        EdgeRef e2 = h2->edge();
        HalfedgeRef hb = h2->twin();
        HalfedgeRef h3 = h2->next();
        VertexRef v3 = h3->vertex();
        HalfedgeRef hc = h3->twin();
        EdgeRef ec = hc->edge();
        hc->set_neighbors(hc->next(), hb, vnew, hb->edge(), hc->face());
        hb->set_neighbors(hb->next(), hc, v3, hb->edge(), hb->face());
        FaceRef f = h1->face();
        e2->halfedge() = hc;
        v3->halfedge() = hb;
        travel = v2backup->halfedge();

        erase(f);
        erase(ec);
        erase(h1->vertex());
        erase(h1);
        erase(h2);
        erase(h3);
        vnew->halfedge() = hc;
    } else
    {
        // normal
        h1 = ha;
        HalfedgeRef h2 = h1->next();
        HalfedgeRef hprev1 = h2;
        FaceRef f = h1->face();
        f->halfedge() = h2;
        while (hprev1->next() != h1)
        {
            hprev1 = hprev1->next();
        }
        hprev1->next() = h2;
        h2->vertex() = vnew;
        erase(h1->vertex());
        erase(h1);
    }
    erase(e);

    return vnew;
}

/*
    This method should collapse the given face and return an iterator to
    the new vertex created by the collapse.
*/
std::optional<Halfedge_Mesh::VertexRef> Halfedge_Mesh::collapse_face(Halfedge_Mesh::FaceRef f)
{

    (void) f;
    return std::nullopt;
}

/*
    This method should flip the given edge counter-clockwise and return an iterator to the
    flipped edge.
*/
std::optional<Halfedge_Mesh::EdgeRef> Halfedge_Mesh::flip_edge(Halfedge_Mesh::EdgeRef e)
{

    if (e->on_boundary())
    {
        return std::nullopt;
    }
    HalfedgeRef h1 = e->halfedge();
    HalfedgeRef ha = h1->twin();
    if (h1->face()->is_boundary() || ha->face()->is_boundary())
    {
        return std::nullopt;
    }
    // Need to get halfedge where next = h1 and next = ha
    HalfedgeRef hprev1 = h1->next();
    while (hprev1->next() != h1)
    {
        hprev1 = hprev1->next();
    }
    HalfedgeRef hpreva = ha->next();
    while (hpreva->next() != ha)
    {
        hpreva = hpreva->next();
    }
    VertexRef va = ha->vertex();
    VertexRef v1 = h1->vertex();
    HalfedgeRef h2 = h1->next();
    HalfedgeRef hb = ha->next();
    HalfedgeRef h3 = h1->next()->next();
    HalfedgeRef hc = ha->next()->next();
    VertexRef vc = hc->vertex();
    VertexRef v3 = h3->vertex();
    FaceRef f1 = h1->face();
    FaceRef fa = ha->face();

    // Vertex assignment
    va->halfedge() = h1->next();
    v1->halfedge() = ha->next();

    // Face assignment
    f1->halfedge() = h1;
    fa->halfedge() = ha;
    // Edge assignment
    // redundant
    // Halfedge assignments
    hprev1->next() = ha->next();
    hpreva->next() = h1->next();

    // set_neighbors
    ha->next()->next() = h1;
    h1->next()->next() = ha;
    h1->set_neighbors(h3, ha, vc, e, f1);
    ha->set_neighbors(hc, h1, v3, e, fa);
    h2->face() = fa;
    hb->face() = f1;

    return e;
}

/*
    This method should split the given edge and return an iterator to the
    newly inserted vertex. The halfedge of this vertex should point along
    the edge that was split, rather than the new edges.
*/
std::optional<Halfedge_Mesh::VertexRef> Halfedge_Mesh::split_edge(Halfedge_Mesh::EdgeRef e)
{

    // Check if it is triangle on both sides
    HalfedgeRef h1 = e->halfedge();
    HalfedgeRef ha = h1->twin();
    if (h1->face()->is_boundary())
    {

        h1 = h1->twin();
        ha = ha->twin();
    }
    if (ha->face()->is_boundary())
    {
        if (h1->next()->next()->next() != h1)
        {
            // Not triangle, return
            return std::nullopt;
        }
        VertexRef va = ha->vertex();
        VertexRef v1 = h1->vertex();
        HalfedgeRef h2 = h1->next();
        HalfedgeRef h3 = h2->next();
        VertexRef v3 = h3->vertex();
        FaceRef f1 = h1->face();
        FaceRef fa = ha->face();

        HalfedgeRef preha = ha;
        while (preha->next() != ha)
        {
            preha = preha->next();
        }

        // Create new edges/vertices
        VertexRef vnew = new_vertex();
        EdgeRef enewdown = new_edge();
        EdgeRef enewleft = new_edge();
        HalfedgeRef h32 = new_halfedge();
        HalfedgeRef h22 = new_halfedge();
        HalfedgeRef h12 = new_halfedge();
        HalfedgeRef ha2 = new_halfedge();
        FaceRef f12 = new_face();

        vnew->halfedge() = h1;
        vnew->pos = e->center();
        enewdown->halfedge() = ha;
        enewleft->halfedge() = h22;
        h12->set_neighbors(h22, ha, v1, enewdown, f12);
        h22->set_neighbors(h3, h32, vnew, enewleft, f12);
        h32->set_neighbors(h1, h22, v3, enewleft, f1);
        ha2->set_neighbors(ha, h1, va, e, fa);
        f12->halfedge() = h12;

        // Change old elements
        h1->vertex() = vnew;
        ha->vertex() = vnew;
        ha->edge() = enewdown;
        va->halfedge() = ha2;
        v1->halfedge() = h12;
        h1->twin() = ha2;
        h2->next() = h32;
        h3->next() = h12;
        h3->face() = f12;
        preha->next() = ha2;
        ha->twin() = h12;
        f1->halfedge() = h1;
        fa->halfedge() = ha;

        return vnew;
    }
    if (h1->next()->next()->next() != h1 || ha->next()->next()->next() != ha)
    {
        // Not triangle, return
        return std::nullopt;
    }
    // Setup
    VertexRef va = ha->vertex();
    VertexRef v1 = h1->vertex();
    HalfedgeRef h2 = h1->next();
    HalfedgeRef hb = ha->next();
    HalfedgeRef h3 = h2->next();
    HalfedgeRef hc = hb->next();
    VertexRef vc = hc->vertex();
    VertexRef v3 = h3->vertex();
    FaceRef f1 = h1->face();
    FaceRef fa = ha->face();

    // Create new edges/vertices
    VertexRef vnew = new_vertex();
    EdgeRef enewdown = new_edge();
    EdgeRef enewleft = new_edge();
    EdgeRef enewright = new_edge();
    enewdown->is_new = false;
    enewleft->is_new = true;
    enewright->is_new = true;
    e->is_new = false;
    HalfedgeRef h32 = new_halfedge();
    HalfedgeRef h22 = new_halfedge();
    HalfedgeRef h12 = new_halfedge();
    HalfedgeRef ha2 = new_halfedge();
    HalfedgeRef hb2 = new_halfedge();
    HalfedgeRef hc2 = new_halfedge();
    FaceRef f12 = new_face();
    FaceRef fa2 = new_face();

    vnew->halfedge() = h1;
    vnew->pos = e->center();
    enewdown->halfedge() = ha;
    enewleft->halfedge() = h22;
    enewright->halfedge() = hb2;
    h12->set_neighbors(h22, ha, v1, enewdown, f12);
    h22->set_neighbors(h3, h32, vnew, enewleft, f12);
    h32->set_neighbors(h1, h22, v3, enewleft, f1);
    ha2->set_neighbors(hb2, h1, va, e, fa2);
    hb2->set_neighbors(hc, hc2, vnew, enewright, fa2);
    hc2->set_neighbors(ha, hb2, vc, enewright, fa);
    f12->halfedge() = h12;
    fa2->halfedge() = ha2;

    // Change old elements
    h1->vertex() = vnew;
    ha->vertex() = vnew;
    ha->edge() = enewdown;
    va->halfedge() = ha2;
    v3->halfedge() = h32;
    v1->halfedge() = h12;
    vc->halfedge() = hc2;
    h1->twin() = ha2;
    h2->next() = h32;
    h3->next() = h12;
    h3->face() = f12;
    ha->twin() = h12;
    hb->next() = hc2;
    hc->next() = ha2;
    hc->face() = fa2;
    f1->halfedge() = h1;
    fa->halfedge() = ha;

    return vnew;
}


/*
    This method should insets a vertex into the given face, returning a pointer to the new center vertex
*/
std::optional<Halfedge_Mesh::VertexRef> Halfedge_Mesh::inset_vertex(FaceRef f)
{
    (void) f;
    return std::nullopt;
}

/*
    This method should inset a face into the given face, returning a pointer to the new face.
*/
std::optional<Halfedge_Mesh::FaceRef> Halfedge_Mesh::inset_face(Halfedge_Mesh::FaceRef f)
{

    // hint: use bevel_face positions as a helper function here
    (void) f;
    return std::nullopt;
}

/*
    This method should bevel a vertex and inserts a vertex into the new vertex, returning a pointer to that vertex
*/
std::optional<Halfedge_Mesh::VertexRef> Halfedge_Mesh::extrude_vertex(VertexRef v)
{
    (void) v;
    return std::nullopt;
}

/* Note on the beveling process:

    Each of the bevel_vertex, bevel_edge, and bevel_face functions do not represent
    a full bevel operation. Instead, they should update the _connectivity_ of
    the mesh, _not_ the positions of newly created vertices. In fact, you should set
    the positions of new vertices to be exactly the same as wherever they "started from."

    When you click on a mesh element while in bevel mode, one of those three functions
    is called. But, because you may then adjust the distance/offset of the newly
    beveled face, we need another method of updating the positions of the new vertices.

    This is where bevel_vertex_positions, bevel_edge_positions, and
    bevel_face_positions come in: these functions are called repeatedly as you
    move your mouse, the position of which determines the normal and tangent offset
    parameters. These functions are also passed an array of the original vertex
    positions: for bevel_vertex, it has one element, the original vertex position,
    for bevel_edge, two for the two vertices, and for bevel_face, it has the original
    position of each vertex in order starting from face->halfedge. You should use these 
    positions, as well as the normal and tangent offset fields to assign positions to 
    the new vertices.

    Finally, note that the normal and tangent offsets are not relative values - you
    should compute a particular new position from them, not a delta to apply.
*/

/*
    This method should replace the vertex v with a face, corresponding to
    a bevel operation. It should return the new face.  NOTE: This method is
    only responsible for updating the *connectivity* of the mesh---it does not
    need to update the vertex positions. These positions will be updated in
    Halfedge_Mesh::bevel_vertex_positions (which you also have to
    implement!)
*/
std::optional<Halfedge_Mesh::FaceRef> Halfedge_Mesh::bevel_vertex(Halfedge_Mesh::VertexRef v)
{

    // Reminder: You should set the positions of new vertices (v->pos) to be exactly
    // the same as wherever they "started from."

    (void) v;
    return std::nullopt;
}

/*
    This method should replace the edge e with a face, corresponding to a
    bevel operation. It should return the new face. NOTE: This method is
    responsible for updating the *connectivity* of the mesh only---it does not
    need to update the vertex positions. These positions will be updated in
    Halfedge_Mesh::bevel_edge_positions (which you also have to
    implement!)
*/
std::optional<Halfedge_Mesh::FaceRef> Halfedge_Mesh::bevel_edge(Halfedge_Mesh::EdgeRef e)
{

    // Reminder: You should set the positions of new vertices (v->pos) to be exactly
    // the same as wherever they "started from."

    (void) e;
    return std::nullopt;
}

/*
    This method should replace the face f with an additional, inset face
    (and ring of faces around it), corresponding to a bevel operation. It
    should return the new face.  NOTE: This method is responsible for updating
    the *connectivity* of the mesh only---it does not need to update the vertex
    positions. These positions will be updated in
    Halfedge_Mesh::bevel_face_positions (which you also have to
    implement!)
*/
std::optional<Halfedge_Mesh::FaceRef> Halfedge_Mesh::bevel_face(Halfedge_Mesh::FaceRef f)
{

    // Reminder: You should set the positions of new vertices (v->pos) to be exactly
    // the same as wherever they "started from."

    (void) f;
    return std::nullopt;
}

/*
    Compute new vertex positions for the vertices of the beveled vertex.

    These vertices can be accessed via new_halfedges[i]->vertex()->pos for
    i = 1, ..., new_halfedges.size()-1.

    The basic strategy here is to loop over the list of outgoing halfedges,
    and use the original vertex position and its associated outgoing edge
    to compute a new vertex position along the outgoing edge.
*/
void Halfedge_Mesh::bevel_vertex_positions(const std::vector<Vec3> &start_positions,
                                           Halfedge_Mesh::FaceRef face, float tangent_offset)
{

    std::vector<HalfedgeRef> new_halfedges;
    auto h = face->halfedge();
    do
    {
        new_halfedges.push_back(h);
        h = h->next();
    } while (h != face->halfedge());

    (void) new_halfedges;
    (void) start_positions;
    (void) face;
    (void) tangent_offset;
}

/*
    Compute new vertex positions for the vertices of the beveled edge.

    These vertices can be accessed via new_halfedges[i]->vertex()->pos for
    i = 1, ..., new_halfedges.size()-1.

    The basic strategy here is to loop over the list of outgoing halfedges,
    and use the preceding and next vertex position from the original mesh
    (in the orig array) to compute an offset vertex position.

    Note that there is a 1-to-1 correspondence between halfedges in
    newHalfedges and vertex positions in start_positions. So, you can write 
    loops of the form:

    for(size_t i = 0; i < new_halfedges.size(); i++)
    {
            Vector3D pi = start_positions[i]; // get the original vertex
            position corresponding to vertex i
    }
*/
void Halfedge_Mesh::bevel_edge_positions(const std::vector<Vec3> &start_positions,
                                         Halfedge_Mesh::FaceRef face, float tangent_offset)
{

    std::vector<HalfedgeRef> new_halfedges;
    auto h = face->halfedge();
    do
    {
        new_halfedges.push_back(h);
        h = h->next();
    } while (h != face->halfedge());

    (void) new_halfedges;
    (void) start_positions;
    (void) face;
    (void) tangent_offset;
}

/*
    Compute new vertex positions for the vertices of the beveled face.

    These vertices can be accessed via new_halfedges[i]->vertex()->pos for
    i = 0, ..., new_halfedges.size()-1.

    The basic strategy here is to loop over the list of outgoing halfedges,
    and use the preceding and next vertex position from the original mesh
    (in the start_positions array) to compute an offset vertex
    position.

    Note that there is a 1-to-1 correspondence between halfedges in
    new_halfedges and vertex positions in start_positions. So, you can write 
    loops of the form:

    for(size_t i = 0; i < new_halfedges.size(); i++)
    {
            Vec3 pi = start_positions[i]; // get the original vertex
            position corresponding to vertex i
    }
*/
void Halfedge_Mesh::bevel_face_positions(const std::vector<Vec3> &start_positions,
                                         Halfedge_Mesh::FaceRef face, float tangent_offset,
                                         float normal_offset)
{

    if (flip_orientation) normal_offset = -normal_offset;
    std::vector<HalfedgeRef> new_halfedges;
    auto h = face->halfedge();
    do
    {
        new_halfedges.push_back(h);
        h = h->next();
    } while (h != face->halfedge());

    for (size_t i = 0; i < new_halfedges.size(); i++)
    {
        Vec3 pi = start_positions[i]; // get the original vertex pos
        VertexRef vi = new_halfedges[i]->vertex();
        Vec3 tangent = face->center() - start_positions[i];
        vi->pos = pi - normal_offset * face->normal() - tangent_offset * tangent.normalize();
    }
}

/*
    Updates the position of v using the given start_position
*/
void Halfedge_Mesh::extrude_vertex_position(const Vec3 &start_positions, Halfedge_Mesh::FaceRef face)
{
    (void) start_positions;
    (void) face;
}

/******************************************************************
*********************** Global Operations *************************
******************************************************************/

/*
    Splits all non-triangular faces into triangles.
*/
void Halfedge_Mesh::triangulate()
{

    // For each face...
    FaceRef f;
    std::vector<FaceRef> all_faces;
    for (f = faces_begin(); f != faces_end(); f++)
    {
        if (ferased.find(f) != ferased.end()) continue;
        if (f->is_boundary()) continue;
        all_faces.push_back(f);
    }

    for (unsigned int i = 0; i < all_faces.size(); i++)
    {
        f = all_faces[i];
        unsigned int deg = f->degree();
        if (deg <= 3)
        {
            continue;
        }
        // Face deg larger than 3f
        // First triangle, keeping face
        HalfedgeRef hleft = f->halfedge();
        HalfedgeRef h1 = hleft;
        VertexRef v1 = hleft->vertex();
        HalfedgeRef hbase = hleft->next();
        HalfedgeRef hbasenext = hbase->next();
        HalfedgeRef hright = new_halfedge();
        EdgeRef eright = new_edge();
        eright->halfedge() = hright;
        hbase->next() = hright;
        hright->set_neighbors(hleft, hright, hbasenext->vertex(), eright, f);
        hbase = hbasenext;
        hbasenext = hbase->next();
        HalfedgeRef prevhright = hright;
        EdgeRef preveright = eright;
        // Note hright twin is itself, set that in the next iter.
        for (unsigned int j = 1; j < deg - 3; j++)
        {
            // Make new face, new right edge, new left halfedge
            FaceRef newf = new_face();
            eright = new_edge();
            hleft = new_halfedge();
            hright = new_halfedge();
            // save the next base
            hleft->set_neighbors(hbase, prevhright, v1, preveright, newf);
            prevhright->twin() = hleft;
            hright->set_neighbors(hleft, hright, hbasenext->vertex(), eright, newf);
            eright->halfedge() = hright;
            hbase->next() = hright;
            hbase->face() = newf;
            newf->halfedge() = hleft;
            // setup next loop
            preveright = eright;
            prevhright = hright;
            hbase = hbasenext;
            hbasenext = hbase->next();
        }
        // Last triangle
        FaceRef newf = new_face();
        hleft = new_halfedge();
        eright = hbasenext->edge();
        assert(hbasenext->next() == h1);
        hright = hbasenext;
        // reassign
        hleft->set_neighbors(hbase, prevhright, v1, preveright, newf);
        prevhright->twin() = hleft;
        hright->next() = hleft;
        hbase->face() = newf;
        hright->face() = newf;
        newf->halfedge() = hleft;
    }
    return;
}

/* Note on the quad subdivision process:

        Unlike the local mesh operations (like bevel or edge flip), we will perform
        subdivision by splitting *all* faces into quads "simultaneously."  Rather
        than operating directly on the halfedge data structure (which as you've
        seen is quite difficult to maintain!) we are going to do something a bit nicer:
           1. Create a raw list of vertex positions and faces (rather than a full-
              blown halfedge mesh).
           2. Build a new halfedge mesh from these lists, replacing the old one.
        Sometimes rebuilding a data structure from scratch is simpler (and even
        more efficient) than incrementally modifying the existing one.  These steps are
        detailed below.

  Step I: Compute the vertex positions for the subdivided mesh.
        Here we're going to do something a little bit strange: since we will
        have one vertex in the subdivided mesh for each vertex, edge, and face in
        the original mesh, we can nicely store the new vertex *positions* as
        attributes on vertices, edges, and faces of the original mesh. These positions
        can then be conveniently copied into the new, subdivided mesh.
        This is what you will implement in linear_subdivide_positions() and
        catmullclark_subdivide_positions().

  Steps II-IV are provided (see Halfedge_Mesh::subdivide()), but are still detailed
  here:

  Step II: Assign a unique index (starting at 0) to each vertex, edge, and
        face in the original mesh. These indices will be the indices of the
        vertices in the new (subdivided) mesh. They do not have to be assigned
        in any particular order, so long as no index is shared by more than one
        mesh element, and the total number of indices is equal to V+E+F, i.e.,
        the total number of vertices plus edges plus faces in the original mesh.
        Basically we just need a one-to-one mapping between original mesh elements
        and subdivided mesh vertices.

  Step III: Build a list of quads in the new (subdivided) mesh, as tuples of
        the element indices defined above. In other words, each new quad should be
        of the form (i,j,k,l), where i,j,k and l are four of the indices stored on
        our original mesh elements.  Note that it is essential to get the orientation
        right here: (i,j,k,l) is not the same as (l,k,j,i).  Indices of new faces
        should circulate in the same direction as old faces (think about the right-hand
        rule).

  Step IV: Pass the list of vertices and quads to a routine that clears
        the internal data for this halfedge mesh, and builds new halfedge data from
        scratch, using the two lists.
*/

/*
    Compute new vertex positions for a mesh that splits each polygon
    into quads (by inserting a vertex at the face midpoint and each
    of the edge midpoints).  The new vertex positions will be stored
    in the members Vertex::new_pos, Edge::new_pos, and
    Face::new_pos.  The values of the positions are based on
    simple linear interpolation, e.g., the edge midpoints and face
    centroids.
*/
void Halfedge_Mesh::linear_subdivide_positions()
{

    // For each vertex, assign Vertex::new_pos to
    // its original position, Vertex::pos.
    for (VertexRef v = vertices_begin(); v != vertices_end(); v++)
    {
        v->new_pos = v->pos;
    }

    // For each edge, assign the midpoint of the two original
    // positions to Edge::new_pos.
    for (EdgeRef e = edges_begin(); e != edges_end(); e++)
    {
        e->new_pos = e->center();
    }

    // For each face, assign the centroid (i.e., arithmetic mean)
    // of the original vertex positions to Face::new_pos. Note
    // that in general, NOT all faces will be triangles!
    for (FaceRef f = faces_begin(); f != faces_end(); f++)
    {
        f->new_pos = f->center();
    }

}

/*
    Compute new vertex positions for a mesh that splits each polygon
    into quads (by inserting a vertex at the face midpoint and each
    of the edge midpoints).  The new vertex positions will be stored
    in the members Vertex::new_pos, Edge::new_pos, and
    Face::new_pos. The values of the positions are based on
    the Catmull-Clark rules for subdivision.

    Note: this will only be called on meshes without boundary
*/
void Halfedge_Mesh::catmullclark_subdivide_positions()
{

    // The implementation for this routine should be
    // a lot like Halfedge_Mesh:linear_subdivide_positions:(),
    // except that the calculation of the positions themsevles is
    // slightly more involved, using the Catmull-Clark subdivision
    // rules. (These rules are outlined in the Developer Manual.)

    // Faces
    for (FaceRef f = faces_begin(); f != faces_end(); f++)
    {
        f->new_pos = f->center();
    }

    // Edges
    for (EdgeRef e = edges_begin(); e != edges_end(); e++)
    {
        HalfedgeRef h1 = e->halfedge();
        HalfedgeRef ha = h1->twin();
        Vec3 mid = (h1->face()->new_pos + ha->face()->new_pos) / 2;
        e->new_pos = e->center() / 2 + mid / 2;
    }

    // Vertices
    for (VertexRef v = vertices_begin(); v != vertices_end(); v++)
    {
        Vec3 Q;
        Vec3 R;
        HalfedgeRef travel = v->halfedge();
        float deg = (float) v->degree();
        for (float i = 0.0f; i < deg; i += 1.0f)
        {
            FaceRef curf = travel->face();
            EdgeRef cure = travel->edge();
            Q += curf->new_pos / deg;
            R += cure->center() / deg;
            travel = travel->twin()->next();
        }
        Vec3 pos = Q / deg + 2.0f * R / deg + ((deg - 3) / deg) * v->pos;
        v->new_pos = pos;
    }

}

/*
    This routine should increase the number of triangles in the mesh
    using Loop subdivision. Note: this is will only be called on triangle meshes.
*/
void Halfedge_Mesh::loop_subdivide()
{

    // Each vertex and edge of the original mesh can be associated with a
    // vertex in the new (subdivided) mesh.
    // Therefore, our strategy for computing the subdivided vertex locations is to
    // *first* compute the new positions
    // using the connectivity of the original (coarse) mesh. Navigating this mesh
    // will be much easier than navigating
    // the new subdivided (fine) mesh, which has more elements to traverse.  We
    // will then assign vertex positions in
    // the new mesh based on the values we computed for the original mesh.

    // Compute new positions for all the vertices in the input mesh using
    // the Loop subdivision rule and store them in Vertex::new_pos.
    //    At this point, we also want to mark each vertex as being a vertex of the
    //    original mesh. Use Vertex::is_new for this.

    for (VertexRef v = vertices_begin(); v != vertices_end(); v++)
    {
        v->is_new = false;
        float deg = (float) v->degree();
        Vec3 avg;
        HalfedgeRef travel = v->halfedge();
        float u;
        if (int(deg) == 3)
        {
            u = 3.0f / 16.0f;
        } else
        {
            u = 3.0f / (8.0f * deg);
        }
        for (float i = 0.0f; i < deg; i += 1.0f)
        {
            VertexRef neigh = travel->twin()->vertex();
            avg += neigh->pos * u;
            travel = travel->twin()->next();
        }
        avg += (1.0f - deg * u) * v->pos;
        v->new_pos = avg;
    }
    // Next, compute the subdivided vertex positions associated with edges, and
    // store them in Edge::new_pos.
    for (EdgeRef e = edges_begin(); e != edges_end(); e++)
    {
        Vec3 pos;
        HalfedgeRef h1 = e->halfedge();
        HalfedgeRef ha = h1->twin();
        Vec3 A = h1->vertex()->pos;
        Vec3 B = ha->vertex()->pos;
        Vec3 C = h1->next()->next()->vertex()->pos;
        Vec3 D = ha->next()->next()->vertex()->pos;
        pos = 3.0f * (A + B) / 8.0f + (C + D) / 8.0f;
        e->new_pos = pos;
    }

    // Next, we're going to split every edge in the mesh, in any order.
    // We're also going to distinguish subdivided edges that came from splitting
    // an edge in the original mesh from new edges by setting the boolean Edge::is_new.
    // Note that in this loop, we only want to iterate over edges of the original mesh.
    // Otherwise, we'll end up splitting edges that we just split (and the
    // loop will never end!)
    for (EdgeRef e = edges_begin(); e != edges_end(); e++)
    {
        e->is_new = false;
    }

    size_t n = n_edges();
    EdgeRef e = edges_begin();
    EdgeRef nextEdge = e;
    for (size_t i = 0; i < n; i++)
    {
        nextEdge++;
        if (e->is_new == false)
        {
            VertexRef vnew = split_edge(e).value();
            vnew->is_new = true;
            vnew->pos = e->new_pos;
        }
        e = nextEdge;
    }

    // Now flip any new edge that connects an old and new vertex.
    for (e = edges_begin(); e != edges_end(); e++)
    {
        if (e->is_new)
        {
            HalfedgeRef h1 = e->halfedge();
            HalfedgeRef ha = h1->twin();
            if (h1->vertex()->is_new != ha->vertex()->is_new)
            {
                flip_edge(e);
            }
        }
    }
    // Finally, copy new vertex positions into the Vertex::pos.
    for (VertexRef v = vertices_begin(); v != vertices_end(); v++)
    {
        if (!v->is_new)
        {
            v->pos = v->new_pos;
        }

    }
}

/*
    Isotropic remeshing. Note that this function returns success in a similar
    manner to the local operations, except with only a boolean value.
    (e.g. you may want to return false if this is not a triangle mesh)
*/
bool Halfedge_Mesh::isotropic_remesh()
{

    // Compute the mean edge length.
    // Repeat the four main steps for 5 or 6 iterations
    // -> Split edges much longer than the target length (being careful about
    //    how the loop is written!)
    // -> Collapse edges much shorter than the target length.  Here we need to
    //    be EXTRA careful about advancing the loop, because many edges may have
    //    been destroyed by a collapse (which ones?)
    // -> Now flip each edge if it improves vertex degree
    // -> Finally, apply some tangential smoothing to the vertex positions

    // Note: if you erase elements in a local operation, they will not be actually deleted
    // until do_erase or validate is called. This is to facilitate checking
    // for dangling references to elements that will be erased.
    // The rest of the codebase will automatically call validate() after each op,
    // but here simply calling collapse_edge() will not erase the elements.
    // You should use collapse_edge_erase() instead for the desired behavior.

    return false;
}

/* Helper type for quadric simplification */
struct Edge_Record
{
    Edge_Record()
    {
    }

    Edge_Record(std::unordered_map<Halfedge_Mesh::VertexRef, Mat4> &vertex_quadrics,
                Halfedge_Mesh::EdgeRef e)
            : edge(e)
    {

        // Compute the combined quadric from the edge endpoints.
        // -> Build the 3x3 linear system whose solution minimizes the quadric error
        //    associated with these two endpoints.
        // -> Use this system to solve for the optimal position, and store it in
        //    Edge_Record::optimal.
        // -> Also store the cost associated with collapsing this edge in
        //    Edge_Record::cost.
    }

    Halfedge_Mesh::EdgeRef edge;
    Vec3 optimal;
    float cost;
};

/* Comparison operator for Edge_Records so std::set will properly order them */
bool operator<(const Edge_Record &r1, const Edge_Record &r2)
{
    if (r1.cost != r2.cost)
    {
        return r1.cost < r2.cost;
    }
    Halfedge_Mesh::EdgeRef e1 = r1.edge;
    Halfedge_Mesh::EdgeRef e2 = r2.edge;
    return &*e1 < &*e2;
}

/** Helper type for quadric simplification
 *
 * A PQueue is a minimum-priority queue that
 * allows elements to be both inserted and removed from the
 * queue.  Together, one can easily change the priority of
 * an item by removing it, and re-inserting the same item
 * but with a different priority.  A priority queue, for
 * those who don't remember or haven't seen it before, is a
 * data structure that always keeps track of the item with
 * the smallest priority or "score," even as new elements
 * are inserted and removed.  Priority queues are often an
 * essential component of greedy algorithms, where one wants
 * to iteratively operate on the current "best" element.
 *
 * PQueue is templated on the type T of the object
 * being queued.  For this reason, T must define a comparison
 * operator of the form
 *
 *    bool operator<( const T& t1, const T& t2 )
 *
 * which returns true if and only if t1 is considered to have a
 * lower priority than t2.
 *
 * Basic use of a PQueue might look
 * something like this:
 *
 *    // initialize an empty queue
 *    PQueue<myItemType> queue;
 *
 *    // add some items (which we assume have been created
 *    // elsewhere, each of which has its priority stored as
 *    // some kind of internal member variable)
 *    queue.insert( item1 );
 *    queue.insert( item2 );
 *    queue.insert( item3 );
 *
 *    // get the highest priority item currently in the queue
 *    myItemType highestPriorityItem = queue.top();
 *
 *    // remove the highest priority item, automatically
 *    // promoting the next-highest priority item to the top
 *    queue.pop();
 *
 *    myItemType nextHighestPriorityItem = queue.top();
 *
 *    // Etc.
 *
 *    // We can also remove an item, making sure it is no
 *    // longer in the queue (note that this item may already
 *    // have been removed, if it was the 1st or 2nd-highest
 *    // priority item!)
 *    queue.remove( item2 );
 *
 */
template<class T>
struct PQueue
{
    void insert(const T &item)
    {
        queue.insert(item);
    }

    void remove(const T &item)
    {
        if (queue.find(item) != queue.end())
        {
            queue.erase(item);
        }
    }

    const T &top(void) const
    {
        return *(queue.begin());
    }

    void pop(void)
    {
        queue.erase(queue.begin());
    }

    size_t size()
    {
        return queue.size();
    }

    std::set<T> queue;
};

/*
    Mesh simplification. Note that this function returns success in a similar
    manner to the local operations, except with only a boolean value.
    (e.g. you may want to return false if you can't simplify the mesh any
    further without destroying it.)
*/
bool Halfedge_Mesh::simplify()
{

    std::unordered_map<VertexRef, Mat4> vertex_quadrics;
    std::unordered_map<FaceRef, Mat4> face_quadrics;
    std::unordered_map<EdgeRef, Edge_Record> edge_records;
    PQueue<Edge_Record> edge_queue;

    // Compute initial quadrics for each face by simply writing the plane equation
    // for the face in homogeneous coordinates. These quadrics should be stored
    // in face_quadrics
    // -> Compute an initial quadric for each vertex as the sum of the quadrics
    //    associated with the incident faces, storing it in vertex_quadrics
    // -> Build a priority queue of edges according to their quadric error cost,
    //    i.e., by building an Edge_Record for each edge and sticking it in the
    //    queue. You may want to use the above PQueue<Edge_Record> for this.
    // -> Until we reach the target edge budget, collapse the best edge. Remember
    //    to remove from the queue any edge that touches the collapsing edge
    //    BEFORE it gets collapsed, and add back into the queue any edge touching
    //    the collapsed vertex AFTER it's been collapsed. Also remember to assign
    //    a quadric to the collapsed vertex, and to pop the collapsed edge off the
    //    top of the queue.

    // Note: if you erase elements in a local operation, they will not be actually deleted
    // until do_erase or validate are called. This is to facilitate checking
    // for dangling references to elements that will be erased.
    // The rest of the codebase will automatically call validate() after each op,
    // but here simply calling collapse_edge() will not erase the elements.
    // You should use collapse_edge_erase() instead for the desired behavior.

    return false;
}
