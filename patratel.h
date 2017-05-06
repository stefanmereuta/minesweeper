#ifndef PATRATEL_H
#define PATRATEL_H

class Patratel
{

public:
    int m_X, m_Y, m_BombeInJur, m_ID, m_Grup;
    bool m_AreBomba, m_Apasat, m_Steag;
    static int newID;
    Patratel();

    Patratel(int x, int y);
    void setID();
    static void resetID();
};

#endif // PATRATEL_H
