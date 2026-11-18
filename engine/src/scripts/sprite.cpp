#include <hellbliss/scripts/sprite.hpp>

#include <hellbliss/object.hpp>

#include <hellbliss/engine.hpp>

namespace hellbliss::scripts {
    void Sprite::render() {
        auto& obj = object();
        if (texture) {
            const float scale = std::min(static_cast<float>(obj.size.x) / texture->width(), static_cast<float>(obj.size.y) / texture->height());
            vec2i size = {static_cast<int>(texture->width() * scale), static_cast<int>(texture->height() * scale)};
            vec2i pos = {obj.pos.x + (obj.size.x - size.x) / 2, obj.pos.y + (obj.size.y - size.y) / 2};

            object().engine().renderer().draw_texture(pos, size, *texture, color, shader);
        } else object().engine().renderer().draw_rect(obj.pos.xy(), obj.size, color, shader);
    }
}
