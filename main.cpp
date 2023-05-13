#include <iostream>

using namespace std;

class RubikCube {
    int faces[6][3][3];
    enum {
        green,
        red,
        blue,
        orange,
        yellow,
        white
    };

    void matrixRotation(int ind) { // Универсальный поворот матрицы по часовой
        swap(faces[ind][0][1], faces[ind][1][0]);
        swap(faces[ind][0][2], faces[ind][2][0]);
        swap(faces[ind][1][2], faces[ind][2][1]);

        swap(faces[ind][0][0], faces[ind][0][2]);
        swap(faces[ind][1][0], faces[ind][1][2]);
        swap(faces[ind][2][0], faces[ind][2][2]);
    }
public:
    explicit RubikCube() { // Цвета глянешь по пикчам, которые кинул
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 3; j++) {
                for (int k = 0; k < 3; k++) {
                    faces[i][j][k] = i;
                }
            }
        }
    }

    // Повороты пока все только по часовой (хз, нужны ли против)
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

    void print() { // Выводит в консоль развертку куба
        for (int i = 0; i < 3; i++) {
            cout << "         ";
            for (int j = 0; j < 3; j++) {
                cout << faces[yellow][i][j] << "  ";
            }
            cout << '\n';
        }

        for (int k = 0; k < 3; k++) {
            for (int i = -1; i < 3; i++) {
                cout << '|';
                for (int j = 0; j < 3; j++) {
                    if (i == -1)
                        cout << faces[orange][k][j];
                    else
                        cout << faces[i][k][j];
                    if (j != 2)
                        cout << "  ";
                }
                if (i == 2) cout << "|\n";
            }
        }
        for (int i = 0; i < 3; i++) {
            cout << "         ";
            for (int j = 0; j < 3; j++) {
                cout << faces[white][i][j] << "  ";
            }
            cout << '\n';
        }
    }
};

int main() {
    RubikCube cube;
    cube.print();

    return 0;
}
