#include <pangolin/gui/draw_layer.h>
#include <pangolin/context/factory.h>
#include <pangolin/handler/handler.h>
#include <pangolin/gl/glplatform.h>
#include <Eigen/Core>

#include "glutils.h"

#include <unordered_map>

namespace pangolin
{

struct DrawLayerImpl : public DrawLayer {
    Eigen::Array3f debug_random_color;

    DrawLayerImpl(const DrawLayerImpl::Params& p)
        : size_hint_(p.size_hint),
        handler_(p.handler),
        cam_from_world_(p.cam_from_world),
        intrinsic_k_(p.intrinsic_k),
        non_linear_(p.non_linear),
        objects_(p.objects)
    {
        debug_random_color = (Eigen::Array3f::Random() + 1.0f) / 2.0;

    }

    void renderIntoRegion(const RenderParams& p) override {
        ScopedGlEnable en_scissor(GL_SCISSOR_TEST);
        setGlViewport(p.region);
        setGlScissor(p.region);
        glClearColor(debug_random_color[0], debug_random_color[2], debug_random_color[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    Size sizeHint() const override {
        return size_hint_;
    }

    void setProjection(
        const Eigen::Matrix4d& intrinsic_k,
        const NonLinearMethod non_linear = {},
        double duration_seconds = 0.0) override {

    }

    void setCamFromWorld(
        const Eigen::Matrix4d& cam_from_world,
        double duration_seconds = 0.0) override {

    }

    void setHandler(const std::shared_ptr<Handler>& handler) override {
        handler_ = handler;
    }

    MinMax<Eigen::Vector3d> getSceneBoundsInWorld() const override {
        return bounds_;
    }

    void add(const Shared<Renderable>& r) override {
        objects_.push_back(r);
    }

    void remove(const Shared<Renderable>& r) override {
        throw std::runtime_error("Not implemented yet...");
    }

    void clear() override {
        objects_.clear();
    }

    Size size_hint_;
    MinMax<Eigen::Vector3d> bounds_;
    std::shared_ptr<Handler> handler_;

    Eigen::Matrix4d cam_from_world_;
    Eigen::Matrix4d intrinsic_k_;
    NonLinearMethod non_linear_;

    std::vector<Shared<Renderable>> objects_;
};

PANGO_CREATE(DrawLayer) {
    return Shared<DrawLayerImpl>::make(p);
}

}