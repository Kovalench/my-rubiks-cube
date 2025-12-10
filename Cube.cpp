#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <map>
#include <random>
#define DEG2RAD (PI / 180.0f)

using namespace std;

static const Color BASE_COLORS[] = {
    WHITE,
    YELLOW,
    BLUE,
    GREEN,
    RED,
    ORANGE
};

void DrawMiniCompass(const Camera3D& camera, int x, int y, int size) {
    Vector2 center = { (float)x + size / 2.0f, (float)y + size / 2.0f };
    float radius = size / 2.0f - size * 0.08f;

    Vector3 forward = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
    float angle = atan2f(forward.x, forward.z);

    DrawCircleLines((int)center.x, (int)center.y, radius, WHITE);

    Vector2 northTip = {
        center.x + sinf(angle) * radius,
        center.y - cosf(angle) * radius
    };

    DrawLineEx(center, northTip, size * 0.05f, RED);

    int fontSize = (int)(size * 0.15f);
    int offset = (int)(size * 0.05f);

    DrawText("N",
        (int)(center.x - MeasureText("N", fontSize) / 2),
        (int)(y - offset - 2),
        fontSize, RED);

    DrawText("S",
        (int)(center.x - MeasureText("S", fontSize) / 2),
        (int)(y + size - fontSize + offset + 4),
        fontSize, WHITE);

    DrawText("W",
        (int)(x - fontSize + offset),
        (int)(center.y - fontSize / 2),
        fontSize, WHITE);

    DrawText("E",
        (int)(x + size - fontSize + offset + 14),
        (int)(center.y - fontSize / 2),
        fontSize, WHITE);
}

struct SmallCube {
    Vector3 position;
    map<int, Color> faces;

    void RotateFacesAroundAxis(char axis, bool clockwise) {
        map<int, int> rotationMap;

        if (axis == 'x') {
            rotationMap = clockwise
                ? map<int, int>{ {0, 4}, { 4, 1 }, { 1, 5 }, { 5, 0 } }
            : map<int, int>{ {0, 5}, {5, 1}, {1, 4}, {4, 0} };
        }
        else if (axis == 'y') {
            rotationMap = clockwise
                ? map<int, int>{ {2, 5}, { 5, 3 }, { 3, 4 }, { 4, 2 } }
            : map<int, int>{ {2, 4}, {4, 3}, {3, 5}, {5, 2} };
        }
        else if (axis == 'z') {
            rotationMap = clockwise
                ? map<int, int>{ {0, 3}, { 3, 1 }, { 1, 2 }, { 2, 0 } }
            : map<int, int>{ {0, 2}, {2, 1}, {1, 3}, {3, 0} };
        }

        map<int, Color> newFaces = faces;

        for (const auto& pair : rotationMap) {
            int from = pair.first;
            int to = pair.second;
            if (faces.count(from)) {
                newFaces[to] = faces[from];
            }
            else {
                newFaces.erase(to);
            }
        }

        faces = newFaces;
    }
};

class RubiksCube {
private:
    vector<SmallCube> cubes;

public:
    RubiksCube() {
        int colorIndex = 0;

        for (int x = -1; x <= 1; x++) {
            for (int y = -1; y <= 1; y++) {
                for (int z = -1; z <= 1; z++) {
                    SmallCube cube;
                    cube.position = { (float)x, (float)y, (float)z };

                    if (y == 1) cube.faces[0] = BASE_COLORS[0];
                    if (y == -1) cube.faces[1] = BASE_COLORS[1];
                    if (x == 1) cube.faces[2] = BASE_COLORS[2];
                    if (x == -1) cube.faces[3] = BASE_COLORS[3];
                    if (z == 1) cube.faces[4] = BASE_COLORS[4];
                    if (z == -1) cube.faces[5] = BASE_COLORS[5];

                    cubes.push_back(cube);
                }
            }
        }

        vector<char> faces = { 'L', 'R', 'U', 'D', 'F', 'B' };
        random_device rd;
        mt19937 g(rd());

        for (int i = 0; i < 500; i++) {
            char face = faces[g() % faces.size()];
            bool clockwise = g() % 2;
            RotateFace(face, clockwise);
        }
    }

    void DrawCubeFace(Vector3 position, float size, int faceDir, Color color) {
        Vector3 faceSize;

        switch (faceDir) {
        case 0:
            faceSize = { size, 0.01f, size };
            position.y += 0.5f;
            break;
        case 1:
            faceSize = { size, 0.01f, size };
            position.y -= 0.5f;
            break;
        case 2:
            faceSize = { 0.01f, size, size };
            position.x += 0.5f;
            break;
        case 3:
            faceSize = { 0.01f, size, size };
            position.x -= 0.5f;
            break;
        case 4:
            faceSize = { size, size, 0.01f };
            position.z += 0.5f;
            break;
        case 5:
            faceSize = { size, size, 0.01f };
            position.z -= 0.5f;
            break;
        }

        DrawCubeV(position, faceSize, color);
    }

    void Draw() {
        for (const auto& cube : cubes) {
            Vector3 pos = cube.position;
            float size = 0.95f;

            DrawCube(pos, size, size, size, GRAY);

            for (const auto& face : cube.faces) {
                Vector3 facePos = pos;
                Color color = face.second;

                switch (face.first) {
                case 0: facePos.y -= 0.0175f; break;
                case 1: facePos.y += 0.0175f; break;
                case 2: facePos.x -= 0.0175f; break;
                case 3: facePos.x += 0.0175f; break;
                case 4: facePos.z -= 0.0175f; break;
                case 5: facePos.z += 0.0175f; break;
                }

                DrawCubeFace(facePos, size, face.first, color);
            }
        }
    }

    void RotateFace(int face, bool clockwise) {
        char axis;
        int coord;

        switch (face) {
        case 'U': axis = 'y'; coord = 1; break;
        case 'D': axis = 'y'; coord = -1; break;
        case 'L': axis = 'x'; coord = -1; break;
        case 'R': axis = 'x'; coord = 1; break;
        case 'F': axis = 'z'; coord = 1; break;
        case 'B': axis = 'z'; coord = -1; break;
        default: return;
        }

        vector<SmallCube*> layerCubes;
        for (auto& cube : cubes) {
            if ((axis == 'x' && (int)cube.position.x == coord) ||
                (axis == 'y' && (int)cube.position.y == coord) ||
                (axis == 'z' && (int)cube.position.z == coord)) {
                layerCubes.push_back(&cube);
            }
        }

        for (SmallCube* cube : layerCubes) {
            Vector3 p = cube->position;
            Vector3 newPos;

            if (axis == 'x') {
                if (clockwise)
                    newPos = { p.x, -p.z, p.y };
                else
                    newPos = { p.x, p.z, -p.y };
            }
            else if (axis == 'y') {
                if (clockwise)
                    newPos = { p.z, p.y, -p.x };
                else
                    newPos = { -p.z, p.y, p.x };
            }
            else if (axis == 'z') {
                if (clockwise)
                    newPos = { -p.y, p.x, p.z };
                else
                    newPos = { p.y, -p.x, p.z };
            }

            cube->position = newPos;

            cube->RotateFacesAroundAxis(axis, clockwise);
        }
    }
};

int main() {
    const int screenWidth = 1280;
    const int screenHeight = 720;
    float cameraYaw = 45.0f;
    float cameraPitch = 20.0f;
    float cameraDistance = 8.0f;
    Vector2 lastMousePos = GetMousePosition();
    InitWindow(screenWidth, screenHeight, "Rubik's Cube 3D");

    Camera3D camera = { 0 };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;

    RubiksCube cube;

    SetTargetFPS(120);
    DisableCursor();

    while (!WindowShouldClose()) {
        Vector2 mouseDelta = GetMouseDelta();

        if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
            cameraYaw += mouseDelta.x * 0.3f;
            cameraPitch += mouseDelta.y * 0.3f;

            if (cameraPitch > 89.0f) cameraPitch = 89.0f;
            if (cameraPitch < -89.0f) cameraPitch = -89.0f;
        }

        cameraDistance -= GetMouseWheelMove() * 0.3f;
        if (cameraDistance < 2.0f) cameraDistance = 2.0f;
        if (cameraDistance > 15.0f) cameraDistance = 15.0f;

        float yawRad = cameraYaw * DEG2RAD;
        float pitchRad = cameraPitch * DEG2RAD;

        camera.position.x = cameraDistance * cosf(pitchRad) * sinf(yawRad);
        camera.position.y = cameraDistance * sinf(pitchRad);
        camera.position.z = cameraDistance * cosf(pitchRad) * cosf(yawRad);

        camera.target = { 0.0f, 0.0f, 0.0f };

        bool shift = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);

        if (IsKeyPressed(KEY_D)) cube.RotateFace('L', !shift);
        if (IsKeyPressed(KEY_A)) cube.RotateFace('R', !shift);
        if (IsKeyPressed(KEY_Q)) cube.RotateFace('U', !shift);
        if (IsKeyPressed(KEY_E)) cube.RotateFace('D', !shift);
        if (IsKeyPressed(KEY_W)) cube.RotateFace('F', !shift);
        if (IsKeyPressed(KEY_S)) cube.RotateFace('B', !shift);

        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode3D(camera);
        cube.Draw();
        EndMode3D();

        DrawMiniCompass(camera, screenWidth - 180, 10, 150);

        DrawText("Controls:", 10, 10, 20, WHITE);
        DrawText("Upper Face (Q)", 10, 35, 20, WHITE);
        DrawText("Lower Face (E)", 10, 60, 20, WHITE);
        DrawText("West Face (A)", 10, 85, 20, WHITE);
        DrawText("East Face (D)", 10, 110, 20, WHITE);
        DrawText("North Face (W)", 10, 135, 20, WHITE);
        DrawText("South Face (S)", 10, 160, 20, WHITE);
        DrawText("Hold SHIFT to rotate in the other direction", 10, 185, 20, WHITE);

        DrawText("Press ESC to exit", 10, 230, 20, WHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
