#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <cmath>

// Если используешь MKL или OpenBLAS:
extern "C" {
#include <cblas.h>
}

using namespace std;

// Размер матрицы
const int N = 2048;

// Генерация матрицы
void generate_matrix(vector<double>& A) {
    mt19937 gen(42);
    uniform_real_distribution<double> dist(0.0, 1.0);

    for (int i = 0; i < N * N; i++) {
        A[i] = dist(gen);
    }
}

// =======================
// 1. НАИВНОЕ УМНОЖЕНИЕ
// =======================
void multiply_naive(const vector<double>& A, const vector<double>& B, vector<double>& C) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            double sum = 0.0;
            for (int k = 0; k < N; k++) {
                sum += A[i*N + k] * B[k*N + j];
            }
            C[i*N + j] = sum;
        }
    }
}

// =======================
// 3. БЛОЧНОЕ УМНОЖЕНИЕ
// =======================
void multiply_blocked(const vector<double>& A, const vector<double>& B, vector<double>& C) {
    int block = 64;

    for (int ii = 0; ii < N; ii += block) {
        for (int jj = 0; jj < N; jj += block) {
            for (int kk = 0; kk < N; kk += block) {

                for (int i = ii; i < min(ii + block, N); i++) {
                    for (int j = jj; j < min(jj + block, N); j++) {

                        double sum = C[i*N + j];

                        for (int k = kk; k < min(kk + block, N); k++) {
                            sum += A[i*N + k] * B[k*N + j];
                        }

                        C[i*N + j] = sum;
                    }
                }
            }
        }
    }
}

// =======================
// ЗАМЕР ВРЕМЕНИ
// =======================
double measure(void (*func)(const vector<double>&, const vector<double>&, vector<double>&),
               const vector<double>& A,
               const vector<double>& B,
               vector<double>& C) {

    auto start = chrono::high_resolution_clock::now();

    func(A, B, C);

    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> diff = end - start;
    return diff.count();
}

// =======================
// MAIN
// =======================
int main() {
    vector<double> A(N*N), B(N*N), C(N*N, 0.0);

    generate_matrix(A);
    generate_matrix(B);

    double c = 2.0 * pow(N, 3);

    // 1. Наивный
    fill(C.begin(), C.end(), 0.0);
    double t1 = measure(multiply_naive, A, B, C);
    double p1 = (c / t1) * 1e-6;

    cout << "Naive:\n";
    cout << "Time: " << t1 << " sec\n";
    cout << "Perf: " << p1 << " MFLOPS\n\n";

    // 2. BLAS
    fill(C.begin(), C.end(), 0.0);
    auto start = chrono::high_resolution_clock::now();

    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                N, N, N,
                1.0, A.data(), N,
                B.data(), N,
                0.0, C.data(), N);

    auto end = chrono::high_resolution_clock::now();

    double t2 = chrono::duration<double>(end - start).count();
    double p2 = (c / t2) * 1e-6;

    cout << "BLAS:\n";
    cout << "Time: " << t2 << " sec\n";
    cout << "Perf: " << p2 << " MFLOPS\n\n";

    // 3. Оптимизированный
    fill(C.begin(), C.end(), 0.0);
    double t3 = measure(multiply_blocked, A, B, C);
    double p3 = (c / t3) * 1e-6;

    cout << "Blocked:\n";
    cout << "Time: " << t3 << " sec\n";
    cout << "Perf: " << p3 << " MFLOPS\n\n";

    return 0;
}