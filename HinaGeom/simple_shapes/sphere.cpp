#include "sphere.h"

#include <utility>

#include "pmp/SurfaceMesh.h"
#include "pmp/algorithms/SurfaceTriangulation.h"
#include "pmp/algorithms/SurfaceSubdivision.h"
#include "pmp/algorithms/SurfaceFactory.h"

using namespace HinaPE::HinaGeom;

using namespace pmp;

Sphere::Sphere(Vertex3Matrix V, TriFaceMatrix F) : VertexInfo(std::move(V)), IndexedTriangleMesh(std::move(F))
{
    VertexInfo::build();
    IndexedTriangleMesh::build();
}

Sphere *Sphere::build_Sphere(Real radius)
{
    SurfaceMesh mesh;

    int n_slices = 10 * 2;
    int n_stacks = 10 * 2;

    // add top vertex
    auto v0 = mesh.add_vertex(Point(0, radius, 0));

    // generate vertices per stack / slice
    for (int i = 0; i < n_stacks - 1; i++)
    {
        Real phi = EIGEN_PI * float(i + 1) / float(n_stacks);
        for (int j = 0; j < n_slices; j++)
        {
            Real theta = 2.0 * EIGEN_PI * float(j) / float(n_slices);
            auto xt = radius * sin(phi) * cos(theta);
            auto yt = radius * cos(phi);
            auto zt = radius * sin(phi) * sin(theta);
            mesh.add_vertex(Point(xt, yt, zt));
        }
    }

    // add bottom vertex
    auto v1 = mesh.add_vertex(Point(0, -radius, 0));

    // add top / bottom triangles
    for (int i = 0; i < n_slices; ++i)
    {
        auto i0 = i + 1;
        auto i1 = (i + 1) % n_slices + 1;
        mesh.add_triangle(v0, Vertex(i1), Vertex(i0));
        i0 = i + n_slices * (n_stacks - 2) + 1;
        i1 = (i + 1) % n_slices + n_slices * (n_stacks - 2) + 1;
        mesh.add_triangle(v1, Vertex(i0), Vertex(i1));
    }

    // add quads per stack / slice
    for (int j = 0; j < n_stacks - 2; j++)
    {
        auto j0 = j * n_slices + 1;
        auto j1 = (j + 1) * n_slices + 1;
        for (int i = 0; i < n_slices; i++)
        {
            auto i0 = j0 + i;
            auto i1 = j0 + (i + 1) % n_slices;
            auto i2 = j1 + (i + 1) % n_slices;
            auto i3 = j1 + i;
            mesh.add_quad(Vertex(i0), Vertex(i1),
                          Vertex(i2), Vertex(i3));
        }
    }

    SurfaceTriangulation st(mesh);
    st.triangulate();

    auto vp = mesh.vertex_properties();
    auto ep = mesh.edge_properties();
    auto fp = mesh.face_properties();

    Vertex3Matrix V;
    V.resize(static_cast<Eigen::Index>(mesh.n_vertices()), 3);
    {
        auto points = mesh.get_vertex_property<Point>("v:point");
        int it = 0;
        for (auto &&v: mesh.vertices())
        {
            Point &p = points[v];
            V.row(it) = static_cast<Vector3r>(p).transpose();
            ++it;
        }
    }

    TriFaceMatrix F;
    F.resize(static_cast<Eigen::Index>(mesh.n_faces()), 3);
    {
        int it = 0;
        for (auto &&f: mesh.faces())
        {
            SurfaceMesh::VertexAroundFaceCirculator vs = mesh.vertices(f);
            Vector3i _f;
            int tit = 0;
            for (auto &&v: vs)
            {
                assert(tit < 3); // ensure triangulated
                _f[tit++] = static_cast<Eigen::DenseCoeffsBase<Eigen::Matrix<int, 3, 1, 0>, 1>::Scalar>(v.idx());
            }
            F.row(it) = _f.transpose();
            ++it;
        }
    }

//    EdgeMatrix E;
//    E.resize(static_cast<Eigen::Index>(mesh.n_edges()), 3);
//    {
//        SurfaceMesh::CirCulator
//        int it = 0;
//        for (auto &&e: mesh.edges())
//        {
//
//        }
//    }

    for (auto &&f: SurfaceMesh::FaceContainer(mesh.faces_begin(), mesh.faces_end()))
    {

    }

    log<Vertex3Matrix>(V);
    log<TriFaceMatrix>(F);

    return new Sphere{V, F};
}
