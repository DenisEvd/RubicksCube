#include <iostream>
#include <vector>
#include <functional>
#include <fstream>

using namespace std;

class RubikCube {
private:
    // faces [center, cordY (from top to bottom), cordX (from left to right)]
    int faces[6][3][3]{};
    // moves that solve

    //course of the decision
    const string outputRotations [12] = {"F", "F'", "B", "B'", "L", "L'", "R", "R'", "U", "U'", "D", "D'"};
    vector<int> rotationHistory;
    bool canWriteRotations = true;

    // colors
    const string outputColors [6] = {"G", "O", "B", "R", "Y", "W"};
    enum Colors {
        green,
        red,
        blue,
        orange,
        yellow,
        white
    };

    void matrixRotation(int ind) {
        swap(faces[ind][0][1], faces[ind][1][0]);
        swap(faces[ind][0][2], faces[ind][2][0]);
        swap(faces[ind][1][2], faces[ind][2][1]);

        swap(faces[ind][0][0], faces[ind][0][2]);
        swap(faces[ind][1][0], faces[ind][1][2]);
        swap(faces[ind][2][0], faces[ind][2][2]);
    }


    // utils
    static bool isEdge(int i, int j) {
        if (i == 1 && (j == 0 || j == 2)) {
            return true;
        }
        if (j == 1 && (i == 0 || i == 2)) {
            return true;
        }
        return false;
    }

    void toFirstTypeWhiteCross() {
        L();
        D();
        Lr();
        Dr();
        L();
        while (twoCenterConnected().size() < 2) {
            D();
        }
    }

    static bool checkTwoColors(int v1, int v2, int c1, int c2) {
        if ((v1 == c1 && v2 == c2) || (v1 == c2 && v2 == c1)) {
            return true;
        }
        return false;
    }

    // simple algo
    void greenPifPaf() {
        R();
        U();
        Rr();
        Ur();
    }
    void greenLeftPifPaf() {
        Lr();
        Ur();
        L();
        U();
    }

    void bluePifPaf() {
        L();
        U();
        Lr();
        Ur();
    }
    void blueLeftPifPaf() {
        Rr();
        Ur();
        R();
        U();
    }
    void redPifPaf() {
        B();
        U();
        Br();
        Ur();
    }
    void redLeftPifPaf() {
        Fr();
        Ur();
        F();
        U();
    }
    void orangePifPaf() {
        F();
        U();
        Fr();
        Ur();
    }
    void orangeLeftPifPaf() {
        Br();
        Ur();
        B();
        U();
    }

    void reversPifPaf() {
        L();
        D();
        Lr();
        Dr();
        L();
    }

    static int colorToInt(char color) {
        switch (color) {
            case 'G':
                return 0;
            case 'O':
                return 1;
            case 'B':
                return 2;
            case 'R':
                return 3;
            case 'Y':
                return 4;
            default:
                return 5;
        }
    }

    // funcs to solve white cross
    void downingWhiteEdges(const function<void()>& move, const function<void()>& put, int color) {
        int y = 1, x = 2;
        if (color == blue) {
            y = 1;
            x = 0;
        } else if (color == red) {
            y = 2;
            x = 1;
        } else if (color == orange) {
            y = 0;
            x = 1;
        }
        int countWhiteEdges = 0;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (faces[color][i][j] == white && isEdge(i, j)) {
                    countWhiteEdges++;
                }
            }
        }

        while (countWhiteEdges > 0) {
            for (int i = 0; i < 4; i++) {
                if (faces[color][1][2] != white) {
                    move();
                }
            }
            while (faces[white][y][x] == white) {
                D();
            }
            put();
            countWhiteEdges--;
        }
    }
    bool isWhiteEdgesDowned() {
        if (faces[white][1][0] == white && faces[white][1][2] == white && faces[white][0][1] == white && faces[white][2][1] == white) {
            return true;
        }
        return false;
    }
    vector<int> twoCenterConnected() {
        bool oCon = (faces[orange][2][1] == orange);
        bool gCon = (faces[green][2][1] == green);
        bool rCon = (faces[red][2][1] == red);
        bool bCon = (faces[blue][2][1] == blue);
        bool connected [] = {gCon, rCon, bCon, oCon};
        vector<int> res;
        for (int i = 0; i < 4; i++) {
            if (connected[i]) {
                res.push_back(i);
            }
        }
        return res;
    }

    // solving
    void solvingWhiteCross() {
        while (!isWhiteEdgesDowned()) {
            downingWhiteEdges([this]() {this->U();}, [this]() {this->R();this->R();}, yellow);
            downingWhiteEdges([this]() {this->F();}, [this]() {this->Rr();}, green);
            downingWhiteEdges([this]() {this->B();}, [this]() {this->Lr();}, blue);
            downingWhiteEdges([this]() {this->R();}, [this]() {this->Br();}, red);
            downingWhiteEdges([this]() {this->L();}, [this]() {this->Fr();}, orange);
        }

        while (twoCenterConnected().size() < 2) {
            D();
        }

        auto con = twoCenterConnected();

        if (twoCenterConnected().size() == 2) {
            // second type
            if (checkTwoColors(con[0], con[1], green, blue)) {
                toFirstTypeWhiteCross();
            }
            if (checkTwoColors(con[0], con[1], red, orange)) {
                D();
                toFirstTypeWhiteCross();
            }

            con = twoCenterConnected();
            // first type
            if (checkTwoColors(con[0], con[1], green, red)) {
                reversPifPaf();
            }
            if (checkTwoColors(con[0], con[1], green, orange)) {
               D();
               reversPifPaf();
               Dr();
            }
            if (checkTwoColors(con[0], con[1], orange, blue)) {
                D();
                D();
                reversPifPaf();
                Dr();
                Dr();
            }
            if (checkTwoColors(con[0], con[1], blue, red)) {
                Dr();
                reversPifPaf();
                D();
            }
        }
    }

    static int countTurns(int type, int color) {
        if (type == 1) { // pos 0, 0
            switch (color) {
                case red:
                    return 0;
                case blue:
                    return 1;
                case orange:
                    return 2;
                case green:
                    return 3;
            }
        } else {
            switch (color) {
                case red:
                    return 1;
                case blue:
                    return 2;
                case orange:
                    return 3;
                case green:
                    return 0;
            }
        }
        return -1;
    }

    void putWhiteCorner() {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (faces[yellow][i][j] == white) {
                    while (faces[yellow][2][2] != white) {
                        U();
                    }
                    int color1 = faces[green][0][2];
                    int color2 = faces[red][0][0];
                    if (checkTwoColors(color1, color2, green, orange)) {
                        U();
                        orangePifPaf();
                        orangePifPaf();
                        orangePifPaf();
                    }
                    if (checkTwoColors(color1, color2, green, red)) {
                        greenPifPaf();
                        greenPifPaf();
                        greenPifPaf();
                    }
                    if (checkTwoColors(color1, color2, red, blue)) {
                        Ur();
                        redPifPaf();
                        redPifPaf();
                        redPifPaf();
                    }
                    if (checkTwoColors(color1, color2, orange, blue)) {
                        U();
                        U();
                        bluePifPaf();
                        bluePifPaf();
                        bluePifPaf();
                    }
                }
            }
        }
    }
    void putUpWhiteCorner() {
        if (faces[green][2][0] == white) {
            orangePifPaf();
            orangePifPaf();
            orangePifPaf();
            orangePifPaf();
            orangePifPaf();
        } else if (faces[green][2][2] == white) {
            greenPifPaf();
        } else if (faces[red][2][0] == white) {
            greenPifPaf();
            greenPifPaf();
            greenPifPaf();
            greenPifPaf();
            greenPifPaf();
        } else if (faces[red][2][2] == white) {
            redPifPaf();
        } else if (faces[blue][2][0] == white) {
            redPifPaf();
            redPifPaf();
            redPifPaf();
            redPifPaf();
            redPifPaf();
        } else if (faces[blue][2][2] == white) {
            bluePifPaf();
        } else if (faces[orange][2][0] == white) {
            bluePifPaf();
            bluePifPaf();
            bluePifPaf();
            bluePifPaf();
            bluePifPaf();
        } else if (faces[orange][2][2] == white) {
            orangePifPaf();
        }
    }
    void putWhiteCornerFromThirdLayer() {
        for (int colorSide = 0; colorSide < 5; colorSide++) {
            if (faces[colorSide][0][0] == white) {
                for (int steps = 0; steps < countTurns(1, colorSide); steps++) {
                    U();
                }
                int c1 = faces[yellow][2][2];
                int c2 = faces[green][0][2];
                if (checkTwoColors(c1, c2, green, orange)) {
                    U();
                    L();
                    Fr();
                    Lr();
                    F();
                } else if (checkTwoColors(c1, c2, red, green)) {
                    F();
                    Rr();
                    Fr();
                    R();
                } else if (checkTwoColors(c1, c2, blue, orange)) {
                    U();
                    U();
                    B();
                    Lr();
                    Br();
                    L();
                } else if (checkTwoColors(c1, c2, red, blue)) {
                    Ur();
                    R();
                    Br();
                    Rr();
                    B();
                }

            }
            if (faces[colorSide][0][2] == white) {
                for (int steps = 0; steps < countTurns(2, colorSide); steps++) {
                    U();
                }
                int c1 = faces[yellow][2][2];
                int c2 = faces[red][0][0];
                if (checkTwoColors(c1, c2, green, orange)) {
                    U();
                    Fr();
                    L();
                    F();
                    Lr();
                } else if (checkTwoColors(c1, c2, green, red)) {
                    Rr();
                    F();
                    R();
                    Fr();
                } else if (checkTwoColors(c1, c2, blue, orange)) {
                    U();
                    U();
                    Lr();
                    B();
                    L();
                    Br();
                } else if (checkTwoColors(c1, c2, red, blue)) {
                    Ur();
                    Br();
                    R();
                    B();
                    Rr();
                }
            }
        }
    }
    void putUpWrongWhiteCorner() {
        if (!(faces[white][0][0] == white && faces[orange][2][2] == orange && faces[green][2][0] == green)) {
            orangePifPaf();
        } else if (!(faces[white][0][2] == white && faces[green][2][2] == green && faces[red][2][0] == red)) {
            greenPifPaf();
        } else if (!(faces[white][2][2] == white && faces[red][2][2] == red && faces[blue][2][0] == blue)) {
            redPifPaf();
        } else if (!(faces[white][2][0] == white && faces[blue][2][2] == blue || faces[orange][2][0] == orange)) {
            bluePifPaf();
        }
    }
    bool firstLayerSolved() {
        if ((faces[white][0][0] == white) && (faces[white][0][2]) == white && (faces[white][2][0] == white) && faces[white][2][2] == white) {
            if ((faces[green][2][0] == green) && (faces[green][2][2] == green) && (faces[red][2][0] == red) && (faces[red][2][2] == red) && (faces[blue][2][0] == blue) && (faces[blue][2][2] == blue) && (faces[orange][2][0] == orange) && (faces[orange][2][2] == orange)) {
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }

    void solvingFirstLayer() {
        while (!firstLayerSolved()) {
            putUpWrongWhiteCorner();
            putUpWhiteCorner();
            putWhiteCorner();
            putWhiteCornerFromThirdLayer();
        }
    }

    void downEdgeToSecondLayer() {
        for (int i = 0; i < 4; i++) {
            //find for green
            if ((faces[yellow][0][1] == orange || faces[yellow][0][1] == red) && faces[blue][0][1] == green) {
                U();
                U();
            } else if ((faces[yellow][1][2] == orange || faces[yellow][1][2] == red) && faces[red][0][1] == green) {
                U();
            } else if ((faces[yellow][1][0] == orange || faces[yellow][1][0] == red) && faces[orange][0][1] == green) {
                Ur();
            }
            if (faces[yellow][2][1] == orange && faces[green][0][1] == green) {
                Ur();
                greenLeftPifPaf();
                orangePifPaf();
            } else if (faces[yellow][2][1] == red && faces[green][0][1] == green) {
                U();
                greenPifPaf();
                redLeftPifPaf();
            }

            //find for red
            if ((faces[yellow][1][0] == green || faces[yellow][1][0] == blue) && faces[orange][0][1] == red) {
                U();
                U();
            } else if ((faces[yellow][0][1] == green || faces[yellow][0][1] == blue) && faces[blue][0][1] == red) {
                U();
            } else if ((faces[yellow][2][1] == green || faces[yellow][2][1] == blue) && faces[green][0][1] == red) {
                Ur();
            }
            if (faces[yellow][1][2] == green && faces[red][0][1] == red) {
                Ur();
                redLeftPifPaf();
                greenPifPaf();
            } else if (faces[yellow][1][2] == blue && faces[red][0][1] == red) {
                U();
                redPifPaf();
                blueLeftPifPaf();
            }

            //find for blue
            if ((faces[yellow][2][1] == red || faces[yellow][2][1] == orange) && faces[green][0][1] == blue) {
                U();
                U();
            } else if ((faces[yellow][1][0] == red || faces[yellow][1][0] == orange) && faces[orange][0][1] == blue) {
                U();
            } else if ((faces[yellow][1][2] == red || faces[yellow][1][2] == orange) && faces[red][0][1] == blue) {
                Ur();
            }
            if (faces[yellow][0][1] == red && faces[blue][0][1] == blue) {
                Ur();
                blueLeftPifPaf();
                redPifPaf();
            } else if (faces[yellow][0][1] == orange && faces[blue][0][1] == blue) {
                U();
                bluePifPaf();
                orangeLeftPifPaf();
            }

            // find for orange
            if ((faces[yellow][1][2] == blue || faces[yellow][1][2] == green) && faces[red][0][1] == orange) {
                U();
                U();
            } else if ((faces[yellow][2][1] == blue || faces[yellow][2][1] == green) && faces[green][0][1] == orange) {
                U();
            } else if ((faces[yellow][0][1] == blue || faces[yellow][0][1] == green) && faces[blue][0][1] == orange) {
                Ur();
            }
            if (faces[yellow][1][0] == blue && faces[orange][0][1] == orange) {
                Ur();
                orangeLeftPifPaf();
                bluePifPaf();
            } else if (faces[yellow][1][0] == green && faces[orange][0][1] == orange) {
                U();
                orangePifPaf();
                greenLeftPifPaf();
            }
        }
    }

    void upWrongEdgeOnSecondLayer() {
        if (edgeNotExistYellow(faces[green][1][2], faces[red][1][0]) && (faces[green][1][2] != green || faces[red][1][0] != red)) {
            U();
            greenPifPaf();
            redLeftPifPaf();
        }
        if (edgeNotExistYellow(faces[red][1][2], faces[blue][1][0]) && (faces[red][1][2] != red || faces[blue][1][0] != blue)) {
            U();
            redPifPaf();
            blueLeftPifPaf();
        }
        if (edgeNotExistYellow(faces[blue][1][2], faces[orange][1][0]) && (faces[blue][1][2] != blue || faces[orange][1][0] != orange)) {
            U();
            bluePifPaf();
            orangeLeftPifPaf();
        }
        if (edgeNotExistYellow(faces[orange][1][2], faces[green][1][0]) && (faces[orange][1][2] != orange || faces[green][1][0] != green)) {
            U();
            orangePifPaf();
            greenLeftPifPaf();
        }
    }

    bool secondLayerIsSolved() {
        return faces[green][1][2] == green && faces[red][1][0] == red && faces[red][1][2] == red && faces[blue][1][0] == blue
               && faces[blue][1][2] == blue && faces[orange][1][0] == orange && faces[orange][1][2] == orange && faces[green][1][0] == green;
    }

    static bool edgeNotExistYellow(int c1, int c2) {
        return c1 != yellow && c2 != yellow;
    }

    void solvingSecondLayer() {
        while (!secondLayerIsSolved()) {
            downEdgeToSecondLayer();
            upWrongEdgeOnSecondLayer();
        }
    }

    bool isYellowCrossSolved() {
        if (faces[yellow][0][1] == yellow && faces[yellow][1][0] == yellow && faces[yellow][2][1] == yellow && faces[yellow][1][2] == yellow) {
            return true;
        } else {
            return false;
        }
    }

    vector<int> twoCenterConnectedYellowSide() {
        bool oCon = (faces[orange][0][1] == orange);
        bool gCon = (faces[green][0][1] == green);
        bool rCon = (faces[red][0][1] == red);
        bool bCon = (faces[blue][0][1] == blue);
        bool connected [] = {gCon, rCon, bCon, oCon};
        vector<int> res;
        for (int i = 0; i < 4; i++) {
            if (connected[i]) {
                res.push_back(i);
            }
        }
        return res;
    }

    void yellowCrossPifPaf() {
        R();
        U();
        U();
        Rr();
        Ur();
        R();
        Ur();
        Rr();
    }

    void solvingYellowCross() {
        while (!isYellowCrossSolved()) {
            F();
            greenPifPaf();
            Fr();
            if (faces[yellow][2][1] == yellow && faces[yellow][1][2] == yellow) {
                U();
                U();
            }
            if (faces[yellow][1][0] == yellow && faces[yellow][2][1] == yellow) {
                U();
            }
            if (faces[yellow][0][1] == yellow && faces[yellow][1][2] == yellow) {
                Ur();
            }
            if (faces[yellow][0][1] == yellow && faces[yellow][1][2] == yellow) {
                U();
            }
        }
        while (twoCenterConnectedYellowSide().size() < 2) {
            U();
        }
        auto conn2 = twoCenterConnectedYellowSide();

        if (twoCenterConnectedYellowSide().size() == 2) {
            // second type
            if (checkTwoColors(conn2[0], conn2[1], green, blue)) {
                yellowCrossPifPaf();
            }
            if (checkTwoColors(conn2[0], conn2[1], red, orange)) {
                yellowCrossPifPaf();
            }
            while (twoCenterConnectedYellowSide().size() < 2) {
                U();
            }
            auto conn = twoCenterConnectedYellowSide();
            // first type
            if (checkTwoColors(conn[0], conn[1], green, red)) {
                U();
                U();
                yellowCrossPifPaf();
            }

            if (checkTwoColors(conn[0], conn[1], green, orange)) {
                U();
                yellowCrossPifPaf();
            }

            if (checkTwoColors(conn[0], conn[1], orange, blue)) {
                yellowCrossPifPaf();
            }

            if (checkTwoColors(conn[0], conn[1], blue, red)) {
                Ur();
                yellowCrossPifPaf();
            }
        }
        while (twoCenterConnectedYellowSide().size() != 4) {
            U();
        }
    }

    void reversePifPafGreen() {
        L();
        D();
        Lr();
        Dr();
    }

    bool isYellowCornersSolved() {
        if (faces[yellow][0][0] == yellow && faces[yellow][0][2] == yellow && faces[yellow][2][0] == yellow && faces[yellow][2][2] == yellow) {
            return true;
        }
        return false;
    }

    void reverseYellowCorners() {
        while (!isYellowCornersSolved()) {
            while (faces[yellow][2][0] != yellow) {
                reversePifPafGreen();
            }
            if (isYellowCornersSolved()) {
                break;
            }
            while (faces[yellow][2][0] == yellow) {
                Ur();
            }
        }
        while (twoCenterConnectedYellowSide().size() != 4) {
            U();
        }
    }

    bool yellowCornersOnRightPlaces() {
        return faces[green][0][2] == green && faces[yellow][2][2] == yellow && faces[red][0][0] == red && faces[red][0][2] == red && faces[yellow][0][2] == yellow && faces[blue][0][0] == blue &&
               faces[blue][0][2] == blue && faces[yellow][0][0] == yellow && faces[orange][0][0] == orange && faces[orange][0][2] == orange && faces[yellow][2][0] == yellow && faces[green][0][0] == green;
    }

    void permutationYellowCorners() {
        while (!yellowCornersOnRightPlaces()) {
            if (faces[green][0][2] == green && faces[yellow][2][2] == yellow && faces[red][0][0] == red) {
                L(); L(); D(); L(); L(); Dr(); B(); B(); L(); L(); Ur(); L(); L(); U(); B(); B();
            } else if (faces[red][0][2] == red && faces[yellow][0][2] == yellow && faces[blue][0][0] == blue) {
                F(); F(); D(); F(); F(); Dr(); L(); L(); F(); F(); Ur(); F(); F(); U(); L(); L();
            } else if (faces[blue][0][2] == blue && faces[yellow][0][0] == yellow && faces[orange][0][0] == orange) {
                R(); R(); D(); R(); R(); Dr(); F(); F(); R(); R(); Ur(); R(); R(); U(); F(); F();
            } else {
                B(); B(); D(); B(); B(); Dr(); R(); R(); B(); B(); Ur(); B(); B(); U(); R(); R();
            }
        }
    }


public:
    explicit RubikCube() {
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 3; j++) {
                for (int k = 0; k < 3; k++) {
                    faces[i][j][k] = i;
                }
            }
        }
    }

    void F() {
        matrixRotation(green);
        int tmp[3];
        for (int i = 0; i < 3; i++) {
            tmp[i] = faces[red][i][0];
        }
        for (int i = 0; i < 3; i++) {
            swap(tmp[i], faces[white][0][2-i]);
        }
        for (int i = 0; i < 3; i++) {
            swap(tmp[i], faces[orange][2-i][2]);
        }
        for (int i = 0; i < 3; i++) {
            swap(tmp[i], faces[yellow][2][i]);
        }
        for (int i = 0; i < 3; i++) {
            faces[red][i][0] = tmp[i];
        }
        if (canWriteRotations)
            rotationHistory.push_back(0);
    }
    void Fr() {
        canWriteRotations = false;
        for (int i = 0; i < 3; i++) F();
        canWriteRotations = true;
        rotationHistory.push_back(1);

    }

    void B() {
        matrixRotation(blue);
        int tmp[3];
        for (int i = 0; i < 3; i++) {
            tmp[i] = faces[orange][i][0];
        }
        for (int i = 0; i < 3; i++) {
            swap(tmp[i], faces[white][2][i]);
        }
        for (int i = 0; i < 3; i++) {
            swap(tmp[i], faces[red][2-i][2]);
        }
        for (int i = 0; i < 3; i++) {
            swap(tmp[i], faces[yellow][0][2-i]);
        }
        for (int i = 0; i < 3; i++) {
            faces[orange][i][0] = tmp[i];
        }
        if (canWriteRotations)
            rotationHistory.push_back(2);
    }
    void Br() {
        canWriteRotations = false;
        for (int i = 0; i < 3; i++) B();
        canWriteRotations = true;
        rotationHistory.push_back(3);
    }

    void L() {
        matrixRotation(orange);
        int tmp[3];
        for (int i = 0; i < 3; i++) {
            tmp[i] = faces[green][i][0];
        }
        for (int i = 0; i < 3; i++) {
            swap(tmp[i], faces[white][i][0]);
        }
        for (int i = 0; i < 3; i++) {
            swap(tmp[i], faces[blue][2-i][2]);
        }
        for (int i = 0; i < 3; i++) {
            swap(tmp[i], faces[yellow][i][0]);
        }
        for (int i = 0; i < 3; i++) {
            faces[green][i][0] = tmp[i];
        }
        if (canWriteRotations)
            rotationHistory.push_back(4);
    }
    void Lr() {
        canWriteRotations = false;
        for (int i = 0; i < 3; i++) L();
        canWriteRotations = true;
        rotationHistory.push_back(5);
    }

    void R() {
        matrixRotation(red);
        int tmp[3];
        for (int i = 0; i < 3; i++) {
            tmp[i] = faces[blue][i][0];
        }
        for (int i = 0; i < 3; i++) {
            swap(tmp[i], faces[white][2-i][2]);
        }
        for (int i = 0; i < 3; i++) {
            swap(tmp[i], faces[green][2-i][2]);
        }
        for (int i = 0; i < 3; i++) {
            swap(tmp[i], faces[yellow][2-i][2]);
        }
        for (int i = 0; i < 3; i++) {
            faces[blue][i][0] = tmp[i];
        }
        if (canWriteRotations)
            rotationHistory.push_back(6);
    }
    void Rr() {
        canWriteRotations = false;
        for (int i = 0; i < 3; i++) R();
        canWriteRotations = true;
        rotationHistory.push_back(7);
    }

    void U() {
        matrixRotation(yellow);
        int tmp[3];
        for (int i = 0; i < 3; i++) {
            tmp[i] = faces[red][0][2-i];
        }
        for (int i = 0; i < 3; i++) {
            swap(tmp[i], faces[green][0][2-i]);
        }
        for (int i = 0; i < 3; i++) {
            swap(tmp[i], faces[orange][0][2-i]);
        }
        for (int i = 0; i < 3; i++) {
            swap(tmp[i], faces[blue][0][2-i]);
        }
        for (int i = 0; i < 3; i++) {
            faces[red][0][2-i] = tmp[i];
        }
        if (canWriteRotations)
            rotationHistory.push_back(8);
    }
    void Ur() {
        canWriteRotations = false;
        for (int i = 0; i < 3; i++) U();
        canWriteRotations = true;
        rotationHistory.push_back(9);
    }

    void D() {
        matrixRotation(white);
        int tmp[3];
        for (int i = 0; i < 3; i++) {
            tmp[i] = faces[green][2][i];
        }
        for (int i = 0; i < 3; i++) {
            swap(tmp[i], faces[red][2][i]);
        }
        for (int i = 0; i < 3; i++) {
            swap(tmp[i], faces[blue][2][i]);
        }
        for (int i = 0; i < 3; i++) {
            swap(tmp[i], faces[orange][2][i]);
        }
        for (int i = 0; i < 3; i++) {
            faces[green][2][i] = tmp[i];
        }
        if (canWriteRotations)
            rotationHistory.push_back(10);
    }
    void Dr() {
        canWriteRotations = false;
        for (int i = 0; i < 3; i++) D();
        canWriteRotations = true;
        rotationHistory.push_back(11);
    }

    void solve() {
        solvingWhiteCross();
        solvingFirstLayer();
        solvingSecondLayer();
        solvingYellowCross();
        reverseYellowCorners();
        permutationYellowCorners();
    }

    void randomShuffle() {
        rotationHistory.clear();
        for (int i = 0; i < 50; i++) {
            int random = abs(rand() % 6);
            if (random == 0) R();
            if (random == 1) L();
            if (random == 2) F();
            if (random == 3) B();
            if (random == 4) U();
            if (random == 5) D();
        }
    }

    void printCube() { // output unwrapped cube in console
        for (int i = 0; i < 3; i++) {
            cout << "         ";
            for (int j = 0; j < 3; j++) {
                cout << outputColors[faces[yellow][i][j]] << "  ";
            }
            cout << '\n';
        }

        for (int k = 0; k < 3; k++) {
            for (int i = -1; i < 3; i++) {
                cout << '|';
                for (int j = 0; j < 3; j++) {
                    if (i == -1)
                        cout << outputColors[faces[orange][k][j]];
                    else
                        cout << outputColors[faces[i][k][j]];
                    if (j != 2)
                        cout << "  ";
                }
                if (i == 2) cout << "|\n";
            }
        }
        for (int i = 0; i < 3; i++) {
            cout << "         ";
            for (int j = 0; j < 3; j++) {
                cout << outputColors[faces[white][i][j]] << "  ";
            }
            cout << '\n';
        }
        cout << '\n';
    }

    void printRotations() {
        for (int i = 0; i < rotationHistory.size(); i++) {
            if (i % 10 == 0)
                cout << '\n';
            cout << outputRotations[rotationHistory[i]] << ' ';
        }
        cout << '\n';
    }

    void readFromFile(const string& fileName) {
        ifstream file;
        file.open(fileName);
        std::string line;
        if (file.is_open()) {
            int color = 0;
            int row = -1;
            while (std::getline(file, line)) {
                if (line == "---") {
                    continue;
                } else {
                    row += 1;
                    for (int i = 0; i < line.length(); i++) {
                        faces[color][row][i] = colorToInt(line[i]);
                    }
                    if (row == 2) {
                        row = -1;
                        color++;
                    }
                }

            }
        }
    }

    void writeInFile(const string& filename) {
        ifstream file;
        file.open(filename);
    }
};

int main() {
    RubikCube cube;
    srand(time(nullptr));
    cube.readFromFile("input.txt");
    cube.printCube();
//    cube.randomShuffle();
//    cube.solve();
//    cube.printRotations();

    return 0;
}
