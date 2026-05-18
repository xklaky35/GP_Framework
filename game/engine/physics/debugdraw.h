#pragma once

#include <GL/glew.h>
#include <box2d/box2d.h>

namespace Engine {

    class DebugDraw {
    public:
        DebugDraw();

        ~DebugDraw();

        // Call once after GL context is created.
        // Pass the world extents you used in gluOrtho2D so we can rebuild the
        // same orthographic projection in the shader.
        bool Initialise(float left = -25.0f, float right = 25.0f,
                        float bottom = -25.0f, float top = 25.0f);

        // Returns a b2DebugDraw fully wired to this object's callbacks.
        b2DebugDraw BuildDebugDraw();

        // ── Callbacks (public so free functions can forward to them) ─────────────
        void OnDrawPolygon(const b2Vec2 *verts, int count, b2HexColor color);
        void OnDrawSolidPolygon(b2Transform xf, const b2Vec2 *verts, int count, float radius, b2HexColor color);
        void OnDrawCircle(b2Vec2 center, float radius, b2HexColor color);
        void OnDrawSolidCircle(b2Transform xf, float radius, b2HexColor color);
        void OnDrawSolidCapsule(b2Vec2 p1, b2Vec2 p2, float radius, b2HexColor color);
        void OnDrawLine(b2Vec2 p1, b2Vec2 p2, b2HexColor color);
        void OnDrawTransform(b2Transform xf);
        void OnDrawPoint(b2Vec2 p, float size, b2HexColor color);
        void OnDrawString(b2Vec2 p, const char *s, b2HexColor color);

        static void MakeOrtho(float *m16,
                              float l, float r, float b, float t, float x, float y);

        // ── Cached ortho matrix (column-major) ───────────────────────────────────
        float m_proj[16] = {};

    private:
        // ── GL resources ─────────────────────────────────────────────────────────
        GLuint m_vao = 0;
        GLuint m_vbo = 0;
        GLuint m_shaderProg = 0;

        // ── Shader uniform locations ──────────────────────────────────────────────
        GLint m_locColour = -1; // vec4  uColour
        GLint m_locProj = -1; // mat4  uProj


        // ── Helpers ──────────────────────────────────────────────────────────────
        void FlushLines(const float *xy, int count, float r, float g, float b, float a) const;
        void FlushTris(const float *xy, int count, float r, float g, float b, float a) const;
        void FlushPoints(const float *xy, int count, float size, float r, float g, float b, float a) const;
        void BuildCircleLines(float cx, float cy, float radius, float *out, int &outCount, int segments = 32);
        void BuildCircleFan(float cx, float cy, float radius, float *out, int &outCount, int segments = 32);

        static bool CompileShader(GLuint id, const char *src);
        static void UnpackColor(b2HexColor hex, float &r, float &g, float &b);
        static b2Vec2 TransformPt(b2Transform xf, b2Vec2 local);

        // scratch buffer — avoids per-frame heap allocs
        static constexpr int s_iScratchFloats = 8192;
        float m_scratch[s_iScratchFloats] = {};
    };
}
