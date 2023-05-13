#include <iostream>

using namespace std;

class RubikCube {
    int faces[6][3][3];

    void matrixRotation(int ind) {
        swap(faces[ind][0][1], faces[ind][1][0]);
        swap(faces[ind][0][2], faces[ind][2][0]);
        swap(faces[ind][1][2], faces[ind][2][1]);

        swap(faces[ind][0][0], faces[ind][0][2]);
        swap(faces[ind][1][0], faces[ind][1][2]);
        swap(faces[ind][2][0], faces[ind][2][2]);
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

    void frontRotation() {
        matrixRotation(0);
        int tmp[3];
        for (int i = 0; i < 3; i++) {
            tmp[i] = faces[1][i][0];
        }
        for (int i = 0; i < 3; i++) {
            swap(tmp[i], faces[5][0][2-i]);
        }
        for (int i = 0; i < 3; i++) {
            swap(tmp[i], faces[3][2-i][2]);
        }
        for (int i = 0; i < 3; i++) {
            swap(tmp[i], faces[4][2][i]);
        }
        for (int i = 0; i < 3; i++) {
            faces[1][i][0] = tmp[i];
        }
    }

    void print() {
        for (int i = 0; i < 3; i++) {
            cout << "      ";
            for (int j = 0; j < 3; j++) {
                cout << faces[4][i][j] << ' ';
            }
            cout << '\n';
        }

        for (int k = 0; k < 3; k++) {
            for (int i = -1; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (i == -1)
                        cout << faces[3][k][j] << ' ';
                    else
                        cout << faces[i][k][j] << ' ';
                }
                if (i == 2) cout << '\n';
            }
        }
        for (int i = 0; i < 3; i++) {
            cout << "      ";
            for (int j = 0; j < 3; j++) {
                cout << faces[5][i][j] << ' ';
            }
            cout << '\n';
        }
    }
};

int main() {
    RubikCube cube;
    cube.frontRotation();
    cube.frontRotation();
    cube.print();
    return 0;
}
