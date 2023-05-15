#include <iostream>

using namespace std;

class RubikCube {
private:
    // faces [center, cordY (from top to bottom), cordX (from left to right)]
    int faces[6][3][3]{};

    // colors
    string outputColors [6] = {"G", "R", "B", "O", "Y", "W"};
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

    bool checkTwoColors(int v1, int v2, int c1, int c2) {
        if ((v1 == c1 && v2 == c2) || (v1 == c2 && v2 == c1)) {
            return true;
        }
        return false;
    }

    // simple algos
    void greenPifPaf() {
        R();
        U();
        Rr();
        Ur();
    }
    void bluePifPaf() {
        L();
        Ur();
        Lr();
        U();
    }
    void redPifPaf() {
        B();
        U();
        Br();
        Ur();
    }
    void orangePifPaf() {
        F();
        U();
        Fr();
        Ur();
    }
    void whitePifPaf() {
        R();
        F();
        Rr();
        Fr();
    }
    void yellowPifPaf() {
        R();
        B();
        Rr();
        Br();
    }

    void reversPifPaf() {
        L();
        D();
        Lr();
        Dr();
        L();
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
    }
    void Fr() {
        for (int i = 0; i < 3; i++) F();
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
    }
    void Br() {
        for (int i = 0; i < 3; i++) B();
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
    }
    void Lr() {
        for (int i = 0; i < 3; i++) L();
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
    }
    void Rr() {
        for (int i = 0; i < 3; i++) R();
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
    }
    void Ur() {
        for (int i = 0; i < 3; i++) U();
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
    }
    void Dr() {
        for (int i = 0; i < 3; i++) D();
    }

    void solve() {
        solvingWhiteCross();
        //TODO: solving white cross
        //TODO: solving white layer
        //TODO: solving second layer
        //TODO: solving yellow cross
        //TODO: placing one yellow corner
        //TODO: placing three others corners
        //TODO: flipping yellow corners
    }

    void randomShuffle() {
        for (int i = 0; i < 500; i++) {
            int random = abs(rand() % 12);
            if (random == 0) R();
            if (random == 1) Rr();
            if (random == 2) L();
            if (random == 3) Lr();
            if (random == 4) F();
            if (random == 5) Fr();
            if (random == 6) B();
            if (random == 7) Br();
            if (random == 8) U();
            if (random == 9) Ur();
            if (random == 10) D();
            if (random == 11) Dr();
        }
    }

    void print() { // Выводит в консоль развертку куба
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
    }
};

int main() {
    RubikCube cube;

    srand(time(NULL));
    cube.randomShuffle();
    cube.solve();
    cube.print();

    return 0;
}
