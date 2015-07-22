#ifndef BOARDOBJECT_H
#define BOARDOBJECT_H


class BoardObject
{
public:
    BoardObject();
    ~BoardObject();



private:
    const float m_positions[12] = { 1.0f, -1.0f, 0.0f,
                          1.0f,  1.0f, 0.0f,
                         -1.0f, -1.0f, 0.0f,
                         -1.0f,  1.0f, 0.0f};

    const float m_texcoords[8] = {1.0f, 0.0f,
                          1.0f, 1.0f,
                          0.0f, 0.0f,
                          0.0f, 1.0f};

    const unsigned int m_indices[4] = {0,1,2,3};

};

#endif // BOARDOBJECT_H
