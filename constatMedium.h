#pragma once

#include "utils.h"

#include "hittable.h"
#include "material.h"
#include "texture.h"

class constant_medium : public hittable {
public:
    constant_medium(shared_ptr<hittable> b, double d, shared_ptr<texture> a)
        : boundary(b),
        neg_inv_density(-1 / d),
        phase_function(make_shared<isotropic>(a))
    {}

    constant_medium(shared_ptr<hittable> b, double d, color c)
        : boundary(b),
        neg_inv_density(-1 / d),
        phase_function(make_shared<isotropic>(c))
    {}

    virtual bool hit(
        const ray& r, double tMin, double tMax, hitRecord& record) const override;

    virtual bool getAABB(double startTime, double endTime, aabb& outputBox) const override {
        return boundary->getAABB(startTime, endTime, outputBox);
    }

public:
    shared_ptr<hittable> boundary;
    shared_ptr<material> phase_function;
    double neg_inv_density;
};

bool constant_medium::hit(const ray& r, double t_min, double t_max, hitRecord& record) const {
    // Print occasional samples when debugging. To enable, set enableDebug true.
    const bool enableDebug = false;
    const bool debugging = enableDebug && randomDouble() < 0.00001;

    hitRecord rec1, rec2;

    if (!boundary->hit(r, -infinity, infinity, rec1))
        return false;

    if (!boundary->hit(r, rec1.t + 0.0001, infinity, rec2))
        return false;

    if (debugging) std::cerr << "\nt_min=" << rec1.t << ", t_max=" << rec2.t << '\n';

    if (rec1.t < t_min) rec1.t = t_min;
    if (rec2.t > t_max) rec2.t = t_max;

    if (rec1.t >= rec2.t)
        return false;

    if (rec1.t < 0)
        rec1.t = 0;

    const auto ray_length = r.getDirection().length();
    const auto distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
    const auto hit_distance = neg_inv_density * log(randomDouble());

    if (hit_distance > distance_inside_boundary)
        return false;

    record.t = rec1.t + hit_distance / ray_length;
    record.point = r.resize(record.t);

    if (debugging) {
        std::cerr << "hit_distance = " << hit_distance << '\n'
            << "record.t = " << record.t << '\n'
            << "record.p = " << record.point << '\n';
    }

    record.normal = vec3(1, 0, 0);  // arbitrary
    record.isFrontFace = true;     // also arbitrary
    record.material_ptr = phase_function;

    return true;
}
