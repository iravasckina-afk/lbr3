#include <iostream>
#include <cblas.h>

using namespace std;

int main() {

    const int N = 2;

    double A[N*N] = {
        1,2,
        3,4
    };

    double B[N*N] = {
        5,6,
        7,8
    };

    double C[N*N] = {0};

    cblas_dgemm(
        CblasRowMajor,
        CblasNoTrans,
        CblasNoTrans,
        N,
        N,
        N,
        1.0,
        A,
        N,
        B,
        N,
        0.0,
        C,
        N
    );

    for(int i=0;i<N;i++) {

        for(int j=0;j<N;j++) {

            cout << C[i*N+j] << " ";
        }

        cout << endl;
    }

    return 0;
}