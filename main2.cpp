#include <cblas.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <random>

using namespace std
using namespace chrono;

const int N = 512; // ⚠️ для теста поставь 512, потом 2048

vector<vector<double>> A(N, vector<double>(N));
vector<vector<double>> B(N, vector<double>(N));
vector<vector<double>> C(N, vector<double>(N, 0));

// генерация случайных матриц
void generate() {
    mt19937 gen(42);
    uniform_real_distribution<double> dist(0.0, 1.0);

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            A[i][j] = dist(gen);
            B[i][j] = dist(gen);
        }
}

// 1️⃣ обычное умножение
void multiply_simple() {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            for (int k = 0; k < N; k++)
                C[i][j] += A[i][k] * B[k][j];
}

// 3️⃣ оптимизированное (перестановка циклов)
void multiply_optimized() {
    for (int i = 0; i < N; i++)
        for (int k = 0; k < N; k++) {
            double temp = A[i][k];
            for (int j = 0; j < N; j++) {
                C[i][j] += temp * B[k][j];
            }
        }
}

int main() {
    generate();

    // ---------- 1 вариант ----------
    auto start = high_resolution_clock::now();
    multiply_simple();
    auto end = high_resolution_clock::now();

    double t1 = duration<double>(end - start).count();

    double c = 2.0 * N * N * N;
    double p1 = c / t1 * 1e-6;

    cout << "Simple time: " << t1 << " sec\n";
    cout << "Simple MFlops: " << p1 << endl;

    // очистка
    for (auto& row : C)
        fill(row.begin(), row.end(), 0);

    // ---------- 3 вариант ----------
    start = high_resolution_clock::now();
    multiply_optimized();
    end = high_resolution_clock::now();

    double t3 = duration<double>(end - start).count();
    double p3 = c / t3 * 1e-6;

    cout << "Optimized time: " << t3 << " sec\n";
    cout << "Optimized MFlops: " << p3 << endl;

    return 0;
}