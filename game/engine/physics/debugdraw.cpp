#include "debugdraw.h"
#include <cmath>
#include <cstring>
#include <cstdio>
#include <cassert>

#include "physicsmanager.h"
#include "../../config/config.h"

namespace Engine {
    // ─────────────────────────────────────────────────────────────────────────────
    //  Minimal colour-only vertex / fragment shaders
    // ─────────────────────────────────────────────────────────────────────────────
    static const char *kVertSrc = R"GLSL(
#version 330 core
layout(location = 0) in vec2 aPos;
uniform mat4 uProj;
void main()
{
    gl_Position = uProj * vec4(aPos, 0.0, 1.0);
}
)GLSL";

    static const char *kFragSrc = R"GLSL(
#version 330 core
uniform vec4 uColour;
out vec4 FragColor;
void main()
{
    FragColor = uColour;
}
)GLSL";

    // ─────────────────────────────────────────────────────────────────────────────
    //  Free-function trampolines (b2DebugDraw stores plain C function pointers)
    // ─────────────────────────────────────────────────────────────────────────────
    static void DrawPolygonFcn(const b2Vec2 *v, int n, b2HexColor c, void *ctx) {
        static_cast<DebugDraw *>(ctx)->OnDrawPolygon(v, n, c);
    }

    static void DrawSolidPolygonFcn(b2Transform xf, const b2Vec2 *v, int n, float r, b2HexColor c, void *ctx) {
        static_cast<DebugDraw *>(ctx)->OnDrawSolidPolygon(xf, v, n, r, c);
    }

    static void DrawCircleFcn(b2Vec2 center, float r, b2HexColor c, void *ctx) {
        static_cast<DebugDraw *>(ctx)->OnDrawCircle(center, r, c);
    }

    static void DrawSolidCircleFcn(b2Transform xf, float r, b2HexColor c, void *ctx) {
        static_cast<DebugDraw *>(ctx)->OnDrawSolidCircle(xf, r, c);
    }

    static void DrawSolidCapsuleFcn(b2Vec2 p1, b2Vec2 p2, float r, b2HexColor c, void *ctx) {
        static_cast<DebugDraw *>(ctx)->OnDrawSolidCapsule(p1, p2, r, c);
    }

    static void DrawLineFcn(b2Vec2 p1, b2Vec2 p2, b2HexColor c, void *ctx) {
        static_cast<DebugDraw *>(ctx)->OnDrawLine(p1, p2, c);
    }

    static void DrawTransformFcn(b2Transform xf, void *ctx) {
        static_cast<DebugDraw *>(ctx)->OnDrawTransform(xf);
    }

    static void DrawPointFcn(b2Vec2 p, float size, b2HexColor c, void *ctx) {
        static_cast<DebugDraw *>(ctx)->OnDrawPoint(p, size, c);
    }

    static void DrawStringFcn(b2Vec2 p, const char *s, b2HexColor c, void *ctx) {
        static_cast<DebugDraw *>(ctx)->OnDrawString(p, s, c);
    }

    // ─────────────────────────────────────────────────────────────────────────────
    //  DebugDraw
    // ─────────────────────────────────────────────────────────────────────────────
    DebugDraw::DebugDraw() = default;

    DebugDraw::~DebugDraw() {
        if (m_vbo) {
            glDeleteBuffers(1, &m_vbo);
            m_vbo = 0;
        }
        if (m_vao) {
            glDeleteVertexArrays(1, &m_vao);
            m_vao = 0;
        }
        if (m_shaderProg) {
            glDeleteProgram(m_shaderProg);
            m_shaderProg = 0;
        }
    }

    bool DebugDraw::Initialise(float left, float right, float bottom, float top) {
        // ── Compile shader ────────────────────────────────────────────────────────
        GLuint vert = glCreateShader(GL_VERTEX_SHADER);
        GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
        if (!CompileShader(vert, kVertSrc) || !CompileShader(frag, kFragSrc)) {
            glDeleteShader(vert);
            glDeleteShader(frag);
            return false;
        }
        m_shaderProg = glCreateProgram();
        glAttachShader(m_shaderProg, vert);
        glAttachShader(m_shaderProg, frag);
        glLinkProgram(m_shaderProg);
        glDeleteShader(vert);
        glDeleteShader(frag);

        GLint ok = 0;
        glGetProgramiv(m_shaderProg, GL_LINK_STATUS, &ok);
        if (!ok) return false;

        m_locColour = glGetUniformLocation(m_shaderProg, "uColour");
        m_locProj = glGetUniformLocation(m_shaderProg, "uProj");

        // ── VAO / VBO (dynamic, updated each draw call) ───────────────────────────
        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &m_vbo);

        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, kScratchFloats * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
        glBindVertexArray(0);

        // ── Build ortho projection ────────────────────────────────────────────────
        MakeOrtho(m_proj, left, right, bottom, top, 0,0);


        return true;
    }

    b2DebugDraw DebugDraw::BuildDebugDraw() {
        b2DebugDraw dd = b2DefaultDebugDraw();
        dd.DrawPolygonFcn = DrawPolygonFcn;
        dd.DrawSolidPolygonFcn = DrawSolidPolygonFcn;
        dd.DrawCircleFcn = DrawCircleFcn;
        dd.DrawSolidCircleFcn = DrawSolidCircleFcn;
        dd.DrawSolidCapsuleFcn = DrawSolidCapsuleFcn;
        dd.DrawLineFcn = DrawLineFcn;
        dd.DrawTransformFcn = DrawTransformFcn;
        dd.DrawPointFcn = DrawPointFcn;
        dd.DrawStringFcn = DrawStringFcn;
        dd.drawShapes = true;
        dd.drawJoints = true;
        dd.drawMass = false;
        dd.context = this;
        return dd;
    }

    // ─────────────────────────────────────────────────────────────────────────────
    //  Callbacks
    // ─────────────────────────────────────────────────────────────────────────────
    void DebugDraw::OnDrawPolygon(const b2Vec2 *verts, int count, b2HexColor color) {
        assert(count * 2 <= kScratchFloats);
        for (int i = 0; i < count; ++i) {
            m_scratch[i * 2 + 0] = verts[i].x;
            m_scratch[i * 2 + 1] = verts[i].y;
        }
        float r, g, b;
        UnpackColor(color, r, g, b);
        FlushLines(m_scratch, count, r, g, b, 1.0f);
    }

    void DebugDraw::OnDrawSolidPolygon(b2Transform xf, const b2Vec2 *verts, int count,
                                       float radius, b2HexColor color) {
        float r, g, b;
        UnpackColor(color, r, g, b);

        assert(count * 2 <= kScratchFloats);
        for (int i = 0; i < count; ++i) {
            b2Vec2 w = TransformPt(xf, verts[i]);
            m_scratch[i * 2 + 0] = w.x;
            m_scratch[i * 2 + 1] = w.y;
        }

        FlushTris(m_scratch, count, r, g, b, 0.45f);
        FlushLines(m_scratch, count, r, g, b, 1.0f);
    }

    void DebugDraw::OnDrawCircle(b2Vec2 center, float radius, b2HexColor color) {
        float r, g, b;
        UnpackColor(color, r, g, b);
        int count = 0;
        BuildCircleLines(center.x, center.y, radius, m_scratch, count);
        FlushLines(m_scratch, count / 2, r, g, b, 1.0f);
    }

    void DebugDraw::OnDrawSolidCircle(b2Transform xf, float radius, b2HexColor color) {
        float r, g, b;
        UnpackColor(color, r, g, b);
        float cx = xf.p.x, cy = xf.p.y;

        int count = 0;
        BuildCircleFan(cx, cy, radius, m_scratch, count);
        FlushTris(m_scratch, count / 2, r, g, b, 0.45f);

        count = 0;
        BuildCircleLines(cx, cy, radius, m_scratch, count);
        FlushLines(m_scratch, count / 2, r, g, b, 1.0f);

        // Rotation indicator
        float line[4] = {cx, cy, cx + xf.q.c * radius, cy + xf.q.s * radius};
        FlushLines(line, 2, r, g, b, 1.0f);
    }

    void DebugDraw::OnDrawSolidCapsule(b2Vec2 p1, b2Vec2 p2, float radius, b2HexColor color) {
        float r, g, b;
        UnpackColor(color, r, g, b);

        float dx = p2.x - p1.x, dy = p2.y - p1.y;
        float len = sqrtf(dx * dx + dy * dy);
        float px = 0.0f, py = 0.0f;
        if (len > 1e-6f) {
            px = -dy / len;
            py = dx / len;
        }

        // Rectangle body as two explicit triangles
        float tris[12] = {
            p1.x + px * radius, p1.y + py * radius,
            p1.x - px * radius, p1.y - py * radius,
            p2.x - px * radius, p2.y - py * radius,
            p1.x + px * radius, p1.y + py * radius,
            p2.x - px * radius, p2.y - py * radius,
            p2.x + px * radius, p2.y + py * radius,
        };
        FlushTris(tris, 6, r, g, b, 0.45f);

        // End caps (filled)
        int count = 0;
        BuildCircleFan(p1.x, p1.y, radius, m_scratch, count);
        FlushTris(m_scratch, count / 2, r, g, b, 0.45f);
        count = 0;
        BuildCircleFan(p2.x, p2.y, radius, m_scratch, count);
        FlushTris(m_scratch, count / 2, r, g, b, 0.45f);

        // Outline as a closed loop
        static constexpr float kPI = 3.14159265358979f;
        const int segs = 16;
        float angle = atan2f(dy, dx);
        int oi = 0;
        for (int i = 0; i <= segs; ++i) {
            float a = angle - kPI * 0.5f + kPI * (float) i / (float) segs;
            m_scratch[oi++] = p2.x + cosf(a) * radius;
            m_scratch[oi++] = p2.y + sinf(a) * radius;
        }
        for (int i = 0; i <= segs; ++i) {
            float a = angle + kPI * 0.5f + kPI * (float) i / (float) segs;
            m_scratch[oi++] = p1.x + cosf(a) * radius;
            m_scratch[oi++] = p1.y + sinf(a) * radius;
        }
        FlushLines(m_scratch, oi / 2, r, g, b, 1.0f);
    }

    void DebugDraw::OnDrawLine(b2Vec2 p1, b2Vec2 p2, b2HexColor color) {
        float r, g, b;
        UnpackColor(color, r, g, b);
        float v[4] = {p1.x, p1.y, p2.x, p2.y};
        FlushLines(v, 2, r, g, b, 1.0f);
    }

    void DebugDraw::OnDrawTransform(b2Transform xf) {
        const float kLen = 0.4f;
        float cx = xf.p.x, cy = xf.p.y;
        float xv[4] = {cx, cy, cx + xf.q.c * kLen, cy + xf.q.s * kLen};
        float yv[4] = {cx, cy, cx - xf.q.s * kLen, cy + xf.q.c * kLen};
        FlushLines(xv, 2, 1.0f, 0.0f, 0.0f, 1.0f);
        FlushLines(yv, 2, 0.0f, 1.0f, 0.0f, 1.0f);
    }

    void DebugDraw::OnDrawPoint(b2Vec2 p, float size, b2HexColor color) {
        float r, g, b;
        UnpackColor(color, r, g, b);
        float v[2] = {p.x, p.y};
        FlushPoints(v, 1, size, r, g, b, 1.0f);
    }

    void DebugDraw::OnDrawString(b2Vec2 p, const char *s, b2HexColor color) {
        float r, g, b;
        UnpackColor(color, r, g, b);
        const float sz = 0.1f;
        float cross[8] = {p.x - sz, p.y, p.x + sz, p.y, p.x, p.y - sz, p.x, p.y + sz};
        FlushLines(cross, 4, r, g, b, 1.0f);
        // printf("[DebugDraw] (%.2f, %.2f) %s\n", p.x, p.y, s);
    }

    // ─────────────────────────────────────────────────────────────────────────────
    //  Private helpers
    // ─────────────────────────────────────────────────────────────────────────────
    void DebugDraw::FlushLines(const float *xy, int vertCount,
                               float r, float g, float b, float a) {
        if (vertCount < 2) return;
        glUseProgram(m_shaderProg);
        glUniformMatrix4fv(m_locProj, 1, GL_FALSE, m_proj);
        glUniform4f(m_locColour, r, g, b, a);
        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertCount * 2 * sizeof(float), xy);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDrawArrays(GL_LINE_LOOP, 0, vertCount);
        glBindVertexArray(0);
        glUseProgram(0);
    }

    void DebugDraw::FlushTris(const float *xy, int vertCount,
                              float r, float g, float b, float a) {
        if (vertCount < 3) return;
        glUseProgram(m_shaderProg);
        glUniformMatrix4fv(m_locProj, 1, GL_FALSE, m_proj);
        glUniform4f(m_locColour, r, g, b, a);
        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertCount * 2 * sizeof(float), xy);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDrawArrays(GL_TRIANGLE_FAN, 0, vertCount);
        glBindVertexArray(0);
        glUseProgram(0);
    }

    void DebugDraw::FlushPoints(const float *xy, int vertCount, float size,
                                float r, float g, float b, float a) {
        if (vertCount < 1) return;
        glUseProgram(m_shaderProg);
        glUniformMatrix4fv(m_locProj, 1, GL_FALSE, m_proj);
        glUniform4f(m_locColour, r, g, b, a);
        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertCount * 2 * sizeof(float), xy);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glPointSize(size);
        glDrawArrays(GL_POINTS, 0, vertCount);
        glPointSize(1.0f);
        glBindVertexArray(0);
        glUseProgram(0);
    }

    void DebugDraw::BuildCircleLines(float cx, float cy, float radius,
                                     float *out, int &outCount, int segments) {
        outCount = 0;
        for (int i = 0; i < segments; ++i) {
            float a = (2.0f * 3.14159265f * i) / segments;
            out[outCount++] = cx + cosf(a) * radius;
            out[outCount++] = cy + sinf(a) * radius;
        }
    }

    void DebugDraw::BuildCircleFan(float cx, float cy, float radius,
                                   float *out, int &outCount, int segments) {
        outCount = 0;
        out[outCount++] = cx;
        out[outCount++] = cy;
        for (int i = 0; i <= segments; ++i) {
            float a = (2.0f * 3.14159265f * i) / segments;
            out[outCount++] = cx + cosf(a) * radius;
            out[outCount++] = cy + sinf(a) * radius;
        }
    }

    bool DebugDraw::CompileShader(GLuint id, const char *src) {
        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);
        GLint ok = 0;
        glGetShaderiv(id, GL_COMPILE_STATUS, &ok);
        if (!ok) {
            char log[512];
            glGetShaderInfoLog(id, 512, nullptr, log);
            fprintf(stderr, "[DebugDraw] Shader error: %s\n", log);
        }
        return ok != 0;
    }

    void DebugDraw::MakeOrtho(float *m, float l, float r, float b, float t, float x, float y) {

        memset(m, 0, 16 * sizeof(float));
        m[0] = 2.0f / (r - l);
        m[5] = 2.0f / (t - b);
        m[10] = -1.0f;
        m[12] = (-(r + l) / (r - l)) - ((2.f/Config::GetInstance().windowsWidth) * x) + (abs(x) > 0);
        m[13] = (-(t + b) / (t - b)) + ((2.f/Config::GetInstance().windowsHeight) * y) - (abs(y) > 0);
        m[15] = 1.0f;
    }

    void DebugDraw::UnpackColor(b2HexColor hex, float &r, float &g, float &b) {
        r = ((hex >> 16) & 0xFF) / 255.0f;
        g = ((hex >> 8) & 0xFF) / 255.0f;
        b = ((hex) & 0xFF) / 255.0f;
    }

    b2Vec2 DebugDraw::TransformPt(b2Transform xf, b2Vec2 local) {
        return {
            xf.q.c * local.x - xf.q.s * local.y + xf.p.x,
            xf.q.s * local.x + xf.q.c * local.y + xf.p.y
        };
    }
}
