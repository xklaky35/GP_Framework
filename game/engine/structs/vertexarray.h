#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

namespace Engine {
    class VertexArray {
        // Member methods:
    public:
        VertexArray(const float *pVertices, unsigned int numVertices,
                    const unsigned int *pIndices, unsigned int numIndicies);
        ~VertexArray();

        void SetActive() const;
        [[nodiscard]] unsigned int GetNumVertices() const;
        [[nodiscard]] unsigned int GetNumIndicies() const;

    protected:
        unsigned int m_numVertices;
        unsigned int m_numIndices;
        unsigned int m_glVertexBuffer;
        unsigned int m_glIndexBuffer;
        unsigned int m_glVertexArray;

    };
}
#endif // VERTEXARRAY_H