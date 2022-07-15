
#include "../scene/skeleton.h"
#define TAU 0.001f;

Vec3 closest_on_line_segment(Vec3 start, Vec3 end, Vec3 point)
{

    // TODO(Animation): Task 3

    // Return the closest point to 'point' on the line segment from start to end
    Vec3 gradient = end - start;
    float len = gradient.norm();
    gradient.normalize();
    Vec3 v = point - start;
    float d = dot(v, gradient);
    if (d > len) d = len;
    if (d < 0.0f) d = 0.0f;
    return start + gradient * d;
}

Mat4 Joint::joint_to_bind() const
{

    // TODO(Animation): Task 2

    // Return a matrix transforming points in the space of this joint
    // to points in skeleton space in bind position.

    // Bind position implies that all joints have pose = Vec3{0.0f}

    // You will need to traverse the joint heirarchy. This should
    // not take into account Skeleton::base_pos
    Vec3 disp;
    if (is_root())
    {
        return Mat4::I;
    }
    Joint *curJoint = parent;
    while (!curJoint->is_root())
    {
        disp += curJoint->extent;
        curJoint = curJoint->parent;
    }
    disp += curJoint->extent;
    //disp += extent;
    return Mat4::translate(disp);
}

Mat4 Joint::joint_to_posed() const
{

    // TODO(Animation): Task 2

    // Return a matrix transforming points in the space of this joint
    // to points in skeleton space, taking into account joint poses.

    // You will need to traverse the joint heirarchy. This should
    // not take into account Skeleton::base_pos
    if (is_root())
    {
        return Mat4::euler(pose);
    }
    Joint *curJoint = parent;
    Mat4 acc = Mat4::I;
    while (!curJoint->is_root())
    {
        acc = Mat4::euler(curJoint->pose) * Mat4::translate(curJoint->extent) * acc;
        curJoint = curJoint->parent;
    }
    acc = Mat4::euler(curJoint->pose) * Mat4::translate(curJoint->extent) * acc;
    acc = acc * Mat4::euler(pose);
    return acc;
}

Vec3 Skeleton::end_of(Joint *j)
{

    // TODO(Animation): Task 2

    // Return the bind position of the endpoint of joint j in object space.
    // This should take into account Skeleton::base_pos.
    Mat4 acc = j->joint_to_bind();
    return Mat4::translate(base_pos) * acc * j->extent;
}

Vec3 Skeleton::posed_end_of(Joint *j)
{

    // TODO(Animation): Task 2

    // Return the posed position of the endpoint of joint j in object space.
    // This should take into account Skeleton::base_pos.
    Mat4 acc = j->joint_to_posed();
    return Mat4::translate(base_pos) * acc * j->extent;
}

Mat4 Skeleton::joint_to_bind(const Joint *j) const
{

    // TODO(Animation): Task 2

    // Return a matrix transforming points in joint j's space to object space in
    // bind position. This should take into account Skeleton::base_pos.
    return Mat4::translate(base_pos) * j->joint_to_bind();
}

Mat4 Skeleton::joint_to_posed(const Joint *j) const
{

    // TODO(Animation): Task 2

    // Return a matrix transforming points in joint j's space to object space with
    // poses. This should take into account Skeleton::base_pos.
    return Mat4::translate(base_pos) * j->joint_to_posed();
}

void Skeleton::find_joints(const GL::Mesh &mesh, std::vector<std::vector<Joint *>> &map)
{

    // TODO(Animation): Task 3

    // Construct a mapping: vertex index -> list of joints that should effect the vertex.
    // A joint should effect a vertex if it is within Joint::radius distance of the
    // bone's line segment in bind position.

    const std::vector<GL::Mesh::Vert> &verts = mesh.verts();
    map.resize(verts.size());

    // For each i in [0, verts.size()), map[i] should contain the list of joints that
    // effect vertex i. Note that i is NOT Vert::id! i is the index in verts.
    for (size_t i = 0; i < verts.size(); i++)
    {
        for_joints([&](Joint *j)
                   {
                       // What vertices does joint j effect?
                       Mat4 j2b = joint_to_bind(j);
                       Vec3 end = (j2b * j->extent);
                       Vec3 diff = closest_on_line_segment(j2b * Vec3(), end, verts[i].pos) - verts[i].pos;
                       float dist = diff.norm();
                       if (j->radius >= dist)
                       {
                           map[i].push_back(j);
                       }
                   });

    }

}

void Skeleton::skin(const GL::Mesh &input, GL::Mesh &output,
                    const std::vector<std::vector<Joint *>> &map)
{

    // TODO(Animation): Task 3

    // Apply bone poses & weights to the vertices of the input (bind position) mesh
    // and store the result in the output mesh. See the task description for details.
    // map was computed by find_joints, hence gives a mapping from vertex index to
    // the list of bones the vertex should be effected by.

    // Currently, this just copies the input to the output without modification.

    std::vector<GL::Mesh::Vert> verts = input.verts();

    for (size_t i = 0; i < verts.size(); i++)
    {

        // Skin vertex i. Note that its position is given in object bind space.
        float sumInvDist = 0.0f;
        std::vector<Joint *> joints = map[i];
        std::vector<float> invdists;
        for (size_t k = 0; k < joints.size(); k++)
        {
            Joint *j = joints[k];
            Mat4 j2b = joint_to_bind(j);
            Vec3 end = (j2b * j->extent);
            Vec3 diff = closest_on_line_segment(j2b * Vec3{}, end, verts[i].pos) - verts[i].pos;
            float dist = diff.norm();
            sumInvDist += 1.0f / dist;
            invdists.push_back(1.0f / dist);
        }
        Vec3 newPos;
        Vec3 newNorm;
        for (size_t k = 0; k < joints.size(); k++)
        {
            Joint *j = joints[k];
            float wij = invdists[k] / sumInvDist;
            Mat4 j2p = joint_to_posed(j);
            Mat4 j2b = joint_to_bind(j);
            Vec3 vij = j2p * (j2b.inverse() * verts[i].pos);
            Vec3 vijnorm = j2p * (j2b.inverse() * verts[i].norm);
            newPos += wij * vij;
            newNorm += wij * vijnorm;
        }
        verts[i].pos = newPos;
        verts[i].norm = newNorm;
    }

    std::vector<GL::Mesh::Index> idxs = input.indices();
    output.recreate(std::move(verts), std::move(idxs));
}

void Joint::compute_gradient(Vec3 target, Vec3 current)
{

    // TODO(Animation): Task 2

    // Computes the gradient of IK energy for this joint and, should be called
    // recursively upward in the heirarchy. Each call should storing the result
    // in the angle_gradient for this joint.

    // Target is the position of the IK handle in skeleton space.
    // Current is the end position of the IK'd joint in skeleton space.
    Mat4 posed = joint_to_posed();
    Vec3 jacobx = cross(posed.rotate(Vec3{1.0f, 0.0f, 0.0f}), current - posed * Vec3());
    Vec3 jacoby = cross(posed.rotate(Vec3{0.0f, 1.0f, 0.0f}), current - posed * Vec3());
    Vec3 jacobz = cross(posed.rotate(Vec3{0.0f, 0.0f, 1.0f}), current - posed * Vec3());
    float gradientx = dot(jacobx, current - target);
    float gradienty = dot(jacoby, current - target);
    float gradientz = dot(jacobz, current - target);
    if (!is_root())
    {
        parent->compute_gradient(target, current);
    }
    angle_gradient += Vec3(gradientx, gradienty, gradientz);
}

void Skeleton::step_ik(std::vector<IK_Handle *> active_handles)
{

    // TODO(Animation): Task 2

    // Do several iterations of Jacobian Transpose gradient descent for IK
    for (size_t iter = 0; iter < 55; iter++)
    {
        for (size_t i = 0; i < active_handles.size(); i++)
        {
            IK_Handle *handle = active_handles[i];
            handle->joint->compute_gradient(handle->target, posed_end_of(handle->joint) - base_pos);

        }
        auto update = [](Joint *j)
        {
            j->pose -= (j->angle_gradient) * TAU;
            j->angle_gradient = Vec3();
        };
        for_joints(update);
    }

}
