#include "sphere.h"
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <utility>
#include <vector>

using namespace HinaPE::HinaGeom;

Sphere::Sphere(Vertex3Matrix V, TriFaceMatrix F) : VertexInfo(std::move(V)), IndexedTriangleMesh(std::move(F))
{
    VertexInfo::build();
    IndexedTriangleMesh::build();
}

Sphere *Sphere::build_Sphere(Real radius)
{
    typedef OpenMesh::PolyMesh_ArrayKernelT<> MyMesh;

    MyMesh mesh;
    // generate vertices
    MyMesh::VertexHandle vhandle[8];
    vhandle[0] = mesh.add_vertex(MyMesh::Point(-1, -1, 1));
    vhandle[1] = mesh.add_vertex(MyMesh::Point(1, -1, 1));
    vhandle[2] = mesh.add_vertex(MyMesh::Point(1, 1, 1));
    vhandle[3] = mesh.add_vertex(MyMesh::Point(-1, 1, 1));
    vhandle[4] = mesh.add_vertex(MyMesh::Point(-1, -1, -1));
    vhandle[5] = mesh.add_vertex(MyMesh::Point(1, -1, -1));
    vhandle[6] = mesh.add_vertex(MyMesh::Point(1, 1, -1));
    vhandle[7] = mesh.add_vertex(MyMesh::Point(-1, 1, -1));
    // generate (quadrilateral) faces
    std::vector<MyMesh::VertexHandle> face_vhandles;
    face_vhandles.clear();
    face_vhandles.push_back(vhandle[0]);
    face_vhandles.push_back(vhandle[1]);
    face_vhandles.push_back(vhandle[2]);
    face_vhandles.push_back(vhandle[3]);
    mesh.add_face(face_vhandles);

    face_vhandles.clear();
    face_vhandles.push_back(vhandle[7]);
    face_vhandles.push_back(vhandle[6]);
    face_vhandles.push_back(vhandle[5]);
    face_vhandles.push_back(vhandle[4]);
    mesh.add_face(face_vhandles);
    face_vhandles.clear();
    face_vhandles.push_back(vhandle[1]);
    face_vhandles.push_back(vhandle[0]);
    face_vhandles.push_back(vhandle[4]);
    face_vhandles.push_back(vhandle[5]);
    mesh.add_face(face_vhandles);
    face_vhandles.clear();
    face_vhandles.push_back(vhandle[2]);
    face_vhandles.push_back(vhandle[1]);
    face_vhandles.push_back(vhandle[5]);
    face_vhandles.push_back(vhandle[6]);
    mesh.add_face(face_vhandles);
    face_vhandles.clear();
    face_vhandles.push_back(vhandle[3]);
    face_vhandles.push_back(vhandle[2]);
    face_vhandles.push_back(vhandle[6]);
    face_vhandles.push_back(vhandle[7]);
    mesh.add_face(face_vhandles);
    face_vhandles.clear();
    face_vhandles.push_back(vhandle[0]);
    face_vhandles.push_back(vhandle[3]);
    face_vhandles.push_back(vhandle[7]);
    face_vhandles.push_back(vhandle[4]);
    mesh.add_face(face_vhandles);

    mesh.triangulate();

    Vertex3Matrix V;
    TriFaceMatrix F;
    EdgeMatrix E;

    V.resize(static_cast<Eigen::Index>(mesh.n_vertices()), 3);
    F.resize(static_cast<Eigen::Index>(mesh.n_faces()), 3);
    E.resize(static_cast<Eigen::Index>(mesh.n_edges()), 2);

    int i;

    {
        MyMesh::Point p;
        i = 0;
        for (auto it = mesh.vertices_begin(); it != mesh.vertices_end(); ++it)
        {
            p = mesh.point(*it);
            V.row(i++) = Vector3r(p[0], p[1], p[2]).transpose();
        }
        assert(i == mesh.n_vertices());
    }

    {
        MyMesh::CFVIter fv_iter;
        std::vector<MyMesh::VertexHandle> _vertices;
        i = 0;
        for (auto it = mesh.faces_begin(); it != mesh.faces_end(); ++it)
        {
            _vertices.clear();
            fv_iter = mesh.cfv_iter(*it);
            while (fv_iter.is_valid())
            {
                _vertices.push_back(*fv_iter);
                ++fv_iter;
            }
            assert(_vertices.size() == 3);
            F.row(i++) = Vector3i(_vertices[0].idx(), _vertices[1].idx(), _vertices[2].idx()).transpose();
        }
        assert(i == mesh.n_faces());
    }

    {
        i = 0;
        for (auto it = mesh.edges_begin(); it != mesh.edges_end(); ++it)
        {
            auto e = *it;
            E.row(i++) = Vector2i(e.v0().idx(), e.v1().idx()).transpose();
        }
        assert(i == mesh.n_edges());
    }

//    {
//        MyMesh::H
//    }

    log<Vertex3Matrix>(V);
    log<TriFaceMatrix>(F);
    log<EdgeMatrix>(E);

    return new Sphere{V, F};
}
