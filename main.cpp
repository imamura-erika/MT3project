#include <Novice.h>
#include <Matrix4x4.h>
#include <Vector3.h>
#include <cassert>

const char kWindowTitle[] = "GC2B_03_イマムラ_エリカ_MT_00_03_確認課題";

// 1. 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
    Matrix4x4 matrix = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        translate.x, translate.y, translate.z, 1.0f
    };
    return matrix;
}

// 2. 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
    Matrix4x4 matrix = {
        scale.x, 0.0f, 0.0f, 0.0f,
        0.0f, scale.y, 0.0f, 0.0f,
        0.0f, 0.0f, scale.z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    return matrix;
}

// 3. 座標変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
    Vector3 result;
    result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
    result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
    result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
    float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
    assert(w != 0.0f);
    
    result.x /= w;
    result.y /= w;
    result.z /= w;
    return result;
}

// 数値表示用 Matrix
static const int kRowHeight = 20;
static const int kColumnWidth = 60;
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label) {
    // 文字列の表示
    Novice::ScreenPrintf(x, y - kRowHeight, "%s", label);

    // 行列の要素を表示
    for (int row = 0; row < 4; ++row) {
        for (int column = 0; column < 4; ++column) {
            Novice::ScreenPrintf(
                x + column * kColumnWidth, y + row * kRowHeight, "%6.02f", matrix.m[row][column]);
        }
    }
}

// 数値表示用 Vector
static const int kVectorRowHeight = 20;
void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label) {
    Novice::ScreenPrintf(x, y, "%.2f %.2f %.2f %s", vector.x, vector.y, vector.z, label);
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    // ライブラリの初期化
    Novice::Initialize(kWindowTitle, 1280, 720);

    // キー入力結果を受け取る箱
    char keys[256] = { 0 };
    char preKeys[256] = { 0 };

    // ウィンドウの×ボタンが押されるまでループ
    while (Novice::ProcessMessage() == 0) {
        // フレームの開始
        Novice::BeginFrame();

        // キー入力を受け取る
        memcpy(preKeys, keys, 256);
        Novice::GetHitKeyStateAll(keys);

        ///
        /// ↓更新処理ここから
        ///

        // 1. 平行移動行列
        Vector3 translate{ 4.1f, 2.6f, 0.8f };
        // 2. 拡大縮小行列
        Vector3 scale{ 1.5f, 5.2f, 7.3f };
        // 3. 座標変換
        Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);
        Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);
        Vector3 point{ 2.3f, 3.8f, 1.4f };
        Matrix4x4 transformMatrix = {
            1.0f, 2.0f, 3.0f, 4.0f,
            3.0f, 1.0f, 1.0f, 2.0f,
            1.0f, 4.0f, 2.0f, 3.0f,
            2.0f, 2.0f, 1.0f, 3.0f
        };
        Vector3 transformed = Transform(point, transformMatrix);

        ///
        /// ↑更新処理ここまで
        ///

        ///
        /// ↓描画処理ここから
        ///

        // 数値表示
        VectorScreenPrintf(0, 0, transformed, "transformed"); // 座標変換
        MatrixScreenPrintf(0, kRowHeight * 2, translateMatrix, "translateMatrix"); // 平行移動行列
        MatrixScreenPrintf(0, kRowHeight * 7, scaleMatrix, "scaleMatrix"); // 拡大縮小行列

        ///
        /// ↑描画処理ここまで
        ///

        // フレームの終了
        Novice::EndFrame();

        // ESCキーが押されたらループを抜ける
        if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
            break;
        }
    }

    // ライブラリの終了
    Novice::Finalize();
    return 0;
}
